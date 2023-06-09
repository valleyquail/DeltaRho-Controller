import socket
import time
from time import sleep
import network
from picozero import pico_temp_sensor, pico_led


def connect():
    # Connect to WLAN
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)
    wlan.connect(ssid, password)
    while not wlan.isconnected():
        print('Waiting for connection...')
        sleep(1)
    ip = wlan.ifconfig()[0]
    print(f'Connected on {ip}')
    return ip


def open_socket(ip):
    # Open a socket
    address = (ip, 80)
    connection = socket.socket()
    connection.bind(address)
    connection.listen(1)
    return connection


def webpage(temperature, state):
    # Template HTML
    html = f"""<!DOCTYPE html>
            <html>
            <form action="./lighton">
            <input type="submit" value="Light on" />
            </form>
            <form action="./lightoff">
            <input type="submit" value="Light off" />
            </form>
            <form action="./turnmotors">
            <input type="submit" value="Turn motors" />
            </form>
            <p>LED is {state}</p>
            <p>Temperature is {temperature}</p>
            </body>
            </html>
            """
    return str(html)


def serve(connection):
    # Start a web server
    state = 'OFF'
    pico_led.off()
    temperature = 0
    while True:
        client = connection.accept()[0]
        request = client.recv(1024)
        request = str(request)
        try:
            request = request.split()
            print(request)
            request = request[1]
        except IndexError:
            pass
        if request == '/lighton?':
            pico_led.on()
            state = 'ON'
        elif request == '/lightoff?':
            pico_led.off()
            state = 'OFF'
            m.MotorRun('MA', 'forward', 100, 2)
        elif request == "turn":
            pico_led.on()
            state = 'ON1'
        elif request == '/turnmotors?':
            print("test code")
            try:
                time.sleep(2)
            except KeyboardInterrupt:
                exit()
        temperature = pico_temp_sensor.temp
        html = webpage(temperature, state)
        client.send(('HTTP/1.0 200 OK\n\n' + html).encode())
        client.close()


class WIFIManager:

    def __init__(self, machine=None, network_name=None, password=None):
        self.ssid = network_name
        self.password = password
        try:
            ip = self.connect()
            connection = self.open_socket(ip)
            self.serve(connection)
        except KeyboardInterrupt:
            machine.reset()
        return
