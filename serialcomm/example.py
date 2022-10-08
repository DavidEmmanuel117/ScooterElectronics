# python3 -m pip3 install pyserial
import serial
import time

# Warning: Check serial port to correspond the board
board_port = "/dev/ttyACM0"
board_baudrate = 115200

# Board inititialization
arduino = serial.Serial(port=board_port, baudrate=board_baudrate, timeout=.1)

def write_read(x):
    arduino.write(bytes(x, 'utf-8'))
    # time.sleep(0.05)
    data = arduino.readline().decode('utf-8')

    try:
        data = int(data)
    except:
        data = 0

    print(type(data))
    return data

while True:
    num = input("Enter a number: ")
    time.sleep(0.05)
    value = write_read(num)
    print(value) # printing the value
