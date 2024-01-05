import epics
import threading
import time

pv_list = [
    'pv1',
    'pv2',
    'pv3',
    'pv4',
] #add as many pvs as you require

def get_pv_values():
    return [epics.caget(pv_name) for pv_name in pv_list]

def save_to_csv(interval):
    filename = f"data_log_{time.strftime('%Y-%m-%d_%H-%M-%S')}.csv"
    with open(filename, 'a') as file:
        headers_written = False
        while True:
            timestamp = time.strftime('%Y-%m-%d %H:%M:%S')
            values = get_pv_values()

            if not headers_written:
                headers = ['Timestamp'] + pv_list
                file.write(','.join(headers) + '\n')
                headers_written = True

            values = [str(value) if value is not None else '' for value in values]
            data = [timestamp] + values

            file.write(','.join(data) + '\n')
            file.flush()
            time.sleep(interval)

def periodic_saving(interval):
    save_thread = threading.Thread(target=save_to_csv, args=(interval,))
    save_thread.start()

try:
    interval = int(input("Enter time interval in seconds: "))
    periodic_saving(interval)
    while True:
        time.sleep(1)
except KeyboardInterrupt:
    print("Exiting...")
