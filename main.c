#include "mcc_generated_files/mcc.h"

// RHT related
#define TMPHUM_ADDR     0b1000000   // 7-bit i2c address for temp & humidity sensor
#define MEAS_TEMP       0xE3        // NO HOLD MASTER read temp command code
#define MEAS_RH         0xE5        // NO HOLD - read Relative Humidity command code

// light related
#define LIGHT_DATALOW   0xAC        // Data 1 lower regitster
//#define LIGHT_DATAHIGH  0xAD      // Data 1 uppper register
#define LIGHT_ADDR      0x39        // 7-bit i2c address for the ambient light sensor
#define LIGHT_CONTROL   0xA0        // set light to control register
#define LIGHT_TIMING    0xA1        // timing register

int16_t measureTemp();
int16_t measureRH();
void turnOnALS();
uint16_t readALS();
uint8_t readMoisture();

/*
                         Main application
 */
void main(void) {
    char cmd;
    uint16_t temperature;
    uint16_t humidity;
    uint16_t light;
    uint8_t moisture;

    // Initialize the device
    SYSTEM_Initialize();
    INTERRUPT_PeripheralInterruptEnable();
    INTERRUPT_GlobalInterruptEnable();

    printf("Development Board\r\n");
    printf("Plant Monitor Terminal\r\n");
    printf("Component Testing\r\n");
    printf("\r\n>");

    // activate ambient light sensor
    turnOnALS();



    for (;;) {
        if (EUSART2_DataReady) {
            cmd = EUSART2_Read(); // will not be used TODO remove

            printf("got cmd %u\r\n", cmd);

            while (!EUSART2_is_tx_ready());

            printf("gathering data\r\n");
            humidity = measureRH();
            light = readALS();
            moisture = readMoisture();
            temperature = measureTemp();
            
            printf("Writing values\r\n");

            printf("humidity: %x\r\n", humidity);
            EUSART2_Write(humidity);
            //EUSART2_Write(humidity >> 8);
            //EUSART2_Write(humidity & 0xFF);

            printf("light: %x\r\n", light);
            EUSART2_Write(light);
            //EUSART2_Write(light >> 8);
            //EUSART2_Write(light & 0xFF);

            printf("moisture: %x\r\n", moisture);
            EUSART2_Write(moisture);

            printf("temperature: %x\r\n", temperature);
            EUSART2_Write(temperature);
            //EUSART2_Write(temperature >> 8);
            //EUSART2_Write(temperature & 0xFF);
            
            EUSART2_Write('\n');

            while (!EUSART2_is_tx_done());
            printf("Wrote values\r\n\r\n");

            //temp = ((175.72 * RHT_value)/(65536)) - 46.85;
            //relative = ((125.00*RHT_value)/(65536)) - 6;
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
    while (I2C_Wflag == I2C2_MESSAGE_PENDING);


    //printf("MSB: %X, LSB: %X\r\n", data[0], data[1]);
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
    while (I2C_Wflag == I2C2_MESSAGE_PENDING);


    //printf("MSB: %X, LSB: %X\r\n", data[0], data[1]);
    // convert to a 16 bit value
    value = data[0];
    value = value << 8;
    value = value | data[1];

    return value;
}

void turnOnALS() {
    uint8_t data[1] = {LIGHT_CONTROL};

    I2C2_MESSAGE_STATUS I2C_Wflag = I2C2_MESSAGE_PENDING;

    // tell it we're writing to the control register
    I2C2_MasterWrite(data, 1, LIGHT_ADDR, &I2C_Wflag);
    while (I2C_Wflag == I2C2_MESSAGE_PENDING);

    // write to the control register
    data[0] = 0x03; //turn sensor on
    I2C_Wflag = I2C2_MESSAGE_PENDING;
    I2C2_MasterWrite(data, 1, LIGHT_ADDR, &I2C_Wflag);
    while (I2C_Wflag == I2C2_MESSAGE_PENDING);

    // tell it we're writing to the timing register
    data[0] = LIGHT_TIMING;
    I2C_Wflag = I2C2_MESSAGE_PENDING;
    I2C2_MasterWrite(data, 1, LIGHT_ADDR, &I2C_Wflag);
    while (I2C_Wflag == I2C2_MESSAGE_PENDING);

    // write to the timing register
    data[0] = 0x02;
    I2C_Wflag = I2C2_MESSAGE_PENDING;
    I2C2_MasterWrite(data, 1, LIGHT_ADDR, &I2C_Wflag);
    while (I2C_Wflag == I2C2_MESSAGE_PENDING);

    // verify the timing register
    I2C_Wflag = I2C2_MESSAGE_PENDING;
    I2C2_MasterRead(data, 1, LIGHT_ADDR, &I2C_Wflag);
    while (I2C_Wflag == I2C2_MESSAGE_PENDING);

    if (data[0] == 0x02) printf("Light sensor active\r\n");
    else printf("Light sensor did not turn on\r\n");
}

uint16_t readALS() {
    // tell the sensor to read from data1 lower
    uint8_t data[2] = {LIGHT_DATALOW, 0x00};
    uint16_t value;
    I2C2_MESSAGE_STATUS I2C_Wflag = I2C2_MESSAGE_PENDING;

    // tell it we're writing to the DATA1LOW register
    I2C2_MasterWrite(data, 1, LIGHT_ADDR, &I2C_Wflag);
    while (I2C_Wflag == I2C2_MESSAGE_PENDING);

    //read data
    I2C_Wflag = I2C2_MESSAGE_PENDING;
    I2C2_MasterRead(data, 2, LIGHT_ADDR, &I2C_Wflag);
    while (I2C_Wflag == I2C2_MESSAGE_PENDING);

    value = data[1];
    value = value << 8;
    value = value | data[0];

    return value;
}

uint8_t readMoisture() {
    ADCON0bits.GO_NOT_DONE = 1;
    return ADRESH;
}

void TMR0_DefaultInterruptHandler(void) {

}
/**
 End of File
 */
