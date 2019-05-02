#include "mcc_generated_files/mcc.h"

//Semi-working implementation of pi<->PIC communication
void main(void)
{
  SYSTEM_Initialize();
  /* INTERRUPT_GlobalInterruptEnable(); */
  /* INTERRUPT_PeripheralInterruptEnable(); */

  printf("\r\n\r\n\r\nFuck my asshole daddy\r\n");
  while (1)
    {
      if(EUSART1_DataReady) {
        while(!EUSART2_is_tx_ready());
        char cmd = EUSART1_Read();
        printf("Sending %c over EUSART2\r\n", cmd);
        EUSART2_Write(cmd);
        while(!EUSART2_is_tx_done());

        printf("Waiting for communication over EUSART2\r\n");
        while(!EUSART2_is_rx_ready());
        printf("Read %c\r\n", EUSART2_Read());
      }
    }
}
