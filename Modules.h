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
 *  @file Modules.h
 *  @author Petr Buben
 *  @brief Definition of enumerate containing names of all modules
 *
 * Copyright 2021 esc Aerospace s.r.o., All rights reserved.
 */

#ifndef __MODULES_H__
#define __MODULES_H__

#include <stdio.h>

/* Typedef containing all modules */
typedef enum
{
	E_MODULES_ID_MAINEVENTLOOP           = 1U,
	E_MODULES_ID_EVENT                   = 2U,
	E_MODULES_ID_QUEUE                   = 3U,
	E_MODULES_ID_TRIGGEREVENT            = 4U,
	E_MODULES_ID_WRITEAPPENDEVENTS       = 5U,
	E_MODULES_ID_NVMMEM                  = 6U
} Modules_Id_e;

#endif /* __MODULES_H__ */
