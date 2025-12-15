#include <stdint.h>
#include <stdbool.h>
#include "launchpad.h"

void MotionInit(void)
{
    // Initialize PC4 as input
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_4);
}
