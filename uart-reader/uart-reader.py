import shlex
import time
import serial
# https://pyserial.readthedocs.io/en/latest/shortintro.html
from serial.tools import list_ports
from datetime import datetime
import sys
import matplotlib.pyplot as plt
import csv
import os

global stm_device
connection_lost = 0
connected = False
PORT_TYPE = "usbmodem"  # or com port, depends on os
BAUD_RATE = ''
watch_counter = 0
nums_saved = set([])


def gen_hist_plot(rand_numbers: []):
    test_time = datetime.now().strftime("%y_%m_%d")
    try:
        plt.hist(rand_numbers, align='left', bins=100, edgecolor='black', linewidth=1.2)
        plt.xlabel('Wartości wygenerowanych liczb')
        plt.ylabel('Częstotliwość występowania')
        plt.grid(True)
        # plt.show()
        plt.savefig('tests/' + test_time + "_hist_plot"'.pdf')
    except Exception as e:
        print("Failed to generate plot, due to:")
        print(e)
        print("Check the origin file.")
    try:
        os.system("open " + shlex.quote('tests/' + test_time + "_hist_plot"'.pdf'))
    except Exception as e:
        print("Failed to open the plot, due to: ")
        print(e)
        print("\n")
    return


def gen_col_plot(rand_numbers: []):
    test_time = datetime.now().strftime("%y_%m_%d")

    x_pairs = rand_numbers.copy()[:-1]
    y_pairs = rand_numbers.copy()[1:]
    try:
        plt.hist2d(x_pairs, y_pairs, bins=100, cmap='plasma')
        plt.grid(True)
        # plt.clim(1)
        plt.colorbar()
        plt.savefig('tests/' + test_time + "_color_map" + '.pdf')
    except Exception as e:
        print("Failed to generate plot, due to:")
        print(e)
        print("Check the origin file.")

    try:
        os.system("open " + shlex.quote('tests/' + test_time + "_color_map" + '.pdf'))
    except Exception as e:
        print("Failed to open the plot, due to: ")
        print(e)
        print("\n")


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


def read_line_uart(rand_list: []):
    global connection_lost
    build_s = ""
    build_int = 0
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
        build_s = build_s[2:].rsplit('\n\r')
        build_int = int(build_s[0])
    except Exception as e:
        print("Failed to convert the data, due to: ")
        print(e)

    if build_int != 0:
        rand_list.append(build_int)
        if len(rand_list) % 60 == 0:
            save_data(rand_list)
            rand_list.clear()

    return rand_list


def save_data(rand_list: []):
    test_time = datetime.now().strftime("%y_%m_%d")
    with open('tests/' + test_time + ".csv", 'a') as csv_file:
        csv.writer(csv_file).writerows([rand_list])


def connect_to_device():
    global stm_device
    global connection_lost
    while not connected:
        stm_device = find_device()
        time.sleep(1)
    connection_lost = 0


def read_loop():
    random_list = []
    while True:
        random_list = read_line_uart(random_list)


def generate_plot(plot_type: str):
    rand_numbers = []
    path = ""
    try:
        path = str(input("Enter data path: "))
    except Exception as e:
        print("Failed to load path, due to: ")
        print(e)

    while os.path.exists(path) is False:
        print("Can't find expected file. Enter path again.")
        try:
            path = str(input("Enter data path: "))
        except Exception as e:
            print("Failed to load path, due to: ")
            print(e)

    with open(path, 'r') as csv_file:
        csvreader = csv.reader(csv_file)
        for row in csvreader:
            for number in row:
                rand_numbers.append(int(number))

    if plot_type == 'a':
        gen_hist_plot(rand_numbers)

    if plot_type == 'b':
        gen_col_plot(rand_numbers)


def prog_main():
    user_input = 99
    print("\nMain menu")
    print("1. Read uart")
    print("2. Generate plots")
    print("0. Quit\n")

    try:
        user_input = int(input("Choose testing option: "))

    except Exception as e:
        print("Failed to obtain user input, due to:")
        print(e)
        print("\n")

    if user_input == 0:
        sys.exit()
    elif user_input == 1:
        connect_to_device()
        read_loop()
    elif user_input == 2:
        user_string = ""
        print("a. Histogram")
        print("b. Colormap")
        user_string = input("Choose type of plot: ")
        generate_plot(user_string)
    else:
        print("Please choose from existing options.")


print("################################")
print("\tUART-READER")
print("################################\n")

while True:
    prog_main()
