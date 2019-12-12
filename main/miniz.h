#pragma once

#include <rom/miniz.h>

int tinfl_decompress_mem_to_callback(const void *pIn_buf, size_t *pIn_buf_size,
				     tinfl_put_buf_func_ptr pPut_buf_func, void *pPut_buf_user,
				     int flags);
