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
 *  @file Event.h
 *  @author Petr Buben
 *  @brief Header file for the Event structure
 *
 * Copyright 2022 esc Aerospace s.r.o., All rights reserved.
 */

#ifndef __EVENT_H__
#define __EVENT_H__

#include <stdint.h>

#define EV_FILE_LOC_SIZE  400
#define NUM_TYPEVENTS       7
#define EV_RECORD_SIZE    100
#define RATE_TRESHOLDevPerS  0.17 //.1
#define REPORT_RESTARTmins  1 //10

typedef enum
{
    E_LOW_SEVERITY,
    E_NORMAL_REPORT,
    E_MEDIUM_SEVERITY
} Severity_et;

typedef enum
{
    E_NULL_ARG, //sev medium
    E_FILENOTFOUND,
    E_RAW_QUEUE_UNDERFLOW,
    E_RAW_QUEUE_OVERFLOW,
    E_MEMORY,
    E_FILE,
    E_GENERAL
} Type_et;

 typedef struct Event { //listedlargest to smallest - padding avoidance
    char file_loc [EV_FILE_LOC_SIZE];
    char user_param[32];
    Severity_et sev_e;
    Type_et typ_e;
    float type_event_rate;
    uint8_t line_num;
    int type_event_count;
 } Event_t;

 #endif /* __EVENT_H__ */
