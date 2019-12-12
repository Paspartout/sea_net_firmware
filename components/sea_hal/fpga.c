#include <unistd.h>

#include <driver/gpio.h>
#include <esp_log.h>

#include "sea_pins.h"
#include "fpga.h"

#define TAG "FPGA"

static bool pins_configured = false;

// Since all GPIO pins are < 32, these macros can be used to directly set and clear bits.
// This is a lot faster than calling gpio_set_level from esp-idf, which does input pin validation
// and handles GPIO pins >= 32. See driver/gpio.c:174 from esp-idf.
#define GPIO_BIT_SET(GPIO_PIN) GPIO.out_w1ts = (1 << GPIO_PIN)
#define GPIO_BIT_CLR(GPIO_PIN) GPIO.out_w1tc = (1 << GPIO_PIN)
#define GPIO_BIT_SET_LEVEL(GPIO_PIN, VALUE)                                                        \
	do {                                                                                       \
		if ((VALUE)) {                                                                     \
			GPIO_BIT_SET(GPIO_PIN);                                                    \
		} else {                                                                           \
			GPIO_BIT_CLR(GPIO_PIN);                                                    \
		}                                                                                  \
	} while (0)

void fpga_configure_init(void)
{
	if (pins_configured) {
		ESP_LOGW(TAG,
			 "fpga_configure_init() called twice, pins prbably already configured");
	}
	const gpio_config_t config_input = {.mode = GPIO_MODE_INPUT,
					    .pin_bit_mask = FPGA_INT_B_PIN_SEL | FPGA_DONE_PIN_SEL,
					    .pull_up_en = GPIO_PULLUP_ENABLE,
					    .pull_down_en = GPIO_PULLDOWN_DISABLE,
					    .intr_type = GPIO_INTR_DISABLE};
	const gpio_config_t config_output = {
	    .mode = GPIO_MODE_OUTPUT,
	    .pin_bit_mask = FPGA_CCLK_PIN_SEL | FPGA_PROGRAM_B_PIN_SEL | FPGA_DIN_PIN_SEL,
	    .pull_up_en = GPIO_PULLUP_ENABLE,
	    .pull_down_en = GPIO_PULLDOWN_DISABLE,
	    .intr_type = GPIO_INTR_DISABLE};
	gpio_config(&config_input);
	gpio_config(&config_output);
	pins_configured = true;
}

#define FPGA_MAX_POLLS 1000
#define FPGA_TIMEOUT_US 1000000

fpga_error_t fpga_configure_begin(void)
{
	if (!pins_configured) {
		ESP_LOGE(TAG,
			 "fpga_configure_init() was not called before fpga_configure_begin()!");
	}

	// Tell fpga that we want to program it
	GPIO_BIT_SET_LEVEL(FPGA_PROGRAM_B_PIN, 0);
	GPIO_BIT_SET_LEVEL(FPGA_CCLK_PIN, 0);
	GPIO_BIT_SET_LEVEL(FPGA_PROGRAM_B_PIN, 1);

	// Wait until fpga is reset/ready for programming
	ESP_LOGD(TAG, "Waiting for fpga to pull INT_B down...");
	bool success = false;
	for (int i = 0; i < FPGA_MAX_POLLS; i++) {
		if (gpio_get_level(FPGA_INT_B_PIN) == 0) {
			// FPGA_INT_B pulled down => FGPA ready for configuration
			success = true;
			break;
		}
		usleep(FPGA_TIMEOUT_US / FPGA_MAX_POLLS);
	}

	return success ? FPGA_OK : FPGA_ERROR_TIMEOUT;
}

fpga_error_t fpga_configure_done(void)
{
	bool success = false;
	for (int i = 0; i < FPGA_MAX_POLLS; i++) {
		if (gpio_get_level(FPGA_DONE_PIN) == 1) {
			// FPGA_DONE_PIN pulled high => FGPA configuration done
			success = true;
			break;
		}
		usleep(FPGA_TIMEOUT_US / FPGA_MAX_POLLS);
	}

	return success ? FPGA_OK : FPGA_ERROR_TIMEOUT;
}

// TODO: More optimizations
fpga_error_t fpga_configure_write(const uint8_t *data, int len)
{
	// Write bits from data to device
	ESP_LOGD(TAG, "Writing %d len bytes...", len);
	for (int i = 0; i < len; i++) {
		const uint8_t byte = data[i];
		for (int j = 0; j < 8; j++) {
			GPIO_BIT_SET_LEVEL(FPGA_CCLK_PIN, 0);
			// configuration goes from MSB to LSB
			uint8_t bit = (byte >> (7 - j)) & 0x1;
			GPIO_BIT_SET_LEVEL(FPGA_DIN_PIN, bit);
			GPIO_BIT_SET_LEVEL(FPGA_CCLK_PIN, 1);
		}
	}

	GPIO_BIT_SET_LEVEL(FPGA_CCLK_PIN, 0);
	return FPGA_OK;
}
