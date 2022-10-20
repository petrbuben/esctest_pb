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
 *  @file Storage.h
 *  @author esc
 *  @brief This module stores/loads data to/from local non-volatile memory.
 *
 * Copyright 2021 esc Aerospace s.r.o., All rights reserved.
 */

#ifndef __STORAGE_H__
#define __STORAGE_H__

//#include "Common.h"
#include <stdint.h>


/**
 * @brief The function stores event report in local memory.
 *
 * @param in_pu8EventData   Event report data array
 * @param in_u32DataSize    Size of event report data in bytes
 */
void Storage_StoreEventReport(const uint8_t *in_pu8EventData, uint32_t in_u32DataSize);

#endif /* __STORAGE_H__ */
