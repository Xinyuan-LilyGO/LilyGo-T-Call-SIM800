'''
  Using your phone:
    - Disable PIN code on the SIM card
    - Check your balance
    - Check that APN, User, Pass are correct and you have internet
  Ensure the SIM card is correctly inserted into the board
  Ensure that GSM antenna is firmly attached

  NOTE: While GSM is connected to the Internet, WiFi can be used only in AP mode

  More docs on GSM module here:
  https://github.com/loboris/MicroPython_ESP32_psRAM_LoBo/wiki/gsm

  Author: Volodymyr Shymanskyy
'''
import socket
import machine
import time
import sys
import gsm

SIM800L_IP5306_VERSION_20190610 = 0
SIM800L_AXP192_VERSION_20200327 = 1
SIM800C_AXP192_VERSION_20200609 = 2


# Please change to the version you use here, the default version is IP5306
board_type = SIM800L_IP5306_VERSION_20190610


# APN credentials (replace with yours)
GSM_APN = ''  # Your APN
GSM_USER = ''  # Your User
GSM_PASS = ''  # Your Pass

UART_BAUD = 115200

# defaule use SIM800L_IP5306_VERSION_20190610
MODEM_POWER_PIN = 23
MODEM_RST = 5
MODEM_PWRKEY_PIN = 4
MODEM_TX = 27
MODEM_RX = 26
LED_PIN = 13


if board_type == SIM800C_AXP192_VERSION_20200609:
    LED_PIN = 12
    MODEM_POWER_PIN = 25
    MODEM_RST = 0

    # Power on the GSM module
GSM_POWER = machine.Pin(MODEM_POWER_PIN, machine.Pin.OUT)
GSM_POWER.value(1)

LED = machine.Pin(LED_PIN, machine.Pin.OUT)
LED.value(1)

if MODEM_RST > 0:
    MODEM_RST = machine.Pin(MODEM_RST, machine.Pin.OUT)
    MODEM_RST.value(1)

GSM_PWR = machine.Pin(MODEM_PWRKEY_PIN, machine.Pin.OUT)
GSM_PWR.value(1)
time.sleep_ms(200)
GSM_PWR.value(0)
time.sleep_ms(1000)
GSM_PWR.value(1)

# Init PPPoS

gsm.debug(True)  # Uncomment this to see more logs, investigate issues, etc.

gsm.start(tx=MODEM_TX, rx=MODEM_RX, apn=GSM_APN,
          user=GSM_USER, password=GSM_PASS)

sys.stdout.write('Waiting for AT command response...')
for retry in range(20):
    if gsm.atcmd('AT'):
        break
    else:
        sys.stdout.write('.')
        time.sleep_ms(5000)
else:
    raise Exception("Modem not responding!")
print()

print("Connecting to GSM...")
gsm.connect()

while gsm.status()[0] != 1:
    pass

print('IP:', gsm.ifconfig()[0])

# GSM connection is complete.
# You can now use modules like urequests, uPing, etc.
# Let's try socket API:
print("Connected !")
addr = socket.getaddrinfo('micropython.org', 80)[0][-1]
s = socket.socket()
s.connect(addr)
s.send(b'GET / HTTP/1.1\r\nHost: micropython.org\r\n\r\n')
data = s.recv(1000)
print(data)
s.close()

# You should see terminal version of StarWars episode
# Just like this: https://asciinema.org/a/1457
