import serial
# https://pyserial.readthedocs.io/en/latest/shortintro.html
from serial.tools import list_ports
from datetime import datetime
import sys
import matplotlib.pyplot as plt
import csv

PORT_TYPE = "usbmodem"  # or com port, depends on os
BAUD_RATE = ''
watch_counter = 0
num_counter = 0
nums_saved = set([])
to_save = []
randNumbers = []


def rand_hist_plot():
    global num_counter
    if num_counter % 20 == 0:
        plt.hist(randNumbers, align='left', bins=10, edgecolor='black', linewidth=1.2)
        plt.xlabel('Wartości liczb')
        plt.ylabel('Częstotliwość występowania')
        plt.grid(True)
        if num_counter % 60 == 0:
            save_data()
        plt.show()

    else:
        return


def add_random(uart_read):
    randNumbers.append(uart_read)


def error_handler(err):
    if err == AttributeError:
        print("Can't find any COM device")
        sys.exit()

    elif KeyboardInterrupt:
        print("Exiting...")
        sys.exit()

    else:
        print("Unknown error occurred")


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
    global num_counter
    build_s = ""
    read_c = ""
    try:
        while read_c != '\r':
            read_c = stm_device.read().decode()
            build_s += str(read_c)
    except:
        print("Failed to read the uart")

    try:
        build_s = build_s[2:].rsplit('\n\r')
        build_s = int(build_s[0])
    except:
        print("Failed to convert the data")
        return
    add_random(build_s)
    num_counter += 1
    return build_s


def save_data():
    global to_save
    global nums_saved
    to_save = set(randNumbers)
    try:
        to_save = to_save.difference(nums_saved)
    except:
        pass
    test_time = datetime.now().strftime("%y_%m_%d")
    with open('tests/' + test_time + ".csv", 'a') as csv_file:
        csv.writer(csv_file).writerows([to_save])

    plt.savefig('tests/' + test_time + '.pdf')
    nums_saved.update(to_save)


# TODO: INIT
stm_device = find_device()

# TODO: MAIN
while True:
    print(read_line_uart())
    rand_hist_plot()
