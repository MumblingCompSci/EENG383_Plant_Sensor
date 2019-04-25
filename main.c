#include "mcc_generated_files/mcc.h"

// RHT related
#define TMPHUM_ADDR     0b1000000   // 7-bit i2c address for temp & humidity sensor
#define MEAS_TEMP       0xE3        // NO HOLD MASTER read temp command code
#define MEAS_RH         0xE5        // NO HOLD - read Relative Humidity command code

// light related
#define LIGHT_DATALOW   0xE         // Data 1 lower regitster
#define LIGHT_DATAHIGH  0xF         // Data 1 uppper register
#define LIGHT_ADDR      0b0101001   // 7-bit i2c address for the ambient light sensor
#define LIGHT_CONTROL   0xA0        // set light to control register

void printHelp();
int16_t measureTemp();
int16_t measureRH();
void turnOnALS();


/*
                         Main application
 */
void main(void)
{
    uint16_t RHT_value = 0x00;
    char cmd;
    int16_t temp;
    int16_t relative;
    int16_t lightLevel;
    // Initialize the device
    SYSTEM_Initialize();
    INTERRUPT_PeripheralInterruptEnable();
    INTERRUPT_GlobalInterruptEnable();
    
    // activate ambient light sensor
    turnOnALS();
    
    
    printf("Development Board\r\n");
    printf("Plant Monitor Terminal\r\n");
    printf("Component Testing\r\n");
    printf("\r\n>");

    for (;;) {
        if (EUSART1_DataReady) {
            cmd = EUSART1_Read();
            
            switch(cmd) {
                case '?' :
                    printHelp();
                    break;
                case 'z' :
                    for (int i = 0; i < 40; i++)
                        printf("\r\n");
                    break;
                case 'Z' :
                    for (int i = 0; i < 40; i++)
                        printf("\r\n");
                    RESET();
                    break;
                case 'o' :
                    printf("o:    ok\r\n");
                    break;
                    
                    
                /** READ TEMP */
                case 't' :
                    printf("\r\nCollecting Temperature value...\r\n");
                    RHT_value = measureTemp();
                    
                    printf("Read %X from Temp Sensor\r\n", RHT_value);
                    
                    temp = ((175.72 * RHT_value)/(65536)) - 46.85;
                    
                    printf("Estimated temperature is: %i\r\n", temp);
                    
                    break;
                /** READ Relative Humidity */
                case 'r' :
                    printf("\r\nCollecting Relative Humidity value...\r\n");
                    RHT_value = measureRH();
                    
                    relative = ((125.00*RHT_value)/(65536)) - 6;
                    printf("Estimated Relative Humidity: %i\r\n", relative);
                    
                    break;
                    
                /** Read Ambient Light */
                case 'L' :
                    printf("\r\nCollecting Ambient Light Level value...\r\n");
                    
                default:
                    printf("Unknown Key %c\r\n", cmd);
                    break;
            }
        }
    }
}

int16_t measureTemp() {
    int16_t value;
    uint8_t data[4] = {MEAS_TEMP, MEAS_TEMP, 0, 0};
    I2C2_MESSAGE_STATUS I2C_Wflag;

    //tell the sensor to measure
    I2C_Wflag = I2C2_MESSAGE_PENDING;
    I2C2_MasterWrite(data, 1, TMPHUM_ADDR, &I2C_Wflag);
    while (I2C_Wflag == I2C2_MESSAGE_PENDING);
    
    
    // read from the sensor
    I2C_Wflag = I2C2_MESSAGE_PENDING;
    I2C2_MasterRead(data, 4, TMPHUM_ADDR, &I2C_Wflag);
    while(I2C_Wflag == I2C2_MESSAGE_PENDING);
    
    
    printf("MSB: %X, LSB: %X\r\n", data[0], data[1]);
    // convert to a 16 bit value
    value = data[0];
    value = value << 8;
    value = value | data[1];
    
    return value;
}

int16_t measureRH() {
    int16_t value;
    uint8_t data[4] = {MEAS_RH, MEAS_RH, 0, 0};
    I2C2_MESSAGE_STATUS I2C_Wflag;

    //tell the sensor to measure
    I2C_Wflag = I2C2_MESSAGE_PENDING;
    I2C2_MasterWrite(data, 1, TMPHUM_ADDR, &I2C_Wflag);
    while (I2C_Wflag == I2C2_MESSAGE_PENDING);
    
    
    // read from the sensor
    I2C_Wflag = I2C2_MESSAGE_PENDING;
    I2C2_MasterRead(data, 4, TMPHUM_ADDR, &I2C_Wflag);
    while(I2C_Wflag == I2C2_MESSAGE_PENDING);
    
    
    printf("MSB: %X, LSB: %X\r\n", data[0], data[1]);
    // convert to a 16 bit value
    value = data[0];
    value = value << 8;
    value = value | data[1];
    
    return value;
}

void turnOnALS() {
    uint8_t data[1] = { LIGHT_CONTROL };
    
    I2C2_MESSAGE_STATUS I2C_Wflag = I2C2_MESSAGE_PENDING;
    
    // tell it we're writing to the control register
    I2C2_MasterWrite(data, 1, LIGHT_ADDR, &I2C_Wflag);
    while(I2C_Wflag == I2C2_MESSAGE_PENDING);
    
    // write to the control register
    data[0] = 0x03; //turn sensor on
    I2C_Wflag = I2C2_MESSAGE_PENDING;
    I2C2_MasterWrite(data, 1, LIGHT_ADDR, &I2C_Wflag);
    while (I2C_Wflag == I2C2_MESSAGE_PENDING);
}

void printHelp() {
    printf("\r\n--------------------\r\n");
    printf("--------------------\r\n");
    printf("o: k\r\n");
    printf("Z: reset processor\r\n");
    printf("z: clear terminal\r\n");
    printf("r: get relative humidity\r\n");
    printf("t: get temperature\r\n");
    printf("L: get ambient light level\r\n");
    printf("--------------------\r\n");
}

void TMR0_DefaultInterruptHandler(void) {
    
}
/**
 End of File
*/