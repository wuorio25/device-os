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
 * MERCHAN'TABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#include "port.h"

#if PLATFORM_ID == PLATFORM_TRACKER

#include "application.h"
#include "concurrent_hal.h"
#include "delay_hal.h"
#include "sdspi_host.h"

// The semaphore indicating the slave is ready to receive stuff.
os_semaphore_t wifiIntSem = nullptr;

/// Set CS high
void at_cs_high(void) {
    digitalWrite(PORT_CS_PIN, HIGH);
    PORT_SPI.endTransaction();
}

/// Set CS low
void at_cs_low(void) {
    PORT_SPI.beginTransaction(__SPISettings(1*MHZ, MSBFIRST, SPI_MODE0));
    digitalWrite(PORT_CS_PIN, LOW);
}

esp_err_t at_spi_transmit(void* tx_buff, void* rx_buff, uint32_t len) {
    PORT_SPI.transfer(tx_buff, rx_buff, len, NULL);
    return ESP_OK;
}

static void esp32WakeupPinHandler(void) {
    if (wifiIntSem) {
        os_semaphore_give(wifiIntSem, false);
    }
}

esp_err_t at_spi_slot_init(void) {
    // Initialize PORT_SPI bus
    // For SD cards, CS must stay low during the whole read/write operation,
    // rather than a single SPI transaction.
    // pinMode(MISO, INPUT_PULLUP);
    PORT_SPI.setClockSpeed(100, KHZ);
    PORT_SPI.setDataMode(SPI_MODE0);
    PORT_SPI.begin();

    // Configure CS pin
    pinMode(PORT_CS_PIN, OUTPUT);
    digitalWrite(PORT_CS_PIN, HIGH);

    pinMode(PORT_INT_PIN, INPUT_PULLUP);
    // NOTE: Don't enable interrupt here on ATSoMv0.2
    // The unexpected interrupt casuses a SPI read on IO Expander,
    // which interferes with initial sequence of SDIO
    // attachInterrupt(PORT_INT_PIN, esp32WakeupPinHandler, FALLING);

    return ESP_OK;
}

esp_err_t at_spi_int_init(void) {
    attachInterrupt(PORT_INT_PIN, esp32WakeupPinHandler, FALLING);
    SPARK_ASSERT(os_semaphore_create(&wifiIntSem, 1, 0) == 0);
    if (digitalRead(PORT_INT_PIN) == LOW) {
        SPARK_ASSERT(os_semaphore_give(wifiIntSem, false) == 0);
    }
    return ESP_OK;
}

esp_err_t at_spi_wait_int(uint32_t wait_ms) {
    //wait for semaphore
    int ret = os_semaphore_take(wifiIntSem, wait_ms, false);
    if (ret) {
        // Log.error("wifiIntSem TIMEOUT!");
        return ESP_ERR_TIMEOUT;
    } else {
        // Log.info("wifiIntSem SUCCESS!");
    }

    return ESP_OK;
}

void at_do_delay(uint32_t wait_ms) {
    HAL_Delay_Milliseconds(wait_ms);
}

AT_MUTEX_T at_mutex_init(void) {
    os_mutex_t mutex = NULL;
    SPARK_ASSERT(os_mutex_create(&mutex) == 0);
    return mutex;
}

void at_mutex_lock(AT_MUTEX_T pxMutex) {
    os_mutex_lock(pxMutex);
}

void at_mutex_unlock(AT_MUTEX_T pxMutex) {
    os_mutex_unlock(pxMutex);
}

void at_mutex_free(AT_MUTEX_T pxMutex) {
    os_mutex_destroy(pxMutex);
    pxMutex = NULL;
}

#endif // PLATFORM_ID == PLATFORM_TRACKER
