/*
 ******************************************************************************
 *                                                                            *
 *                                                                            *
 *                           ALL RIGHTS RESERVED                              *
 *                                                                            *
 ******************************************************************************
 */

/**
 *  @file TrigerEvent.h
 *  @author Petr Buben
 *  @brief Header file for TriggerEvent.c
 *
 * Copyright 2022 s.r.o., All rights reserved.
 */

#ifndef __TRIGGEREVENT_H__
#define __TRIGGEREVENT_H__

#include "Event.h"
#include <sys/timeb.h>

char TriggerEvent_TrigEvSim(struct timeb, Severity_et, Type_et, char* file, int line, char [32]);

#endif /* __TRIGGEREVENT_H__ */
