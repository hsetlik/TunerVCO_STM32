# Eurorack VCO with STM32-powered tuner

This repo contains the code, schematic, and PCB design for a full-featured voltage controlled oscillator with an OLED-based tuner in a 9HP Eurorack module. The VCO is
built around the [AS3340 IC from Alfa](https://www.alfarzpp.lv/eng/sc/AS3340.pdf), while the tuner is powered by an [STM32G030F6P6 microcontroller](https://www.st.com/en/microcontrollers-microprocessors/stm32g030f6.html).

## Background

This is an improved version of my earlier [tuner VCO module](https://github.com/hsetlik/TunerVCO). The old one runs on an ESP32 WROOM cannibalized from an unused dev board I had lying around, with firmware written in the Arduino framework.
The old ESP32 version works fine and sounds great, but I decided it was worthwhile to make an updated version based on a different MCU for a few reasons:

1. The ESP32 is far larger and more powerful than this little OLED tuner needs
2. The ESP32 WROOM module and its associated components (USB connector, USB-to-UART bridge, BJTs, etc.) take up a lot of space on the PCB, which necessitated a horrible design involving right-angle header pins on the first version.
3. I also put the pots too close together on the first version and the knobs didn't fit

## Hardware

I've included the full KiCad project inside the 'Schematic' directory. The PCB is 4 layers with the two internal layers as ground planes. Passives and semiconductors are in SMD packages except for the AS3340 which is in a DIP-16 package.

### 3340-based VCO

The analog part of the circuit is mostly derived from the [AS3340 datasheet](https://www.alfarzpp.lv/eng/sc/AS3340.pdf) and so isn't super interesting, but a couple things worth mentioning:

1. The two trim pots to adjust the 1V/oct tracking are mounted such that they can be adjusted from the front panel without taking the module out of the case
2. There's an output stage based on summing op-amps to both isolate the AS3340 from the output jacks and to bias all three waves to a +/- 5V range

### STM32-based tuner

Out of the whole STM32 lineup I settled on the STM32G030F6P6 because

1. The TSSOP-20 package is the perfect size for the module's form factor
2. It requires no external components
3. Relatively low cost
