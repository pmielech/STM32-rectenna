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
BAUD_RATE = ''
watch_counter = 0
nums_saved = set([])


def gen_hist_plot(rand_numbers: []):
    test_time = datetime.now().strftime("%y_%m_%d")
    try:
        plt.hist(rand_numbers, align='left', bins=100, edgecolor='black', linewidth=1.2)
        # logs=True
        plt.xlabel('Values of the generated numbers')
        plt.ylabel('Frequency')
        plt.grid(True)
        # plt.show()
        plt.savefig('tests/' + test_time + "_hist_plot"'.pdf')
        plt.close()
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
        plt.hist2d(x_pairs, y_pairs, bins=100, cmap='plasma', vmax=20)
        #vmax=40
        #xmin, xmax, ymin, ymax = plt.axis()
        #ann = "ymax = " + str(ymax)
        #plt.annotate(ann,(1,5))
        #plt.xlim(2, 25)
        #plt.ylim(2, 25)
        # plt.grid(True)
        plt.xlabel("Value of the first number in the pair")
        plt.ylabel("Value of the second number in the pair")
        plt.colorbar()
        plt.savefig('tests/' + test_time + "_color_map" + '.pdf')
        plt.close()
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


def find_device(port_type: str):
    global connection_lost
    global connected
    device = None
    port_list = list(list_ports.comports())
    try:
        for x in port_list:
            if port_type in str(x):
                string_edit = str(x).lower()[:str(x).index(' - ')]
                device = serial.Serial(string_edit, 38400, timeout=2)
                break
    except Exception as e:
        print("Failed to connect to serial port, due to: ")
        print(e)
        sys.exit()
    if device is not None:
        connected = True
        return device
    else:
        print("Can't find any COM device")
        return device


def read_line_uart(rand_list_raw: [], rand_list_mod: []):
    global connection_lost
    msg_type = ""
    build_s = ""
    read_c = ""
    build_int = 0
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
        msg_type = build_s[:1]
        build_s = build_s[2:].rsplit('\n\r')
        build_int = int(build_s[0])
    except Exception as e:
        print("Failed to convert the data, due to: ")
        print(e)

    if build_int != 0:
        if msg_type == 'M':
            rand_list_mod.append(build_int)
            if len(rand_list_mod) % 60 == 0:
                save_data(rand_list_mod, msg_type)
                rand_list_mod.clear()

        else:
            rand_list_raw.append(build_int)
            if len(rand_list_raw) % 60 == 0:
                save_data(rand_list_raw, msg_type)
                rand_list_raw.clear()

    return rand_list_raw, rand_list_mod


def save_data(rand_list: [], data_t: str):
    test_time = datetime.now().strftime("%y_%m_%d")
    with open('tests/' + test_time + "_" + data_t + ".csv", 'a') as csv_file:
        csv.writer(csv_file).writerows([rand_list])


def connect_to_device():
    global stm_device
    global connection_lost
    com_err = 0
    port_type = ""
    try:
        port_type = str(input("Please specify COM port: "))
    except Exception as e:
        print("Failed to load port name, due to: ")
        print(e)

    while not connected:
        stm_device = find_device(port_type)
        if stm_device is None:
            com_err += 1
            if com_err > 6:
                break
        time.sleep(1)
    if com_err != 0:
        connection_lost = 1
    else:
        connection_lost = 0


def read_loop():
    global connection_lost
    random_list_raw, random_list_mod = [], []
    readings_number = 0
    if connection_lost != 0:
        return
    else:
        while readings_number == 0:
            try:
                readings_number = int(input("Enter number of measurements: "))
            except Exception as e:
                print("Failed to load number, due to:")
                print(e)

        for x in range(0, readings_number):
            random_list_raw, random_list_mod = read_line_uart(random_list_raw, random_list_mod)
        if not random_list_raw:
            save_data(random_list_raw, 'V')
            random_list_raw.clear()

        if not random_list_mod:
            save_data(random_list_mod, 'M')
            random_list_mod.clear()
        print("Completed")


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


def main():
    user_input = 99
    print("\nMain menu")
    print("1. Read uart")
    print("2. Generate plots")
    print("0. Quit\n")

    try:
        user_input = int(input("Choose testing option(1/2): "))

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
        print("a. Histogram")
        print("b. Colormap")
        user_string = input("Choose type of plot: ")
        if user_string == 'a' or user_string == 'b':
            generate_plot(user_string)
        else:
            print("{lease provide a letter from the provided options")
    else:
        print("Please choose from existing options.")


print("################################")
print("\tUART-READER")
print("################################\n")

while __name__ == "__main__":
    main()
