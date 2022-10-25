/*
 ******************************************************************************
 *                                                                            *
 *                                                                            *
 *                           ALL RIGHTS RESERVED                              *
 *                                                                            *
 ******************************************************************************
 */

/**
 *  @file Queue.h
 *  @author Petr Buben
 *  @brief Header file for Queue.c
 *
 * Copyright 2022 s.r.o., All rights reserved.
 */

#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "Event.h"
#include <stdio.h>
#include <stdalign.h>
#include <sys/timeb.h>

#define RAW_Q_SIZE 100

void Queue_Enqueue(Event_t* prEvent);
Event_t Queue_Dequeue(void);
uint8_t Queue_Show(void);


#endif /* __QUEUE_H__ */
