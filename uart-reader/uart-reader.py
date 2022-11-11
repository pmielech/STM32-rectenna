import serial
# https://pyserial.readthedocs.io/en/latest/shortintro.html
from serial.tools import list_ports
from datetime import datetime
import sys
import matplotlib.pyplot as plt
import csv

PORT_TYPE = "usbmodem"  # com port
BAUD_RATE = ''
watch_counter = 0
num_counter = 0

randNumbers = []


def rand_hist_plot():
    global num_counter
    if num_counter % 20 == 0:
        plt.hist(randNumbers, align='left')     # edgecolor='black'
        plt.xlabel('Wartości liczb')
        plt.ylabel('Częstotliwość występowania')
        plt.grid(True)
        if num_counter % 60 == 0:
            save_data()
        plt.show()

    else:
        return


def add_random(uart_read):
    randNumbers.append(int(uart_read[2:]))


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
    global num_counter
    build_s = ""
    read_c = ""
    while read_c != '\r':
        read_c = stm_device.read().decode()
        build_s += str(read_c)

    if build_s[0] == 'R':
        add_random(build_s)
        num_counter += 1

    return build_s


def save_data():
    test_time = datetime.now().strftime("%y_%m_%d")
    csv_file = open('tests/' + test_time + ".csv", 'a')
    csv.writer(csv_file).writerow(randNumbers)
    csv_file.close()
    plt.savefig('tests/' + test_time + '.pdf')


# TODO: INIT
stm_device = find_device()

# TODO: MAIN
while True:
    print(read_line_uart())
    rand_hist_plot()
