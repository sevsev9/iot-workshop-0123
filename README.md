# iot-workshop-0123
This repository contains the resources relating to the IoT part of the 0xAthena Workshop @ 13.01.2023.

## Technology Used
- Presentation done in (reveal.js)[https://revealjs.com/]
- Code samples: (codeimg)[https://codeimg.io/]
- Coding and Flashing the Software (Arduino IDE v2)[https://docs.arduino.cc/software/ide-v2]

## Code Folders
- i2c display print text
- i2c bmp280 read data and write to serial
- serial echo program
- read bmp280, task -> print data on i2c screen
- connect to wifi
- read bmp280, task -> post data to server over wifi

## Structure of the Workshop

- Arduino IDE v2
    - Download the IDE (~200MB)
    - Download the ESP32 Libraries (~300MB)
    - Download and install the BMP280 and SSD1306 Libraries (and Adafruit GFX)
- What is IoT
- Short introduction to Arduino and the IoT Community
    - Smart Home
    - General Electronics Hobbyist Community
    - Monitoring of all of things
    - Collect all sorts of data
    - Automation
- The ESP32 (WROOM32)
    - RTOS
    - Flashing
    - Capabilities (WiFi & BT, I2C, Bus Systems)
    - Two Cores
        - What runs on which core
- Quick description of the Dev Module USB/TTY Converter
- I2C
    - Basic Workings (and superficial explanation for understanding)
    - Multiple Devices on one bus
- I2C OLED 128x64
- I2C BMP280

- Programming
    - Start a serial echo communication with the pc throught the usb tty converter
    - Read the BMP280 and send it to the serial console
    - Print text to the i2c display
    - Read the Sensor Data and sprintf it into a global malloc'd string
    - Spawn a task that prints the data on the i2c display
    - IF WE HAVE THE TIME:
        - Connect to a WiFi
        - Spawn a task that posts the data to a server in the network


# Resources

## Development Environment

Paste the following into "File > Preferences > Settings TAB > Additional boards manager URLs: ":

`https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json, http://arduino.esp8266.com/stable/package_esp8266com_index.json`

