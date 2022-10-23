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
 *  @file WriteAppendEvents.h
 *  @author Petr Buben
 *  @brief Header file for WriteAppendEvents.c
 *
 * Copyright 2022 esc Aerospace s.r.o., All rights reserved.
 */

#ifndef __WRITEAPPENDEVENTS_H__
#define __WRITEAPPENDEVENTS_H__

#include "Event.h"

 #define WA_FILENAME "events.txt"

void WriteAppendEvens_write_file(Event_t *);


#endif // __WRITEAPPENDEVENTS_H__
