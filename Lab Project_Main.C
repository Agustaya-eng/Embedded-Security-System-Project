#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "launchpad.h"
#include "buzzer.h"
#include "motion.h"
#include "seg7.h"

// The data structure for the system state
typedef struct {
    // Activated state: if the system is activated or not
    bool activated;

    // Alert state: If the alert is on or off
    bool alerted;

    // Sound state: If the buzzer is turned on to make a sound or not
    // Note: When the system is activated and alerted, the buzzer is turned
    // on and off periodically
    bool buzzer_on;
} SysState;

static SysState sys =
{
    false /* not activated */,
    false /* not alerted */,
    false /* buzzer not turned on */
};

Event buzzer_play_event;
Event check_push_button_event;
Event check_motion_event;

Seg7Display seg7 = { { 3, 2, 1, 0 }, false };  // initial state for seg7

bool error = false; // global error variable for seg7

void BuzzerPlay(Event *event)                   // the scheduled time
{
    uint32_t delay = 500;                       // default checking interval

    if (sys.alerted && sys.activated)
    {

        // Turn buzzer on and off
        if (sys.buzzer_on)
        {
            // The buzzer is on: turn it off
            BuzzerOff();
            sys.buzzer_on = false;
            delay = 2988;                       // off for 2988 ms
        }
        else
        {
            // The buzzer is off: turn it on
            BuzzerOn();
            sys.buzzer_on = true;
            delay = 12;                         // on for 12 ms
        }
    }
    else
    {
        // Make sure the buzzer is turned off
        if (sys.buzzer_on)
        {
            BuzzerOff();
            sys.buzzer_on = false;
        }
    }

    // schedule callback to buzzer play
    EventSchedule(event, event->time + delay);
}


void CheckMotion(Event *event)
{
    uint32_t delay = 500;   // check every 0.5s
    int motion = MotionDetected();

    if (sys.activated)
    {
        if (motion)
        {
            // Motion detected while activated -> alert
            sys.alerted = true;
            LedTurnOnOff(true /* red */, false /* blue */, false /* green */);
            uprintf("motion detected\n\r");
        }
        else
        {
            // No motion while activated -> not alerted
            sys.alerted = false;
            LedTurnOnOff(false /* red */, false /* blue */, true /* green */);
            uprintf("motion not detected\n\r");
        }
    }
    else
    {
        // System is deactivated -> LED off and no alert
        sys.alerted = false;
        LedTurnOnOff(false, false, false);
    }

    EventSchedule(event, event->time + delay);
}

void CheckPushButton(Event *event)                                  // invoked by pushbutton interrupt framework
{

    int code = PushButtonRead();                                    // read which button changed
    static int count[4];
    static int index = 0;

    if (index < 4) {
        switch(code)
        {
        case 1:
            count[index] = 0;
            index++;
            break;

        case 2:
            count[index] = 1;
            index++;
            break;

        default:
            break;
        }
    }

    if (index == 4) {

        if (count[0] == 1 && count[1] == 0 && count[2] == 1 && count[3] == 1) {
            if (sys.activated == true) {                                          // system is on and we entered code to turn it off
                sys.activated = false;
                sys.alerted   = false;
                LedTurnOnOff(false, false, false);                               //    turn LEDs off
                index = 0;
            }
            else {                                                               // system is off and we entered code to turn it on
                sys.activated = true;
                sys.alerted = false;
                LedTurnOnOff(false, false, true);                                // turn LEDs green
                index = 0;
            }

    }
        else {                                                                   // display E for error on seg7 display
            error = true;
                }
    }
}

void updateSeg7(void) {
    seg7.digit[3] = 13;
    seg7.digit[2] = 13;
    seg7.digit[1] = 13;
    if (sys.activated) {
        //locked
        seg7.digit[0] = 11;  // map digit 11 to A
    }

    else if (error) {
        seg7.digit[0] = 10; // map digit 10 to E
    }
    else {
        //unlocked
        seg7.digit[0] = 12;  // map digit 12 to U
    }
}

int main(void)
{
    // Initialize the Tiva C LaunchPad and 7-seg
    LaunchPadInit();
    BuzzerInit();
    MotionInit();
    Seg7Init();

    // Start deactivated, LED off
    LedTurnOnOff(false, false, false);


    // Initialize the event objects
    EventInit(&buzzer_play_event, BuzzerPlay);
    EventInit(&check_motion_event, CheckMotion);
    EventInit(&check_push_button_event, CheckPushButton);

    // Register ISR event for buttons
    PushButtonEventRegister(&check_push_button_event);

    // Schedule time events
    EventSchedule(&buzzer_play_event, 100);
    EventSchedule(&check_motion_event, 100);

    // Run the callback scheduler
    while (true)
    {
        updateSeg7();
        Seg7Update(&seg7);
        // Wait for interrupt events (include callback events)
        asm("   wfi");

        // Run callback events
        EventExecute();
    }
}
