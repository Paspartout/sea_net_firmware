library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity blinky is
    port ( clk : in STD_LOGIC;
           rst : in STD_LOGIC;
           led1 : out STD_LOGIC;
           led2 : out STD_LOGIC);
end blinky;

architecture rtl of blinky is
    signal count_r : integer := 0;
    signal led1_r: std_logic := '0';
begin

    process(clk)
    begin
        if rising_edge(clk) then
            if rst = '0' then
                count_r <= 0;
                led1_r <= '0';
            else
                count_r <= count_r + 1;
                if count_r = 50000000 then
                    count_r <= 0;
                    led1_r <= not led1_r;
                end if;
            end if;
        end if;
    end process;
    
    led1 <= led1_r;
    led2 <= not led1_r;

end rtl;
