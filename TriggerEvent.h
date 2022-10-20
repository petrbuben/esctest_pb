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
 *  @file TrigerEvent.h
 *  @author Petr Buben
 *  @brief Header file for TriggerEvent.c
 *
 * Copyright 2022 esc Aerospace s.r.o., All rights reserved.
 */

#ifndef __TRIGGEREVENT_H__
#define __TRIGGEREVENT_H__

#include "Event.h"

char TriggerEvent_TrigEvSim(Severity_et, Type_et, char* file, uint8_t line, char [32]);

#endif /* __TRIGGEREVENT_H__ */
