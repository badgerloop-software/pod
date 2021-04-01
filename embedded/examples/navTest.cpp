extern "C" {
#include <data.h>
#include <imu.h>
#include <nav.h>
#include <retro.h>
#include <stdio.h>
#include <string.h>
}
#include "connStat.h"
#include <TelemetryLoop.h>
#define ROLL "roll"
#define EXP "exp"
#define RAW "raw"

extern void initNav();

int main(int argc, char* argv[])
{
    initData();
    /*  SetupIMU();*/
    initRetros();
    initNav();

    SetupTelemetry((char*)DASHBOARD_IP, 33333);
    joinRetroThreads();

    while (1)
        ;
}
