
import os
os.system('pip3 install pyserial')
import serial, os

COM_NAME = '/dev/ttyACM0'
BAUDRATE = 115200
TIMEOUT = 5 # Read timeout value in seconds

# Create an array that will store our wait times
wait_time_arr = []
# Create a variable that is used to read from the COM port
wait_time = ''

# Initialize an object of the serial class
ser = serial.Serial(COM_NAME, BAUDRATE, timeout=TIMEOUT)

if os.path.exists("recorded_sequence.txt"):
    os.remove("recorded_sequence.txt")


while True:
    print("Starting now..")
    user_inp = input("Enter your macro:")
    if user_inp == 'r':
        if os.path.exists("recorded_sequence.txt"):
            os.remove("recorded_sequence.txt")
        # Keep reading till you find an empty line
        input_val = bytes(user_inp, 'utf-8')
        ser.write(input_val)
        while (wait_time != b'\r\n'):
            # Check if the port is open
            if ser.is_open:
                # Read a line from the port, and store it in `wait_time`
                wait_time = ser.readline()
                # Append the data into the `wait_time_arr` array
                wait_time_arr.append(wait_time)
                # Print the data to the terminal, that is running this
                # script
                print(wait_time)
        
                file_name = open("recorded_sequence.txt", "a");
                final_str = wait_time.decode("utf-8")
                if final_str != "":
                    file_name.write(wait_time.decode("utf-8"))
        
        # Close the file
        file_name.close() 
    if user_inp == 'p': 
        # Open the file to read the contents
        file_name = open("recorded_sequence.txt", "r")
        
        # Initialize a flag and index
        flag = 0
        idx = 0
        
        # Store the file contents in an array
        file_val_arr = [x for x in file_name]
        
        input_val = bytes(user_inp, 'utf-8')
        ser.write(input_val)
                
        wait_time = ser.readline()
        
        # Keep doing this till complete message is read from console
        while (wait_time != b'Complete\r\n'):
            print(wait_time)
            # Check if this is the first write or the blinking for 1st iteration is done
            if (flag == 0 or wait_time == b'Done Blinking this iteration\r\n'):
                # Convert string to byte string
                file_val = bytes(file_val_arr[idx], 'utf-8')
                # Print out the value that is sent to the RP2040
                print(file_val)
                # Write the value to the console
                ser.write(file_val)
                # Increment the index by 2, since after every element an extra `\n`
                # follows as next element.
                idx += 2
                # Break the loop if index exceeds array length
                if idx > len(file_val_arr):
                    break
                # Update flag
                flag = 1
        
            # Read the output from the console
            wait_time = ser.readline()
        
        # Close the file
        file_name.close()

# Close the port
ser.close()
