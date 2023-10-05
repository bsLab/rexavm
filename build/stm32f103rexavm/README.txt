
# Tools

http://dan.drown.org/stm32duino/package_STM32duino_index.json

# Board

Board: Generic STM32F103C Series 
Variant: STM32F103CB (20kB RAM 128kB ROM)
CPU Spped.: 72 MHz
Uload method: ST Link

# Config

config.input.h

# Make VM

build

# Bootloader

st-flash --reset write STM32duino-bootloader-PB2.bin 0x8000000

# Upload Sketch

Boot Jumper Outside Position 1

# Start Sketch

Boot Jumper Outside Position 0



