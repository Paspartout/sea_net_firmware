Spartan Edge Accelerator Net Firmware
=====================================

**Still Work in Progress!**

Idea
----

Configure FPGA on [Spartan Edge Accelerator Board][SEAB] using HTTP Post Request using compressed bitstream file.

Proof of Concept
----------------

Currently this project configures the FPGA using a bliky deflate/zlib compressed bitstream.

On linux you can use the zlib-flate and xxd commands.
Assuming you have a bitstream file called blinky.bit in the working directory you can use the following commands
to create the header file containing the compressed bitstream:

	$ zlib-flate -compress <blinky.bit >blinky.bit.zlib
	$ xxd -i blinky.bit.zlib >main/blinky.bit.zlib.h


Acknowledgements
----------------

Thank you to the authors of the [spartan-edge-esp32-boot](https://github.com/sea-s7/spartan-edge-esp32-boot)
and [sea-fpga](https://github.com/shantzis62/sea-fpga-loader) repositories. Their code was
very helpful.

Building
--------

The project was built using the [release/v3.3 branch][idf_branch] of the esp-idf.
After the [idf is set up][idf_setup] you should only have to use `make` and `make flash` to flash
the firmware to the esp32.

[SEAB]: https://www.seeedstudio.com/Spartan-Edge-Accelerator-Board-p-4261.html
[idf_branch]: https://github.com/espressif/esp-idf/tree/release/v3.3
[idf_setup]: https://docs.espressif.com/projects/esp-idf/en/stable/get-started/index.html


