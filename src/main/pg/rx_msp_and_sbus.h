#pragma once

#include "drivers/io_types.h"

#include "pg/pg.h"

typedef struct mspAndSbusConfig_s {
    uint8_t offboard_chan;
    uint16_t offboard_threshold_l;
    uint16_t offboard_threshold_h;
} mspAndSbusConfig_t;

PG_DECLARE(mspAndSbusConfig_t, mspAndSbusConfig);
