import serial
# https://pyserial.readthedocs.io/en/latest/shortintro.html
from serial.tools import list_ports
import time
import sys

PORT_TYPE = "usbmodem"  # com port
BAUD_RATE = ''
watch_counter = 0


def error_handler(err):
    if err == AttributeError:
        print("Can't find any COM device")
        sys.exit()

    elif "infinite_loop":
        print("infinite loop error")
        sys.exit()
        # TODO: rerunning code

    elif KeyboardInterrupt:
        print("Exiting...")
        sys.exit()

    else:
        print("Unknown error occurred")


def watchdog(cnt):
    cnt += 1
    if cnt <= 10:
        error_handler("infinite_loop")

    else:
        return cnt


def find_device():
    device = None
    port_list = list(list_ports.comports())
    for x in port_list:
        if PORT_TYPE in str(x):
            string_edit = str(x).lower()[:str(x).index(' - ')]
            device = serial.Serial(string_edit, 38400, timeout=2)

    if device is not None:
        return device

    else:
        error_handler(AttributeError)


def read_line_uart():
    build_s = ""
    read_c = ""
    while read_c != '\r':
        read_c = stm_device.read().decode()
        build_s += str(read_c)
        # watch_counter = watchdog(watch_counter)
    return build_s


# TODO: INIT
stm_device = find_device()

# TODO: MAIN
while True:
    try:
        print(read_line_uart())
    except KeyboardInterrupt:
        error_handler(KeyboardInterrupt)

