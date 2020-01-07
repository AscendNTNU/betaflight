/*
 * This file is part of Cleanflight and Betaflight.
 *
 * Cleanflight and Betaflight are free software. You can redistribute
 * this software and/or modify this software under the terms of the
 * GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * Cleanflight and Betaflight are distributed in the hope that they
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software.
 *
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "platform.h"


#ifdef USE_RX_MSP_AND_SBUS

#include "build/debug.h"

#include "common/utils.h"

#include "drivers/time.h"

#include "io/serial.h"

#ifdef USE_TELEMETRY
#include "telemetry/telemetry.h"
#endif

#include "pg/rx.h"

#include "rx/rx.h"
#include "rx/sbus.h"
#include "rx/sbus_channels.h"
#include "rx/msp.h"
#include "rx/msp_and_sbus.h"

#ifndef MSP_AND_SBUS_OFFBOARD_CHAN
#define MSP_AND_SBUS_OFFBOARD_CHAN 7
#endif

static bool offboard = false;

static uint16_t rxMspAndSbusReadRawRC(const rxRuntimeConfig_t *rxRuntimeConfig, uint8_t chan)
{
    if (sbusChannelsReadRawRC(rxRuntimeConfig, MSP_AND_SBUS_OFFBOARD_CHAN) > 1500) {
        offboard = true;
    }
    else {
        offboard = false;
    }

    if (offboard) {
        return rxMspReadRawRC(rxRuntimeConfig, chan);
    }
    else {
        return sbusChannelsReadRawRC(rxRuntimeConfig, chan);
    }
}

static uint8_t rxMspAndSbusFrameStatus(rxRuntimeConfig_t *rxRuntimeConfig)
{
    if (offboard) {
        return rxMspFrameStatus(rxRuntimeConfig);
    }
    else {
        return sbusFrameStatus(rxRuntimeConfig);
    }
}

bool rxMspAndSbusInit(const rxConfig_t *rxConfig, rxRuntimeConfig_t *rxRuntimeConfig) {
    rxMspInit(rxConfig, rxRuntimeConfig);
    sbusInit(rxConfig, rxRuntimeConfig);

    rxRuntimeConfig->rcReadRawFn = rxMspAndSbusReadRawRC;
    rxRuntimeConfig->rcFrameStatusFn = rxMspAndSbusFrameStatus;

    return true;
}
#endif
