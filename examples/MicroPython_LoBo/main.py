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

import machine, time, sys
import gsm

# APN credentials (replace with yours)

GSM_APN  = '' # Your APN
GSM_USER = '' # Your User
GSM_PASS = '' # Your Pass

# Power on the GSM module

GSM_PWR = machine.Pin(4, machine.Pin.OUT)
GSM_RST = machine.Pin(5, machine.Pin.OUT)

GSM_PWR.value(0)
GSM_RST.value(1)

# Init PPPoS

#gsm.debug(True)  # Uncomment this to see more logs, investigate issues, etc.

gsm.start(tx=27, rx=26, apn=GSM_APN, user=GSM_USER, password=GSM_PASS)

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

import socket
addr_info = socket.getaddrinfo("towel.blinkenlights.nl", 23)
addr = addr_info[0][-1]
s = socket.socket()
s.connect(addr)

while True:
    data = s.recv(500)
    print(str(data, 'utf8'), end='')

# You should see terminal version of StarWars episode
# Just like this: https://asciinema.org/a/1457
