Benchmarks/Notes of configuration speed
=======================================

```
bitstream_bytes = 538948
bitstream_bits = 538948 * 8 = 4_311_584
esp32_clockrate = 240_000_000
```

theoretical minimum?
--------------------

Depneding on how long it takes to load and store the bitstream from flash/compression
to the gpio one can calculate the number of cycles it takes to configure the fpga:

```
cycles_per_bit = 3
flash_cycles = cycles_per_bit * bitstream_bits = 3 * 4_311_584 = 12_934_752
```

no compression
--------------

TODO: Make benchmark of bitstream from flash without compression.

zlib compressed bitstream
-------------------------

### Using gpio_set_level(rougly)

```
160 mhz 3.75s
240 mhz 2.52s
```

### Using registers

```
BLINKY: Configuration took 172_005_445 clock cylces = 0.716689 seconds!
```
