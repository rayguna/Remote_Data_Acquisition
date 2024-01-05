import socket

# Arduino Ethernet shield IP address and port
arduino_ip = 'XXX.XXX.XXX.XXX'
arduino_port = 80

# Create a socket object
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Connect to the Arduino
client_socket.connect((arduino_ip, arduino_port))

while True:
    command = input("Enter '1' to turn LED on or '0' to turn LED off: ")
    
    # Send command to Arduino
    client_socket.send(command.encode())
    
    if command == 'exit':
        break

# Close the socket connection
client_socket.close()
