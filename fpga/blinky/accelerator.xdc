set_property IOSTANDARD LVCMOS33 [get_ports clk]
set_property IOSTANDARD LVCMOS33 [get_ports led1]
set_property IOSTANDARD LVCMOS33 [get_ports led2]
set_property IOSTANDARD LVCMOS33 [get_ports rst]

# 100 mhz clk
set_property PACKAGE_PIN H4 [get_ports clk]
create_clock -period 10.000 -name sys_clk_pin -waveform {0.000 5.000} -add [get_ports clk]

# leds
set_property PACKAGE_PIN J1 [get_ports led1]
set_property PACKAGE_PIN A13 [get_ports led2]
set_property PACKAGE_PIN D14 [get_ports rst]
