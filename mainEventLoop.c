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
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#//include <time.h>
#include <sys/time.h>

extern const char* Queue_TypeEv_e[];
extern alignas(64) Event_t ev; //if cacheline 64b

alignas(64) Event_t mainEventLoop_Events_array[EV_RECORD_SIZE];
uint8_t ev_arr_index = 0;

int mainEventLoop_NumOfEvents[NUM_TYPEVENTS];

char g_status = '0';
struct timeb tstart, tend;
uint8_t tdiff;

pthread_cond_t cond= PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock= PTHREAD_MUTEX_INITIALIZER;

/* **************** */
void* monitorThread(){//monitor, notify on valid status
    int m = 0;
    while(m<5)
    {
        ++m;
        sleep(1);
        pthread_mutex_lock(&lock);
        printf("\n1.monitor monitoring events .. queue size:%d \n", Queue_Show());

        if(0 < Queue_Show())
        {
            printf("\n2.monitor signaling condition set\n");
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

/* ************************* */
void* dispatchThread(){
    while(1)
    {
        printf("\n1.dispatch waiting to receive.. \n");
        //sleep(1);
        pthread_mutex_lock(&lock);
        pthread_cond_wait(&cond,&lock);//unlocks if cond not true/signaled and waits
        //work receive
        printf("\n2.dispatching received event\n");//<< events.front().desc<<" "<<events.front().severity <<std::endl;

        if (0 < Queue_Show())
        {
            //work
            //record
            mainEventLoop_Events_array[ev_arr_index] = Queue_Dequeue();
            printf("Record array %s", Queue_TypeEv_e[mainEventLoop_Events_array[ev_arr_index].typ_e]);
            ++ev_arr_index;

            //num Events
            ++mainEventLoop_NumOfEvents[mainEventLoop_Events_array[ev_arr_index-1].typ_e];
            printf("\nnum of these events %u",
                    mainEventLoop_NumOfEvents[mainEventLoop_Events_array[ev_arr_index-1].typ_e]);
            //rate of events
            ftime(&tend);
            //tdiff = difftime(&tend, &tstart);
            tdiff = (int) (1000.0 * (tend.time - tstart.time) + tend.millitm - tstart.millitm);
            printf("\nTime difference %u ms\n", tdiff);

            if(mainEventLoop_Events_array[ev_arr_index-1].sev_e == E_MEDIUM_SEVERITY)
            {
                    printf("\n\nE_MEDIUM_SEVERITY event, exiting.........\n");
                    system("pause");
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
    pthread_t thread1_id;
    pthread_t thread2_id;

    if(pthread_create(&thread1_id, NULL, monitorThread, NULL) != 0) //xxx
    {
        // raise critical
        pthread_mutex_lock(&lock);
        TriggerEvent_TrigEvSim(E_MEDIUM_SEVERITY, E_MEMORY, __FILE__, __LINE__, "0");
        //record and exit
        mainEventLoop_Events_array[ev_arr_index] = ev;
        printf("Record array %s in file %s",
               Queue_TypeEv_e[mainEventLoop_Events_array[ev_arr_index].typ_e],
               mainEventLoop_Events_array[ev_arr_index].file_loc);
        printf("\n\nE_MEDIUM_SEVERITY event, exiting.........\n");
                system("pause");
        exit(1); //restart
    }
    if(pthread_create(&thread2_id,NULL, dispatchThread, NULL) !=0){
        //raise critical
        pthread_mutex_lock(&lock);
        TriggerEvent_TrigEvSim(E_MEDIUM_SEVERITY, E_MEMORY, __FILE__, __LINE__, "0");
        //record and exit
        mainEventLoop_Events_array[ev_arr_index] = ev;
        printf("Record array %s in file %s",
               Queue_TypeEv_e[mainEventLoop_Events_array[ev_arr_index].typ_e],
               mainEventLoop_Events_array[ev_arr_index].file_loc);
        printf("\n\nE_MEDIUM_SEVERITY event, exiting.........\n");
                system("pause");
        exit(1); //restart
    }

    int i=0;
    //while( i <2)
    {
    ++i;
        pthread_mutex_lock(&lock);
        g_status = TriggerEvent_TrigEvSim(E_LOW_SEVERITY, E_GENERAL, __FILE__, __LINE__, "0");
        //Queue_Dequeue(); Queue_Dequeue();
        g_status = TriggerEvent_TrigEvSim(E_NORMAL_REPORT, E_FILENOTFOUND, __FILE__, __LINE__, "0");
        g_status = TriggerEvent_TrigEvSim(E_MEDIUM_SEVERITY, E_NULL_ARG, __FILE__, __LINE__, "0");
        pthread_mutex_unlock(&lock);

        ftime(&tstart);
    }

    pthread_join(thread1_id,NULL);
	pthread_join(thread2_id,NULL);

	return 0;
}
