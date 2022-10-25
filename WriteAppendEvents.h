/*
 ******************************************************************************
 *                                                                            *
 *                                                                            *
 *                           ALL RIGHTS RESERVED                              *
 *                                                                            *
 ******************************************************************************
 */

/**
 *  @file WriteAppendEvents_write_file.h
 *  @author Petr Buben
 *  @brief Header file for WriteAppendEvents.c
 *
 * Copyright 2022 s.r.o., All rights reserved.
 */

#ifndef __WRITEAPPENDEVENTS_H__
#define __WRITEAPPENDEVENTS_H__

#include "Event.h"

 #define WA_FILENAME "events.txt"

void WriteAppendEvents_write_file(Event_t *);


#endif // __WRITEAPPENDEVENTS_H__
