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
 *  @file TriggerEvent.c
 *  @author Petr Buben
 *  @brief Simulating triggering of events
 *
 * Copyright 2022 esc Aerospace s.r.o., All rights reserved.
 */

#include "TriggerEvent.h"
#include "Queue.h"
#include <stdio.h>
#include <string.h>

alignas(64) Event_t ev; //if cacheline 64b

 char TriggerEvent_TrigEvSim(Severity_et es_e, Type_et et_e, char* file, uint8_t line, char userPar[32]){
    //alignas(64) Event_t ev; //if cacheline 64b
    ev.sev_e = es_e;
    ev.typ_e = et_e;
    strcpy_s(ev.file_loc, sizeof(ev.file_loc), file);
    ev.line_num = line;
    strcpy(ev.user_param,userPar);

    Queue_Enqueue(&ev);

    return '1';
 }

    /* test of queue implementation
    printf("qs size:%d\n", Queue_Show());

    Queue_Dequeue();
    printf("qs size:%d\n", Queue_Show());

    strcpy_s(ev.Ev_desc, sizeof("desc2"), "desc2");
    Queue_Enqueue(&ev);
    printf("qs size:%d\n", Queue_Show());

    strcpy_s(ev.Ev_desc, sizeof("desc3"), "desc3");
    Queue_Enqueue(&ev);
    printf("qs size:%d\n", Queue_Show());

    Queue_Dequeue();
    printf("qs size:%d\n", Queue_Show());

    strcpy_s(ev.Ev_desc, sizeof("desc3"), "desc3");
    Queue_Enqueue(&ev);
    printf("qs size:%d\n", Queue_Show());

    Queue_Dequeue();
    printf("qs size:%d\n", Queue_Show());

    Queue_Dequeue();
    printf("qs size:%d\n", Queue_Show());

    Queue_Dequeue();
    printf("qs size:%d\n", Queue_Show());
    */

