#include <iox_rick.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

extern "C" {
#include <data.h>
#include <i2c.h>
#include <mcp23017.h>
#include <proc_iox.h>
}

#define IOX_RICK_ADDR 0x20

#ifdef NOI2C
#define VI2C
#endif

static i2c_settings iox;
static int setupIox(void);

/***
 *  initiox_rick - Meant to be overly pedantic
 *      even if the values arent changing,
 *      just to make extremely clear the initial
 *      state of everything.
 *
 */
iox_rick::iox_rick()
{
}

int iox_rick::init(bool hardStart)
{
    if (setupMCP(&iox, IOX_RICK_ADDR) != 0)
        return -1;
    else if (hardStart) {
        if (clearSettingsMCP(&iox) != 0)
            return -1;
        else if (this->setupIox() != 0)
            return -1;
    }
    return 0;
}

int iox_rick::setupIox()
{
    // static int setupIox() moved into constructor:
    setDir(&iox, MCU_LATCH, MCP_DIR_OUT);
    setDir(&iox, MSTR_SW_FDBK, MCP_DIR_IN);
    setDir(&iox, E_STOP_FDBK, MCP_DIR_IN);
    setDir(&iox, LMT_SW_FDBK, MCP_DIR_IN);
    setDir(&iox, BMS_STAT_FDBK, MCP_DIR_IN);
    setDir(&iox, IMD_STAT_FDBK, MCP_DIR_IN);
    setDir(&iox, INRT_STAT_FDBK, MCP_DIR_IN);
    setDir(&iox, MCU_STAT_FDBK, MCP_DIR_IN);
    setDir(&iox, MCU_HV_EN, MCP_DIR_OUT);
    setDir(&iox, BMS_MPI1, MCP_DIR_OUT);
    setDir(&iox, LIM_MCU3, MCP_DIR_IN);
    setDir(&iox, LIM_MCU2, MCP_DIR_IN);
    setDir(&iox, LIM_MCU1, MCP_DIR_IN);
    setDir(&iox, LIM_MCU0, MCP_DIR_IN);
    setDir(&iox, CHARGE_LV, MCP_DIR_OUT);

    return 0;
}

i2c_settings iox_rick::getiox_rickDev()
{
    return iox;
}

int iox_rick::setMCULatch(bool val)
{
#ifdef NOI2C
    return 1;
#endif
    return setState(&iox, MCU_LATCH, val);
}

int iox_rick::getMSTRSw()
{
#ifdef NOI2C
    return 1;
#endif
    return getState(&iox, MSTR_SW_FDBK);
}

int iox_rick::getEStop()
{
#ifdef NOI2C
    return 1;
#endif
    return getState(&iox, E_STOP_FDBK);
}

int iox_rick::getLMTSw()
{
#ifdef NOI2C
    return 1;
#endif
    return getState(&iox, LMT_SW_FDBK);
}

int iox_rick::getBMSStat()
{
#ifdef NOI2C
    return 1;
#endif
    return getState(&iox, BMS_STAT_FDBK);
}

int iox_rick::getIMDStat()
{
#ifdef NOI2C
    return 1;
#endif
    return getState(&iox, IMD_STAT_FDBK);
}

int iox_rick::getIRTStat()
{
#ifdef NOI2C
    return 1;
#endif
    return getState(&iox, INRT_STAT_FDBK);
}

int iox_rick::getMCUStat()
{
#ifdef NOI2C
    return 1;
#endif
    return getState(&iox, MCU_STAT_FDBK);
}

int iox_rick::setMCUHvEn(bool val)
{
#ifdef NOI2C
    return 1;
#endif
    return setState(&iox, MCU_HV_EN, val);
}

int iox_rick::setBMS_MPI1(bool val)
{
#ifdef NOI2C
    return 1;
#endif
    return setState(&iox, BMS_MPI1, val);
}

int iox_rick::getLIM_MCU3()
{
#ifdef NOI2C
    return 1;
#endif
    return getState(&iox, LIM_MCU3);
}

int iox_rick::getLIM_MCU2()
{
#ifdef NOI2C
    return 1;
#endif
    return getState(&iox, LIM_MCU2);
}

int iox_rick::getLIM_MCU1()
{
#ifdef NOI2C
    return 1;
#endif
    return getState(&iox, LIM_MCU1);
}

int iox_rick::getLIM_MCU0()
{
#ifdef NOI2C
    return 1;
#endif
    return getState(&iox, LIM_MCU0);
}

int iox_rick::ChargeLV(int val)
{
#ifdef NOI2C
    return 1;
#endif
    return setState(&iox, CHARGE_LV, val);
}