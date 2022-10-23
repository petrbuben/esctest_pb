/*
 ******************************************************************************
 *                                                                            *
 *                           esc Aerospace s.r.o.                             *
 *                       https://www.esc-aerospace.com                        *
 *                                                                            *
 *                           ALL RIGHTS RESERVED                              *
 *                                                                            *
 ******************************************************************************
 */

/**
 *  @file mainEventLoop.c
 *  @author Petr Buben
 *  @brief  This module is used to write data into the non-volatile memory.
 *
 * Copyright 2021 esc Aerospace s.r.o., All rights reserved.
 */

#include "TriggerEvent.h"
#include "Queue.h"
#include "WriteAppendEvens_write_file.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

extern const char* Queue_TypeEv_e[];
extern alignas(64) Event_t TriggerEvent_ev; //if cacheline 64b

alignas(64) Event_t mainEventLoop_Events_record_array[EV_RECORD_SIZE];
uint8_t ev_arr_index = 0u;
int rate_cond = 0;

alignas(64) int mainEventLoop_NumOfEvents[NUM_TYPEVENTS];

char g_status = '0';
struct timeb tstart, tend;
int tdiff = 0;
float rate_per_type = 0.0;
char reset_continue = '0';
uint8_t queue_count = 0u;

//mask to disable events
char Type_enabled[NUM_TYPEVENTS]={'1', '1', '1', '1', '1', '0'};

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

/* ******************************************************************************* */
void* monitorThread(){//monitor, notify on valid status
    int m = 0;
    while(m<5)
    {
        ++m;
        sleep(1);
        pthread_mutex_lock(&lock);
        printf("\n1.monitor monitoring events .. queue size:%d\n", queue_count=Queue_Show());

        if(0 < queue_count)
        {
            printf("\n2.monitor SIGNALING condition set\n");
            pthread_cond_signal(&cond);
        }
        else
        {
            g_status = '0';
        }
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

/* ******************************************************************************* */
void* dispatchThread(){
    while(1)
    {
        printf("\n1.dispatch waiting to receive.. \n");
        //sleep(1);
        pthread_mutex_lock(&lock);
        pthread_cond_wait(&cond,&lock);//unlocks if cond not true/signaled and waits
        //work receive
        printf("\n2.dispatching received event\n");//<< events.front().desc<<" "<<events.front().severity <<std::endl;

        if (0 < queue_count)
        {
            //work ****************************** */
            //deque - record
            mainEventLoop_Events_record_array[ev_arr_index] = Queue_Dequeue();

            //disable single types
            if ('0' == Type_enabled[mainEventLoop_Events_record_array[ev_arr_index].typ_e])
            {
                //disabled, ignore events of this type
                printf("Event type %s is disabled, ignoring..",
                       Queue_TypeEv_e[mainEventLoop_Events_record_array[ev_arr_index].typ_e]);

                pthread_mutex_unlock(&lock);
                continue;
            }


            //rate of events
            if(-1 != rate_cond)//not count only
            {
                ftime(&tend);
                tdiff = (int) (1000.0 * (tend.time - tstart.time) + tend.millitm - tstart.millitm);
                printf("Time difference %d ms\n", tdiff);
                //count / tdiff
                rate_per_type =
                (float)1000*(1+mainEventLoop_Events_record_array[ev_arr_index].type_event_count)
                /(float)tdiff;

                printf("rate for event type %s is %.3f per sec\n",
                    Queue_TypeEv_e[mainEventLoop_Events_record_array[ev_arr_index].typ_e],
                    rate_per_type);
            }

            //process rate exception, bigger rate, no go ***********************
            if ( (1 >= mainEventLoop_Events_record_array[ev_arr_index].sev_e
                    && rate_per_type < RATE_TRESHOLDevPerS)
                   ||
                 (1 < mainEventLoop_Events_record_array[ev_arr_index].sev_e )
                   ||
                 (-1 == rate_cond)
                  )
            { //can go
                if (-1 == rate_cond) //if previous
                {
                    ftime(&tend);
                    tdiff = tend.time - tstart.time; //figuring pause difference now

                    if(tdiff >= REPORT_RESTARTmins)//ok to restart
                    {
                      //reset after rate cond -1 : pause expired
                      rate_cond = 0;
                      ftime(&tstart); //normal time
                      pthread_mutex_unlock(&lock);
                      continue;
                      /*
                      if(0 == remove(WA_FILENAME)) //delete file
                      {
                         printf("File deleted successfully\n");
                      }
                      else
                      {
                          printf("No file to delete\n");
                            //raise critical, exit
                      }
                      */
                    }
                    else
                    {
                        printf("Waiting to clear event overflow...\n");
                    }

                }//end of cond set 1 no go

                if(0 == rate_cond) //go, no overflow
                {
                    //work
                    //record rate
                    mainEventLoop_Events_record_array[ev_arr_index].type_event_rate = rate_per_type;

                    //increase this event count
                    ++mainEventLoop_Events_record_array[ev_arr_index].type_event_count;
                    printf("Record array %s",
                           Queue_TypeEv_e[mainEventLoop_Events_record_array[ev_arr_index].typ_e]);

                    //append file tbd
                    WriteAppendEvens_write_file(&mainEventLoop_Events_record_array[ev_arr_index]);

                    ++ev_arr_index;
                    //ftime(&tstart);
                }
            } //rate processing end
            else //set rate condition no go
            {
                printf("Low or/and NORMAL events exception - rate over %.3f per sec\n",
                       RATE_TRESHOLDevPerS);
                printf("Only recording count per type\n");

                if(0 == rate_cond){
                    ftime(&tstart); //to count pause
                    rate_cond = -1;
                }
            }

            //num Events per type array
            ++mainEventLoop_NumOfEvents
                [mainEventLoop_Events_record_array[ev_arr_index-1-rate_cond].typ_e];
            printf("\nnum of these events %u", mainEventLoop_NumOfEvents
                     [mainEventLoop_Events_record_array[ev_arr_index-1-rate_cond].typ_e]);

            if(mainEventLoop_Events_record_array[ev_arr_index-1-rate_cond].sev_e == E_MEDIUM_SEVERITY)
            {
                printf("\n\nE_MEDIUM_SEVERITY event, reseting.........\n");
                system("pause");
                reset_continue = '1';
                pthread_mutex_unlock(&lock);
                pthread_exit(NULL);
                //exit(-1);
            }
        }
        else
        {
            g_status = '0';
        }

        pthread_mutex_unlock(&lock);
	}
}

/* ************** */
int main(int argc, char *argv[])
{
    do
    {
        reset_continue = '0';
        //initialize event
        TriggerEvent_ev.type_event_count = 0;
        TriggerEvent_ev.type_event_rate = 0.0;

        pthread_t thread1_id;
        pthread_t thread2_id;

        if(pthread_create(&thread1_id, NULL, monitorThread, NULL) != 0) //xxx
        {
            // raise critical
            pthread_mutex_lock(&lock);
            //record and exit
            TriggerEvent_TrigEvSim(tstart, E_MEDIUM_SEVERITY, E_MEMORY, __FILE__, __LINE__, "0");
            mainEventLoop_Events_record_array[ev_arr_index] = TriggerEvent_ev;
            printf("Record array %s in file %s",
                   Queue_TypeEv_e[mainEventLoop_Events_record_array[ev_arr_index].typ_e],
                   mainEventLoop_Events_record_array[ev_arr_index].file_loc);
            printf("\n\nE_MEDIUM_SEVERITY event, exiting.........\n");
                    system("pause");
            exit(1); //restart
        }
        if(pthread_create(&thread2_id,NULL, dispatchThread, NULL) !=0){
            //raise critical
            pthread_mutex_lock(&lock);
            //record and exit
            TriggerEvent_TrigEvSim(tstart, E_MEDIUM_SEVERITY, E_MEMORY, __FILE__, __LINE__, "0");
            mainEventLoop_Events_record_array[ev_arr_index] = TriggerEvent_ev;
            printf("Record array %s in file %s",
                   Queue_TypeEv_e[mainEventLoop_Events_record_array[ev_arr_index].typ_e],
                   mainEventLoop_Events_record_array[ev_arr_index].file_loc);
            printf("\n\nE_MEDIUM_SEVERITY event, exiting.........\n");
                    system("pause");
            exit(1); //restart
        }

        int i=0;

        while( i < 1) //num of events simulated
        {
        ++i;
            pthread_mutex_lock(&lock);
            ftime(&tstart); //all time
            g_status = TriggerEvent_TrigEvSim(tstart, E_LOW_SEVERITY, E_GENERAL, __FILE__, __LINE__, "0");
            //Queue_Dequeue(); Queue_Dequeue();
            sleep(1);
            g_status = TriggerEvent_TrigEvSim(tstart, E_NORMAL_REPORT, E_FILENOTFOUND, __FILE__, __LINE__, "0");
            sleep(1);
            g_status = TriggerEvent_TrigEvSim(tstart, E_MEDIUM_SEVERITY, E_NULL_ARG, __FILE__, __LINE__, "0");
            sleep(1);
            g_status = TriggerEvent_TrigEvSim(tstart, E_LOW_SEVERITY, E_MEMORY, __FILE__, __LINE__, "0");
            sleep(1);
            g_status = TriggerEvent_TrigEvSim(tstart, E_NORMAL_REPORT, E_GENERAL, __FILE__, __LINE__, "0");
            sleep(1);
            pthread_mutex_unlock(&lock);
            //ftime(&tend);
        }

        pthread_join(thread2_id,NULL);
        pthread_cancel(thread1_id);
        //pthread_join(thread1_id,NULL);

    } while('1' == reset_continue);

	return 0;
}
