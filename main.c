#include "mcc_generated_files/mcc.h"

#define TMPHUM_ADDR 0b1000000   // 7-bit i2c address for temp & humidity sensor
#define MEAS_TEMP   0xF3        // NO HOLD MASTER read temp command code
#define MEAS_RH     0xF5        // NO HOLD - read Relative Humidity command code
#define WRITE_RHT   0xE6        // Write to register
#define READ_RHT    0xE7        // Read from user register

void printHelp();

void measureRHT();
void readRHT(uint16_t &rht_value);

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
        if (EUSART1_DataReady()) {
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
                    //TODO: measure temp
                    //TODO: read temp
                    
                    
                    break;
                /** READ Relative Humidity */
                default:
                    printf("Unknown Key %c\r\n", cmd);
                    break;
            }
        }
    }
}

void measureRHT() {
    
}

void readRHT(uint16_t &rht_value) {
    
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