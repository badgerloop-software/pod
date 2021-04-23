#ifndef __CAN_H__
#define __CAN_H__

#include <linux/can.h>
#include <net/if.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#ifdef USE_VCAN
#define CAN_INTF "vcan0"
#else
#define CAN_INTF "can1"
#endif

extern volatile bool NEW_CAN_MESSAGE;

int initCan();

int canRead(struct can_frame* can_mesg);

int canSend(uint32_t id, uint8_t* data, uint8_t size);

/* Potential ideas for a future API */
// bool start_can_read();

// bool stop_can_read();

#endif
