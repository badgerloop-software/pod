#include <stdint.h>
#include <stdio.h>

#define VOLTAGE_SENSOR CHANNEL_0
#define CURR_SENSOR CHANNEL_7

#define VOLT_SCALE(x) (((((double)x) / 255.0) * 5.0000 * 2.942))
#define CURR_SCALE(x) ((((((double)x) / 255.0) * 5.0000) - 2.5 / 2.5) * 50)
double getLVBattVoltage();
double getLVCurrent();
double getLVBattVoltage()
{
    uint8_t data[2] = { 0 };
    // readPressureSensor(ADC_0, VOLTAGE_SENSOR, data);
    return VOLT_SCALE((double)data[0]);
}

double getLVCurrent()
{
    uint8_t data[2] = { 0 };
    // readPressureSensor(ADC_0, CURR_SENSOR, data);

    return CURR_SCALE(data[0]);
}
