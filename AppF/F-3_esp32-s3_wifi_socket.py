import socket
import errno
import sys

ESP_IP = 'XXX.XXX.XXX.XXX'  # Replace with your ESP32's IP address
ESP_PORT = 80

def send_command(command):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.settimeout(5)  # Set a timeout for the socket connection (5 seconds in this case)
        try:
            sock.connect((ESP_IP, ESP_PORT))
            sock.sendall(command.encode())
            response = sock.recv(1024)
            print(f"Response from ESP32: {response.decode()}")
        except socket.timeout:
            print("Socket connection timed out.")
        except socket.error as e:
            if e.errno == errno.ECONNREFUSED:
                print("Connection refused. Check if the ESP32 server is running.")
            else:
                print(f"Socket error: {e}")

while True:
    user_input = input("Enter 'ON' to turn on the LED or 'OFF' to turn off: ").upper()
    if user_input == 'ON' or user_input == 'OFF':
        send_command(user_input)
    else:
        print("Invalid input. Please enter 'ON' or 'OFF'.")
