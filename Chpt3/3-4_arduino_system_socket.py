import socket

# Arduino's IP address and port
arduino_ip = "XXX.XXX.XXX.XXX"  # Change to your Arduino's IP address
arduino_port = 80

def receive_response(s):
    response = ""
    while True:
        chunk = s.recv(1024).decode()
        if not chunk:
            break
        response += chunk
        if '\n' in response:
            print("Response:", response.strip())
            break

def send_command(command):
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect((arduino_ip, arduino_port))
            s.sendall(command.encode())
            receive_response(s)
    except Exception as e:
        print("Error:", str(e))

send_command("TDS\n")  # Send TDS command
send_command("WL\n")   # Send WL command
send_command("DHTT\n")  # Send TDS command
send_command("DHTH\n")   # Send WL command
