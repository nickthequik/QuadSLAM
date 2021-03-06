-- ==============================================================
-- RTL generated by Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC
-- Version: 2016.2
-- Copyright (C) 1986-2016 Xilinx, Inc. All Rights Reserved.
-- 
-- ===========================================================

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity bd_776d_hsc_0_v_hscaler_reg_ap_uint_9_s is
port (
    ap_clk : IN STD_LOGIC;
    ap_rst : IN STD_LOGIC;
    in_V : IN STD_LOGIC_VECTOR (8 downto 0);
    ap_return : OUT STD_LOGIC_VECTOR (8 downto 0);
    ap_ce : IN STD_LOGIC );
end;


architecture behav of bd_776d_hsc_0_v_hscaler_reg_ap_uint_9_s is 
    constant ap_const_logic_1 : STD_LOGIC := '1';
    constant ap_const_logic_0 : STD_LOGIC := '0';



begin



    process (ap_clk)
    begin
        if (ap_clk'event and ap_clk = '1') then
            if ((ap_const_logic_1 = ap_ce)) then
                ap_return <= in_V;
            end if;
        end if;
    end process;
end behav;
