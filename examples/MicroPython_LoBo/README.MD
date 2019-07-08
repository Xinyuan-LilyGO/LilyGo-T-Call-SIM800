
## Flash Micropython to ESP32

The recommended version of Micropython for this board is
[`MicroPython_LoBo_esp32_psram_all.zip`](https://github.com/loboris/MicroPython_ESP32_psRAM_LoBo/raw/master/MicroPython_BUILD/firmware/MicroPython_LoBo_esp32_psram_all.zip)
from [LoBo prebuilt packages](https://github.com/loboris/MicroPython_ESP32_psRAM_LoBo/tree/master/MicroPython_BUILD/firmware).

Flash using [`esptool`](https://github.com/espressif/esptool):
```sh
esptool --chip esp32 --port PORT --baud BAUD --before default_reset --after no_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size detect 0x1000 bootloader/bootloader.bin 0xf000 phy_init_data.bin 0x10000 MicroPython.bin 0x8000 partitions_mpy.bin
```
Or you can use any other GUI/tools and specify address for each file.

## Upload the example

Using [`ampy`](https://github.com/pycampers/ampy):

```sh
ampy put main.py
```
