#pragma once

#include <stdint.h>
#include <stdbool.h>

/// Error type
typedef enum fpga_error_t {
	FPGA_OK = 0,		///!< No error
	FPGA_ERROR_TIMEOUT = 1, /// < Timeout while configuration initialization
} fpga_error_t;

/// Setup gpio input/output directions for fpga configuration.
void fpga_configure_init(void);

/// Initiate fpga configuration/programming.
///
/// @return FPGA_OK or FPGA_ERROR_TIMEOUT when FPGA doesn't pull FPGA_INT_B down.
fpga_error_t fpga_configure_begin(void);

/// Return true if configuration was done successfully.
///
/// @return FPGA_OK or FPGA_ERROR_TIMEOUT when FPGA doesn't pull
fpga_error_t fpga_configure_done(void);

/// Write len bytes to the FPGA using slave serial mode. fpga_configure_begin() has to be called
/// before this.
///
/// @return FPGA_OK
fpga_error_t fpga_configure_write(const uint8_t *data, int len);
