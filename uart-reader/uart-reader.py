import time

import serial
# https://pyserial.readthedocs.io/en/latest/shortintro.html
from serial.tools import list_ports
from datetime import datetime
import sys
import matplotlib.pyplot as plt
import csv

global stm_device
connection_lost = 0
connected = False
PORT_TYPE = "usbmodem"  # or com port, depends on os
BAUD_RATE = ''
watch_counter = 0
num_counter = 0
nums_saved = set([])
to_save = []
randNumbers = []
message_type = ""


def gen_hist_plot():
    global num_counter
    if num_counter % 20 == 0:
        plt.hist(randNumbers, align='left', bins=10, edgecolor='black', linewidth=1.2)
        plt.xlabel('Wartości wygenerowanych liczb')
        plt.ylabel('Częstotliwość występowania')
        plt.grid(True)
        if num_counter % 60 == 0:
            save_data()
        plt.show()

    else:
        return


def add_number(uart_read):
    randNumbers.append(uart_read)


def custom_errors(err):
    if err == AttributeError:
        print("Can't find any COM device")

    elif KeyboardInterrupt:
        print("Exiting...")
        sys.exit()

    else:
        print("Unknown error occurred")


def find_device():
    global connection_lost
    global connected
    device = None
    port_list = list(list_ports.comports())
    try:
        for x in port_list:
            if PORT_TYPE in str(x):
                string_edit = str(x).lower()[:str(x).index(' - ')]
                device = serial.Serial(string_edit, 38400, timeout=2)
    except Exception as e:
        print("Failed to connect to serial port, due to: ")
        print(e)
        sys.exit()
    if device is not None:
        connected = True
        return device

    else:
        custom_errors(AttributeError)


def read_line_uart():
    global message_type
    global connection_lost
    global num_counter
    build_s = ""
    read_c = ""
    try:
        while read_c != '\r':
            read_c = stm_device.read().decode()
            build_s += str(read_c)
    except Exception as e:
        print("Failed to read the uart, due to: ")
        print(e)
        connection_lost += 1
        if connection_lost >= 5:
            print("Connection lost. Trying to reconnect to the device...")
            connect_to_device()

    try:
        message_type = build_s[2:]
        build_s = build_s[2:].rsplit('\n\r')
        build_s = int(build_s[0])
    except Exception as e:
        print("Failed to convert the data, due to: ")
        print(e)
        return

    add_number(build_s)
    num_counter += 1

    return build_s


def save_data():
    global to_save
    global nums_saved
    to_save = set(randNumbers)
    try:
        to_save = to_save.difference(nums_saved)
    except Exception as e:
        print("Failed to add to array, due to: ")
        print(e)
        print("Skipping value")
        pass
    test_time = datetime.now().strftime("%y_%m_%d")
    with open('tests/' + test_time + ".csv", 'a') as csv_file:
        csv.writer(csv_file).writerows([to_save])

    plt.savefig('tests/' + test_time + '.pdf')
    nums_saved.update(to_save)


def connect_to_device():
    global stm_device
    global connection_lost
    while not connected:
        stm_device = find_device()
        time.sleep(1)
    connection_lost = 0


def program_state():

    pass

connect_to_device()

while True:
    read_line_uart()
    gen_hist_plot()
