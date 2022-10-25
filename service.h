/*
 ******************************************************************************
 *                                                                            *
 *                                                                            *
 *                           ALL RIGHTS RESERVED                              *
 *                                                                            *
 ******************************************************************************
 */

/**
 *  @file service.h
 *  @author Petr Buben
 *  @brief Header file for service.c
 *
 * Copyright 2022 s.r.o., All rights reserved.
 */

 #ifndef __SERVICE_H__
 #define __SERVICE_H__

 #include "Event.h"
 #include <stdint.h>

 void mainEventLoop_Raise_critical(Type_et, char*, uint8_t);
 float figure_rate(void);
 void Record_rate_count_write_file(void);

 #endif
