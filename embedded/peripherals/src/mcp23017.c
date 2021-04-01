#include "mcp23017.h"
#include "i2c.h"
#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int setupMCP(i2c_settings* i2c, char mcpAddress)
{
    uint8_t flush[1];
    i2c->bus = 2;
    i2c->deviceAddress = mcpAddress;
    i2c->openMode = O_RDWR;

    if (i2c_begin(i2c) == -1) {
        fprintf(stderr, "Could not open i2c bus.\n");
        return -1;
    }

    /* check if there actually is a device there */
    if (read_i2c(i2c, flush, 1) != 0) {
        return -1;
    }

    return 0;
}

/* Sets the pins to default state of inputs, at 0 */
int clearSettingsMCP(i2c_settings* i2c)
{
    if (write_data_i2c(i2c, IODIRA, 0xFF) != 0)
        return -1;
    if (write_data_i2c(i2c, IODIRB, 0xFF) != 0)
        return -1;
    if (write_data_i2c(i2c, GPIOA, 0x00) != 0)
        return -1;
    if (write_data_i2c(i2c, GPIOB, 0x00) != 0)
        return -1;
    return 0;
}

// helper method for getting data from a specific address
static uint8_t getFromAddress(i2c_settings* i2c, char address)
{
    write_byte_i2c(i2c, address);

    uint8_t dataBuffer[1];
    read_i2c(i2c, dataBuffer, 1);

    return dataBuffer[0]; // getting the digit corresponding to the pin
}
/* TODO temp function, going to replace with a faster solution by using a 0x<Bank><pin> encoding later */
static uint8_t makeRelativeToBank(uint8_t pin)
{
    /* If its in bank B, we need to shif the numbering convention */
    if (pin >= (NUM_PINS >> 1)) {
        pin -= (NUM_PINS >> 1);
    }
    return pin;
}

static int getAllStates(i2c_settings* i2c, uint8_t address)
{
    return (getFromAddress(i2c, address));
}

static int validatePin(uint8_t pin)
{
    /* Check that we are getting a valid pin */
    if (pin >= NUM_PINS) {
        return -1;
    }
    return 0;
}

static int writeToDev(i2c_settings* i2c, char addr, uint8_t pin, bool val)
{
    int ret;
    pin = makeRelativeToBank(pin);
    if (val) {
        ret = write_data_i2c(i2c, addr, getAllStates(i2c, addr) | (1 << pin));
    } else {
        ret = write_data_i2c(i2c, addr, getAllStates(i2c, addr) & ~(1 << pin));
    }
    return ret;
}

static char getIodirBank(uint8_t pin)
{
    if (pin < (NUM_PINS >> 1)) {
        return IODIRA;
    } else {
        return IODIRB;
    }
}

static char getGpioBank(uint8_t pin)
{
    if (pin < (NUM_PINS >> 1)) {
        return GPIOA;
    } else {
        return GPIOB;
    }
}

int getState(i2c_settings* i2c, uint8_t pin)
{
    if (validatePin(pin) == -1) {
        fprintf(stderr, "Invalid pin input getState\n");
        return -1;
    }
    char addr = getGpioBank(pin);
    pin = makeRelativeToBank(pin);
    return (getFromAddress(i2c, addr) >> pin) & 1;
}

int setState(i2c_settings* i2c, uint8_t pin, bool val)
{
    if (validatePin(pin) == -1) {
        fprintf(stderr, "setState: Invalid pin number\n");
        return -1;
    }
    if (getDir(i2c, pin) != 0) {
        fprintf(stderr, "setState: Only can set value on an output\n");
        return -1;
    }
    uint8_t address = getGpioBank(pin);
    int currentState = getState(i2c, pin);
    if (val == currentState) {
        return 0;
    }
    return writeToDev(i2c, address, pin, val);
}

int getDir(i2c_settings* i2c, uint8_t pin)
{
    if (validatePin(pin) == -1) {
        fprintf(stderr, "Invalid pin\n");
        return -1;
    }
    char addr = getIodirBank(pin);
    pin = makeRelativeToBank(pin);
    return (getFromAddress(i2c, addr) >> pin) & 1;
}

int setDir(i2c_settings* i2c, uint8_t pin, bool val)
{
    if (validatePin(pin) == -1) {
        fprintf(stderr, "Invalid pin");
    }
    char address = getIodirBank(pin);
    int currentState = getDir(i2c, pin);
    if (val == currentState) {
        return 0;
    }
    return writeToDev(i2c, address, pin, val);
}
