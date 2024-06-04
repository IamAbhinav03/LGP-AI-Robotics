import serial  # Import the serial library for serial communication
import serial.tools.list_ports as port_list  # Import for listing serial ports

# Define the filename for the CSV output (modify as needed)
file_name = "./test.csv"

# List available serial ports (optional, for verification)
ports = list(port_list.comports())
print("Available serial ports:", ports)  # Uncomment to print port information

# Configure serial port settings (replace with your device's settings)
port = "COM13"  # Replace with the actual COM port of your serial device
baudrate = 115200  # Adjust baud rate to match your device's communication speed

# Open the serial port for communication
serialPort = serial.Serial(
    port=port, baudrate=baudrate, bytesize=8, timeout=1, stopbits=serial.STOPBITS_ONE
)

# Send a command to the serial device (replace with the appropriate command)
# serialPort.write(bytes.fromhex("A551F6"))  # Uncomment for a different command
serialPort.write(bytes.fromhex("A555FA"))  # Example command, replace if needed

# Continuously read data from the serial port
while True:
    try:
        # Read a line from the serial port and decode it
        serialString = serialPort.readline().decode("ISO-8859-1").strip()

        # Check if data is empty (optional, for handling empty reads)
        if len(serialString) == 0:
            continue

        # Parse the received data (modify based on your data format)
        l1 = []
        if "," in serialString:
            l1 = serialString.split(",")
        else:
            l1.append(serialString)

        # Write the parsed data to a CSV file
        with open(file_name, 'a+', newline="") as file:
            writer = csv.writer(file)
            writer.writerow(l1)

        # Print the received data to the console for monitoring
        print(serialString)

    except KeyboardInterrupt:
        break  # Exit the loop gracefully on Ctrl+C

# Close the serial port to release resources
serialPort.close()

