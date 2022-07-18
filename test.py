from machine import Pin
from time import sleep_ms

## definition of IOpins
# status LED
greenLED_pin = Pin(8, Pin.OUT)
yellowLED_pin = Pin(9, Pin.OUT)
redLED_pin = Pin(10, Pin.OUT)

# encoder pins
encA_pin = Pin(11, Pin.IN, Pin.PULL_UP)
encB_pin = Pin(12, Pin.IN, Pin.PULL_UP)

# relays pins
relG_pin = Pin(3, Pin.OUT)
relR_pin = Pin(4, Pin.OUT)

## setting initial values of pins
# status LED
greenLED_pin.value(0)
yellowLED_pin.value(0)
redLED_pin.value(0)

# relays pins
relG_pin.value(0)
relR_pin.value(0)

# interrupt handler
def alert(pin):
    # miejsce na stary kod


# attaching the intterupt
encA_pin.irq(trigger = Pin.IRQ_RISING,
             handler = alert)


# MAIN loop
while True:
    greenLED_pin.toggle()
    sleep_ms(100)
    greenLED_pin.toggle()
    sleep_ms(100)
    yellowLED_pin.toggle()
    sleep_ms(100)
    yellowLED_pin.toggle()
    sleep_ms(100)
    redLED_pin.toggle()
    sleep_ms(100)
    redLED_pin.toggle()
    sleep_ms(100)


