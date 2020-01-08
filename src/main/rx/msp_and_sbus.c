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

static uint8_t offboard_chan = 5;
static uint16_t offboard_threshold_l = 1600;
static uint16_t offboard_threshold_h = 2000;
static bool offboard = false;

static uint16_t rxMspAndSbusReadRawRC(const rxRuntimeConfig_t *rxRuntimeConfig, uint8_t chan)
{
    uint16_t offboard_chan_value = sbusChannelsReadRawRC(rxRuntimeConfig, offboard_chan);

    if (offboard_chan_value >= offboard_threshold_l && offboard_chan_value <= offboard_threshold_h) {
        offboard = true;
    }
    else {
        offboard = false;
    }

    uint16_t sbus_value = sbusChannelsReadRawRC(rxRuntimeConfig, chan);
    uint16_t msp_value = rxMspReadRawRC(rxRuntimeConfig, chan);

    if (offboard) {
        return msp_value;
    }
    else {
        return sbus_value;
    }
}

static uint8_t rxMspAndSbusFrameStatus(rxRuntimeConfig_t *rxRuntimeConfig)
{
    uint8_t sbus_res = sbusFrameStatus(rxRuntimeConfig);
    uint8_t msp_res = rxMspFrameStatus(rxRuntimeConfig);

    if (offboard) {
        return msp_res;
    }
    else {
        return sbus_res;
    }
}

bool rxMspAndSbusInit(const rxConfig_t *rxConfig, const mspAndSbusConfig_t *mspAndSbusConfig, rxRuntimeConfig_t *rxRuntimeConfig)
{
    UNUSED(mspAndSbusConfig);
    offboard_chan = mspAndSbusConfig->offboard_chan;
    offboard_threshold_l = mspAndSbusConfig->offboard_threshold_l;
    offboard_threshold_h = mspAndSbusConfig->offboard_threshold_h;
    rxMspInit(rxConfig, rxRuntimeConfig);
    sbusInit(rxConfig, rxRuntimeConfig);

    rxRuntimeConfig->rcReadRawFn = rxMspAndSbusReadRawRC;
    rxRuntimeConfig->rcFrameStatusFn = rxMspAndSbusFrameStatus;

    return true;
}
#endif
