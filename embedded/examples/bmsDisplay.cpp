#include <TelemetryLoop.h>
#include <connStat.h>
#include <pthread.h>
#include <stdio.h>
extern "C" {
#include "NCD9830DBR2G.h"
#include "bms.h"
#include "can_devices.h"
#include "data.h"
#include "imu.h"
extern pthread_t CANThread;
}

int main()
{
    initData();
    initPressureSensors();
    SetupCANDevices();
    SetupTelemetry((char*)DASHBOARD_IP, DASHBOARD_PORT);
    pthread_join(CANThread, NULL);
}
