#include "platform.h"

#if defined(USE_RX_MSP_AND_SBUS)

#ifndef DEFAULT_OFFBOARD_CHAN
#define DEFAULT_OFFBOARD_CHAN 5
#endif

#include "pg/pg.h"
#include "pg/pg_ids.h"

#include "pg/rx_msp_and_sbus.h"

PG_REGISTER_WITH_RESET_FN(mspAndSbusConfig_t, mspAndSbusConfig, PG_RX_MSP_AND_SBUS_CONFIG, 0);

void pgResetFn_mspAndSbusConfig(mspAndSbusConfig_t *mspAndSbusConfig)
{
    mspAndSbusConfig->offboard_chan = DEFAULT_OFFBOARD_CHAN;
    mspAndSbusConfig->offboard_threshold_l = 1600;
    mspAndSbusConfig->offboard_threshold_h = 2000;
}

#endif
