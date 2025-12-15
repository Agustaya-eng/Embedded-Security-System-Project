        .cdecls "motion.h"
        .text

_GPIO_PORTC_BASE     .field  GPIO_PORTC_BASE

        .global MotionDetected
        .asmfunc
MotionDetected
        PUSH    {LR}
        LDR     r0, _GPIO_PORTC_BASE
        MOV     r1, #GPIO_PIN_4
        BL      GPIOPinRead
        POP     {PC}
        .endasmfunc
