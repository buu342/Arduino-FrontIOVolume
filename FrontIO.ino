#include <HID-Project.h>

#define D9   9
#define D10  10
#define D11  11
#define PINCOUNT  (sizeof(global_pins)/sizeof(uint32_t))

#define DEBOUNCETIME 0.05
#define DELAYTIME    0.6
#define RATE         0.04

#define BUTTON_UP    D9
#define BUTTON_DOWN  D10

typedef uint32_t pin_t;

/*=============================
            Globals
=============================*/

unsigned long global_curtime = 0;

pin_t         global_pins[] = {D9, D10, D11, A0, A1, A2, LED_BUILTIN};
int           global_pinmode[PINCOUNT] = {0};
int           global_pincurstate[PINCOUNT] = {0};
int           global_pinlaststate[PINCOUNT] = {0};
unsigned long global_lastdebouncetime[PINCOUNT] = {0};
unsigned long global_delaytime[PINCOUNT] = {0};

unsigned long global_ledtime = 0;


/*=============================
        Helper Functions
=============================*/

void pin_initall()
{
    // Initialize all pins as output zero volts
    for (int i=0; i<PINCOUNT; i++)
    {
        pinMode(global_pins[i], OUTPUT);
        global_pinmode[i] = OUTPUT;
        digitalWrite(global_pins[i], LOW);
    }
}

int pin_index(pin_t pin)
{
    for (int i=0; i<PINCOUNT; i++)
        if (global_pins[i] == pin)
            return i;
    return -1;
}

void pin_setmode(pin_t pin, uint32_t mode)
{
    // Initialize all pins as output zero volts
    pinMode(pin, mode);
    global_pinmode[pin_index(pin)] = mode;
}

bool button_check(pin_t pin)
{
    int index = pin_index(pin);
    if (global_pinmode[index] == INPUT_PULLUP)
        return !global_pincurstate[index];
    else
        return global_pincurstate[index];
}

/*bool button_pressed(pin_t pin)
{
    int index = pin_index(pin);
    return global_pinlaststate[index] != global_pincurstate[index] && button_check(pin);
}

bool button_released(pin_t pin)
{
    int index = pin_index(pin);
    return global_pinlaststate[index] != global_pincurstate[index] && !button_check(pin);
}*/

void led_holdfor(float seconds)
{
    global_ledtime = global_curtime + seconds*1000;
    if (seconds == -1)
        global_ledtime = -1;
}

void loop_start()
{
    global_curtime = millis();
    for (int i=0; i<PINCOUNT; i++)
    {
        int read = digitalRead(global_pins[i]);
        if (read != global_pinlaststate[i])
            global_lastdebouncetime[i] = global_curtime;
        if ((global_curtime - global_lastdebouncetime[i]) > DEBOUNCETIME*1000)
            global_pincurstate[i] = read;
    }
}

void loop_end()
{
    // Handle last input states
    for (int i=0; i<PINCOUNT; i++)
        global_pinlaststate[i] = digitalRead(global_pins[i]);

    // Handle LED state
    if (global_ledtime == -1 || global_ledtime >= global_curtime)
        digitalWrite(LED_BUILTIN, HIGH);
    else
        digitalWrite(LED_BUILTIN, LOW);
}

void input_key(pin_t pin, ConsumerKeycode key)
{
    if (global_delaytime[pin_index(pin)] < global_curtime)
    {
      if (global_delaytime[pin_index(pin)] == 0)
          global_delaytime[pin_index(pin)] = global_curtime + DELAYTIME*1000;
      else
          global_delaytime[pin_index(pin)] = global_curtime + RATE*1000;
      Consumer.write(key);
      led_holdfor(0.1f);
    }
}


/*=============================
      Program Entrypoint
=============================*/

void setup()
{
    pin_initall();
    Serial.begin(9600);

    Consumer.begin();
    
    pin_setmode(BUTTON_UP, INPUT_PULLUP);
    pin_setmode(BUTTON_DOWN, INPUT_PULLUP);
}

void loop()
{
    loop_start();
    if (button_check(BUTTON_UP))
        input_key(BUTTON_UP, MEDIA_VOL_UP);
    else
        global_delaytime[pin_index(BUTTON_UP)] = 0;
    if (button_check(BUTTON_DOWN))
        input_key(BUTTON_DOWN, MEDIA_VOL_DOWN);
    else
        global_delaytime[pin_index(BUTTON_DOWN)] = 0;
    loop_end();
}
