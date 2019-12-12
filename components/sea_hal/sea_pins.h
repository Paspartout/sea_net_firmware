#pragma once

// Mapping of pins -> GPIOs from datasheet at Section 2.2
// https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf

/// Pin No. 14, GPIO 25, OUTPUT
/// Pull to high to reset FPGA to program mode
#define FPGA_PROGRAM 25

/// Pin No. 27, GPIO 17, OUTPUT
/// Used to clock out bitstream to fpga
#define FPGA_CCLK 17

/// Pin No. 15, GPIO 26, INPUT
/// Used to wait for FPGA Reset/entering program mode.
#define FPGA_INTB 26

/// Pin No. 10, GPIO 34, INPUT
/// Pulled high/low?? by FPGA when configuration is done?
#define FPGA_DONE 34

/// Pin No. 16, GPIO 27, OUTPUT when configuring
/// Data input for fpga. Used together with FPGA_CCLK to configure fpga.
#define FPGA_CONFIG_DIN 27

#define FPGA_CCLK_PIN GPIO_NUM_17
#define FPGA_CCLK_PIN_SEL GPIO_SEL_17
#define FPGA_DIN_PIN GPIO_NUM_27
#define FPGA_DIN_PIN_SEL GPIO_SEL_27
#define FPGA_PROGRAM_B_PIN GPIO_NUM_25
#define FPGA_PROGRAM_B_PIN_SEL GPIO_SEL_25
#define FPGA_INT_B_PIN GPIO_NUM_26
#define FPGA_INT_B_PIN_SEL GPIO_SEL_26
#define FPGA_DONE_PIN GPIO_NUM_34
#define FPGA_DONE_PIN_SEL GPIO_SEL_34

// TODO: All the other of the ESP32 pins, e.g. SPI to FPGA and so on
