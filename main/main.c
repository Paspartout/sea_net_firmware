#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>

#include <fpga.h>

// Includes for decompression and compressed bitstream
#include "miniz.h"
#include "blinky.bit.zlib.h"

#define TAG "BLINKY"

struct bit_parser_state {
	bool header_parsed;
};

int decompression_callback(const void *data, int len, void *user)
{
	struct bit_parser_state *state = user;

	uint8_t *bitstrem_bytes = (uint8_t *)data;
	if (state->header_parsed) {
		fpga_configure_write(bitstrem_bytes, len);
	} else {
		// TODO: Proper bitstreamheader parsing
		bitstrem_bytes += 100;
		fpga_configure_write(bitstrem_bytes, len - 100);
		state->header_parsed = true;
	}

	return 1;
}

static int fpga_configure_blinky(void)
{
	fpga_configure_init();
	fpga_error_t err;
	if ((err = fpga_configure_begin()) != FPGA_OK) {
		return err;
	}

	size_t bitstream_size = blinky_bit_zlib_len;
	struct bit_parser_state parser_state = {0};
	int ret = tinfl_decompress_mem_to_callback(blinky_bit_zlib, &bitstream_size,
						   decompression_callback, &parser_state,
						   TINFL_FLAG_PARSE_ZLIB_HEADER);
	if (ret != 1) {
		return -1;
	}
	return fpga_configure_done();
}

void app_main()
{
	ESP_LOGI(TAG, "Starting configuration...");
	int start_ccount = xthal_get_ccount();
	uint64_t cycles;
	int err;
	if ((err = fpga_configure_blinky()) == 0) {
		ESP_LOGI(TAG, "Configuration done!");
	} else {
		ESP_LOGE(TAG, "Configuration failed with error code: %d\n", err);
	}
	int stop_ccount = xthal_get_ccount();

	if (stop_ccount > start_ccount)
		cycles = (stop_ccount - start_ccount);
	else
		cycles = ((uint64_t)stop_ccount + (uint64_t)0xffffffff) - (start_ccount);

	float seconds = (float)cycles / (CONFIG_ESP32_DEFAULT_CPU_FREQ_MHZ * 1000000.0f);
	ESP_LOGI(TAG, "Configuration took %llu clock cylces = %f seconds!", cycles, seconds);

	for (;;) {
		usleep(10000);
		// TODO: Go to deep sleep?
	}
}
