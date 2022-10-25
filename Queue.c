/*
 ******************************************************************************
 *                                                                            *
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
 * Copyright 2022 s.r.o., All rights reserved.
 */

#include "Queue.h"
#include "Event.h"
#include "Modules.h"
#include "TriggerEvent.h"
#include "service.h"
#include <stdlib.h>

//static const Modules_Id_e m_eModuleId = E_MODULES_ID_QUEUE;

extern alignas(64) Event_t TriggerEvent_ev; //if cacheline 64b
extern uint8_t ev_arr_index;
extern alignas(64) Event_t mainEventLoop_Events_record_array[EV_RECORD_SIZE];
extern struct timeb tstart, tend;

alignas(64) const char* Queue_SevEv_e[] = {"E_LOW_SEVERITY","E_NORMAL_REPORT","E_MEDIUM_SEVERITY"};
alignas(64) const char* Queue_TypeEv_e[] = {"E_NULL_ARG", "E_FILENOTFOUND", "E_RAW_QUEUE_UNDERFLOW",
"E_RAW_QUEUE_OVERFLOW","E_MEMORY","E_FILE","E_GENERAL"};
char Type_enabled[NUM_TYPEVENTS] = {'1', '1', '1', '1', '1', '1', '0'};

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
       mainEventLoop_Raise_critical(E_RAW_QUEUE_OVERFLOW, __FILE__, __LINE__);

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
        mainEventLoop_Raise_critical(E_RAW_QUEUE_UNDERFLOW, __FILE__, __LINE__);

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
    if ((Rear == Front-1) || (-1 == Front))
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
