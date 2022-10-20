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
 *  @file Queue.c
 *  @author Petr Buben
 *  @brief C array implementation of raw, report event queues
 *
 * Copyright 2022 esc Aerospace s.r.o., All rights reserved.
 */

#include "Queue.h"
#include "Event.h"
#include "TriggerEvent.h"
#include <stdlib.h>

#define RAW_Q_SIZE 100

extern alignas(64) Event_t ev; //if cacheline 64b
extern uint8_t ev_arr_index;
alignas(64) Event_t mainEventLoop_Events_array[EV_RECORD_SIZE];

const char* Queue_SevEv_e[] = {"E_LOW_SEVERITY","E_NORMAL_REPORT","E_MEDIUM_SEVERITY"};
const char* Queue_TypeEv_e[] = {"E_NULL_ARG", "E_FILENOTFOUND", "E_RAW_QUEUE_UNDERFLOW",
"E_RAW_QUEUE_OVERFLOW","E_MEMORY","E_GENERAL"};

Event_t Queue_Raw_events_queue[RAW_Q_SIZE];
int Rear = -1;
int Front = -1;

/* ********************* */
void Queue_Enqueue(Event_t * prEvent)
{
    if (Rear == (RAW_Q_SIZE - 1))
    {
       printf("Overflow \n"); //tbd exc
       //record and exit
       TriggerEvent_TrigEvSim(E_MEDIUM_SEVERITY, E_RAW_QUEUE_OVERFLOW, __FILE__, __LINE__, "0");
       mainEventLoop_Events_array[ev_arr_index] = ev;
       printf("Record array %s in file %s", Queue_TypeEv_e[mainEventLoop_Events_array[ev_arr_index].typ_e],
              mainEventLoop_Events_array[ev_arr_index].file_loc);
       printf("\n\nE_MEDIUM_SEVERITY event, exiting.........\n");
       system("pause");
       exit(-1);
    }
    else
    {
        if (-1 == Front)
        {
            Front = 0;
        }
        ++Rear;
        printf("Element type to be inserted: %s  Queue size:%d \n", Queue_TypeEv_e[(*prEvent).typ_e],
                Rear+1-Front);
        Queue_Raw_events_queue[Rear] = *prEvent; //enqueue
        return; // prEvent; //Queue_Raw_events_queue[Rear];
    }
}

/* ******************** */
Event_t Queue_Dequeue(void)
{
    if ((-1 == Front) || (Front > Rear))
    {
        printf("\nUnderflow \n");
        //record and exit tbd
        TriggerEvent_TrigEvSim(E_MEDIUM_SEVERITY, E_RAW_QUEUE_UNDERFLOW, __FILE__, __LINE__, "0");
        mainEventLoop_Events_array[ev_arr_index] = ev;
        printf("Record array %s in file %s",
               Queue_TypeEv_e[mainEventLoop_Events_array[ev_arr_index].typ_e],
               mainEventLoop_Events_array[ev_arr_index].file_loc);
        printf("\n\nE_MEDIUM_SEVERITY event, exiting.........\n");
        system("pause");
        exit(-1);
    }
    else
    {
        printf("Element processed: %s  Queue size:%d\n",
               Queue_TypeEv_e[Queue_Raw_events_queue[Front].typ_e], Rear-Front);
        ++Front; //dequeue
    }
    return Queue_Raw_events_queue[Front-1];
}

uint8_t Queue_Show(void)
{
    if ((Rear == Front-1) ||(-1 == Front))
    {
        printf("Empty Queue \n");
        return 0;
    }
    else
    {
        printf("Queue:\n");
        for (int i = Front; i <= Rear; ++i)
        {
            printf(
            "Event type:%s Severity:%s File:\"%s\" Line:%u",
    Queue_TypeEv_e[Queue_Raw_events_queue[i].typ_e], Queue_SevEv_e[Queue_Raw_events_queue[i].sev_e],
             //Queue_Raw_events_queue[i].typ_e, Queue_Raw_events_queue[i].sev_e,
    Queue_Raw_events_queue[i].file_loc, Queue_Raw_events_queue[i].line_num);
            printf("\n");
        }
    }
    return Rear-Front+1;
}
