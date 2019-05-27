// Includes
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "HVTelemetry_Loop.h"
#include "HVTCPSocket.h"
#include "HV_Telem_Recv.h"
#include "data_dump.h"
#include "motor.h"

extern "C" 
{
    #include "data.h"
    #include "can_devices.h"
    #include "state_machine.h"
}

data_t *data;

int init() {
	/* Init all drivers */
    SetupCANDevices();
    

    /* Init Data struct */
    initData();

    /* Allocate needed memory for state machine and create graph */
	buildStateMachine();

    /* Init telemetry */
    SetupHVTelemetry((char *) "192.168.1.126", 33333);
	SetupHVTCPServer();
	SetupHVTelemRecv();	
	
    /* Start 'black box' data saving */
    SetupDataDump();
	
    return 0;	
}

int main() {
	/* Create the big data structures to house pod data */
	
	if (init() == 1) {
		printf("Error in initialization! Exiting...\r\n");
		exit(1);
	}
    
    printf("Here\n");

	while(1) {
		runStateMachine();
        usleep(10000);

		// Control loop
	}
    return 0;
}
