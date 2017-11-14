-- ==============================================================
-- File generated by Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC
-- Version: 2016.2
-- Copyright (C) 1986-2016 Xilinx, Inc. All Rights Reserved.
-- 
-- ==============================================================

--
library ieee; 
use ieee.std_logic_1164.all; 
use ieee.std_logic_unsigned.all;

entity bd_776d_vsc_0_v_vscaler_vscale_core_bicubic_LineBuf_val_val_V_0_0_ram is 
    generic(
            mem_type    : string := "block"; 
            dwidth     : integer := 8; 
            awidth     : integer := 12; 
            mem_size    : integer := 2560
    ); 
    port (
          addr0     : in std_logic_vector(awidth-1 downto 0); 
          ce0       : in std_logic; 
          q0        : out std_logic_vector(dwidth-1 downto 0);
          addr1     : in std_logic_vector(awidth-1 downto 0); 
          ce1       : in std_logic; 
          d1        : in std_logic_vector(dwidth-1 downto 0); 
          we1       : in std_logic; 
          clk        : in std_logic 
    ); 
end entity; 


architecture rtl of bd_776d_vsc_0_v_vscaler_vscale_core_bicubic_LineBuf_val_val_V_0_0_ram is 

signal addr0_tmp : std_logic_vector(awidth-1 downto 0); 
type mem_array is array (0 to mem_size-1) of std_logic_vector (dwidth-1 downto 0); 
shared variable ram : mem_array;

attribute syn_ramstyle : string; 
attribute syn_ramstyle of ram : variable is "block_ram";
attribute ram_style : string;
attribute ram_style of ram : variable is mem_type;
attribute EQUIVALENT_REGISTER_REMOVAL : string;

begin 


memory_access_guard_0: process (addr0) 
begin
      addr0_tmp <= addr0;
--synthesis translate_off
      if (CONV_INTEGER(addr0) > mem_size-1) then
           addr0_tmp <= (others => '0');
      else 
           addr0_tmp <= addr0;
      end if;
--synthesis translate_on
end process;

p_memory_access_0: process (clk)  
begin 
    if (clk'event and clk = '1') then
        if (ce0 = '1') then 
            q0 <= ram(CONV_INTEGER(addr0_tmp)); 
        end if;
    end if;
end process;


p_memory_access_1: process (clk)  
begin 
    if (clk'event and clk = '1') then
        if (ce1 = '1') then 
            if (we1 = '1') then 
                ram(CONV_INTEGER(addr1)) := d1; 
            end if;
        end if;
    end if;
end process;


end rtl;


Library IEEE;
use IEEE.std_logic_1164.all;

entity bd_776d_vsc_0_v_vscaler_vscale_core_bicubic_LineBuf_val_val_V_0_0 is
    generic (
        DataWidth : INTEGER := 8;
        AddressRange : INTEGER := 2560;
        AddressWidth : INTEGER := 12);
    port (
        reset : IN STD_LOGIC;
        clk : IN STD_LOGIC;
        address0 : IN STD_LOGIC_VECTOR(AddressWidth - 1 DOWNTO 0);
        ce0 : IN STD_LOGIC;
        q0 : OUT STD_LOGIC_VECTOR(DataWidth - 1 DOWNTO 0);
        address1 : IN STD_LOGIC_VECTOR(AddressWidth - 1 DOWNTO 0);
        ce1 : IN STD_LOGIC;
        we1 : IN STD_LOGIC;
        d1 : IN STD_LOGIC_VECTOR(DataWidth - 1 DOWNTO 0));
end entity;

architecture arch of bd_776d_vsc_0_v_vscaler_vscale_core_bicubic_LineBuf_val_val_V_0_0 is
    component bd_776d_vsc_0_v_vscaler_vscale_core_bicubic_LineBuf_val_val_V_0_0_ram is
        port (
            clk : IN STD_LOGIC;
            addr0 : IN STD_LOGIC_VECTOR;
            ce0 : IN STD_LOGIC;
            q0 : OUT STD_LOGIC_VECTOR;
            addr1 : IN STD_LOGIC_VECTOR;
            ce1 : IN STD_LOGIC;
            d1 : IN STD_LOGIC_VECTOR;
            we1 : IN STD_LOGIC);
    end component;



begin
    bd_776d_vsc_0_v_vscaler_vscale_core_bicubic_LineBuf_val_val_V_0_0_ram_U :  component bd_776d_vsc_0_v_vscaler_vscale_core_bicubic_LineBuf_val_val_V_0_0_ram
    port map (
        clk => clk,
        addr0 => address0,
        ce0 => ce0,
        q0 => q0,
        addr1 => address1,
        ce1 => ce1,
        d1 => d1,
        we1 => we1);

end architecture;


