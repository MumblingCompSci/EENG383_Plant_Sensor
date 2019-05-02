#include "mcc_generated_files/mcc.h"

//Semi-working implementation of pi<->PIC communication
void main(void)
{
  SYSTEM_Initialize();
  /* INTERRUPT_GlobalInterruptEnable(); */
  /* INTERRUPT_PeripheralInterruptEnable(); */

  printf("\r\n\r\n\r\nStart PIC\r\n");
  while (1)
    {
      if(EUSART2_DataReady) {
          while(!EUSART1_is_tx_ready());
          char cmd = EUSART2_Read();
          printf("Got %c\r\n", cmd);
          EUSART2_Write(cmd);
      }
    }
}
