/*
 * Copyright (c) 2020 Particle Industries, Inc.  All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

// This header defines private USART HAL APIs that may be implemented only on certain platforms
// and are not intended to be exported.
#pragma once

#include "usart_hal.h"
#include <FreeRTOS.h>
#include <event_groups.h>
#include "system_tick_hal.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef enum HAL_USART_Pvt_Events {
    HAL_USART_PVT_EVENT_READABLE = 0x01,
    HAL_USART_PVT_EVENT_WRITABLE = 0x02,
    HAL_USART_PVT_EVENT_MAX = HAL_USART_PVT_EVENT_WRITABLE
} HAL_USART_Pvt_Events;

int HAL_USART_Pvt_Get_Event_Group_Handle(HAL_USART_Serial serial, EventGroupHandle_t* handle);
int HAL_USART_Pvt_Enable_Event(HAL_USART_Serial serial, HAL_USART_Pvt_Events events);
int HAL_USART_Pvt_Disable_Event(HAL_USART_Serial serial, HAL_USART_Pvt_Events events);
int HAL_USART_Pvt_Wait_Event(HAL_USART_Serial serial, uint32_t events, system_tick_t timeout);

#ifdef __cplusplus
}
#endif // __cplusplus
