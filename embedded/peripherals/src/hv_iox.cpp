#include <hv_iox.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

extern "C" {
#include <data.h>
#include <i2c.h>
#include <mcp23017.h>
#include <proc_iox.h>
}

#define HV_IO_ADDR 0x24

#ifdef NOI2C
#define VI2C
#endif

static i2c_settings iox;
static int setupIox(void);

/***
 *  initHVIox - Meant to be overly pedantic
 *      even if the values arent changing,
 *      just to make extremely clear the initial
 *      state of everything.
 *
 */
HVIox::HVIox()
{
}

int HVIox::init(bool hardStart)
{
    if (setupMCP(&iox, HV_IO_ADDR) != 0)
        return -1;
    else if (hardStart) {
        if (clearSettingsMCP(&iox) != 0)
            return -1;
        else if (this->setupIox() != 0)
            return -1;
    }
    return 0;
}

int HVIox::setupIox()
{
    // static int setupIox() moved into constructor:
    setDir(&iox, HV_IND_EN, MCP_DIR_IN);
    setDir(&iox, MCU_LATCH, MCP_DIR_OUT);
    setDir(&iox, BMS_MULTI_IN, MCP_DIR_IN);
    setDir(&iox, IMD_STAT_FDBK, MCP_DIR_IN);
    setDir(&iox, INRT_STAT_FDBK, MCP_DIR_IN);
    setDir(&iox, HV_EN_FDBK, MCP_DIR_IN);
    setDir(&iox, MCU_HV_EN, MCP_DIR_IN);
    setDir(&iox, PS_FDBK, MCP_DIR_IN);
    setDir(&iox, BMS_STAT_FDBK, MCP_DIR_IN);
    setDir(&iox, E_STOP_FDBK, MCP_DIR_IN);
    setDir(&iox, MSTR_SW_FDBK, MCP_DIR_IN);

    return 0;
}

i2c_settings HVIox::getHVIoxDev()
{
    return iox;
}

int HVIox::isHVIndicatorEnabled()
{
#ifdef NOI2C
    return 1;
#endif
    return getState(&iox, HV_IND_EN);
}

int HVIox::setMCULatch(bool val)
{
#ifdef NOI2C
    return 0;
#endif
    return setState(&iox, MCU_LATCH, val);
}

int HVIox::getBMSMultiIn()
{
#ifdef NOI2C
    return 1;
#endif
    return getState(&iox, BMS_MULTI_IN);
}

int HVIox::getIMDStatus()
{
#ifdef NOI2C
    return 1;
#endif
    return getBmsImdStatus() >= 4; //getState(&iox, IMD_STAT_FDBK);
}

int HVIox::getINRTStatus()
{
#ifdef NOI2C
    return 1;
#endif
    return getState(&iox, INRT_STAT_FDBK);
}

int HVIox::isHVEnabled()
{
#ifdef NOI2C
    return 0;
#endif
    return getState(&iox, HV_EN_FDBK);
}

int HVIox::setMCUHVEnabled(int val)
{
#ifdef NOI2C
    return 0;
#endif
    setDir(&iox, MCU_HV_EN, MCP_DIR_OUT);
    return setState(&iox, MCU_HV_EN, val);
}

int HVIox::isMCUHVEnabled()
{
#ifdef NOI2C
    return 1;
#endif
    setDir(&iox, MCU_HV_EN, MCP_DIR_IN);
    return getState(&iox, MCU_HV_EN);
}

int HVIox::emergencyDisableMCU()
{
#ifdef NOI2C
    return 1;
#endif
    setDir(&iox, MCU_HV_EN, MCP_DIR_OUT);
    return setState(&iox, MCU_HV_EN, true);
}

int HVIox::getPsStatus()
{
#ifdef NOI2C
    return 1;
#endif
    return getState(&iox, PS_FDBK);
}

int HVIox::getBMSStatus()
{
#ifdef NOI2C
    return 1;
#endif
    return getState(&iox, BMS_STAT_FDBK);
}

int HVIox::isEStopOn()
{
#ifdef NOI2C
    return 0;
#endif
    return getState(&iox, E_STOP_FDBK);
}

int HVIox::getMasterSwFeedback()
{
#ifdef NOI2C
    return 1;
#endif
    return getState(&iox, MSTR_SW_FDBK);
}
