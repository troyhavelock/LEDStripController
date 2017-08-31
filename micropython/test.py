import machine
import time

def flash():
    pin = machine.Pin(5, machine.Pin.OUT)
    pin.on()
    print("Pin on")
    
    while True:
        time.sleep(1)
        pin.off()
        print("Pin off")
        time.sleep(1)
        pin.on()
        print("Pin on")
