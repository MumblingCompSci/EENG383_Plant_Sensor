#include "mcc_generated_files/mcc.h"

#define TMPHUM_ADDR 0b1000000   // 7-bit i2c address for temp & humidity sensor
#define MEAS_TEMP   0xF3        // NO HOLD MASTER read temp command code
#define MEAS_RH     0xF5        // NO HOLD - read Relative Humidity command code
#define WRITE_RHT   0xE6        // Write to register
#define READ_RHT    0xE7        // Read from user register

void printHelp();
uint16_t measureTemp();
//uint16_t readTemp();

uint16_t RHT_value = 0x00;


/*
                         Main application
 */
void main(void)
{
    char cmd;
    
    // Initialize the device
    SYSTEM_Initialize();
    INTERRUPT_PeripheralInterruptEnable();
    INTERRUPT_GlobalInterruptEnable();
    
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
                    printf("Collecting Temperature Value...\r\n");
                    RHT_value = measureTemp();
                    
                    printf("Read %x from Temp Sensor\r\n", RHT_value);
                    break;
                /** READ Relative Humidity */
                default:
                    printf("Unknown Key %c\r\n", cmd);
                    break;
            }
        }
    }
}

uint16_t measureTemp() {
        uint16_t value;
    uint8_t data[2] = {MEAS_TEMP};
    I2C1_MESSAGE_STATUS I2C_Wflag;

    I2C_Wflag = I2C1_MESSAGE_PENDING;
    I2C1_MasterWrite(data, 1, TMPHUM_ADDR, &I2C_Wflag);
//    while(I2C_Wflag == I2C1_MESSAGE_PENDING);

    
//    I2C1_MESSAGE_STATUS I2C_Wflag = I2C1_MESSAGE_PENDING;
    do {
        printf("Starting read on %x\r\n", TMPHUM_ADDR);
        I2C1_MasterRead(data, 2, TMPHUM_ADDR, &I2C_Wflag);
        printf("Current i2c flag : %x", I2C_Wflag);
    } while(I2C_Wflag == I2C1_MESSAGE_PENDING);
    
    value = data[0];
    value = value << 8;
    value = value + data[1];
    
    return value;
}

void printHelp() {
    printf("\r\n--------------------\r\n");
    printf("--------------------\r\n");
    printf("o: k\r\n");
    printf("Z: reset processor\r\n");
    printf("z: clear terminal\r\n");
    printf("r: get relative humidity\r\n");
    printf("t: get temperature\r\n");
    printf("--------------------\r\n");
}

void TMR0_DefaultInterruptHandler(void) {
    
}
/**
 End of File
*/