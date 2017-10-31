
library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity QuadSLAM_BD_v_tpg_0_1_v_tpg_mul_mul_9ns_20s_28_1_DSP48_6 is
port (
    a: in std_logic_vector(9 - 1 downto 0);
    b: in std_logic_vector(20 - 1 downto 0);
    p: out std_logic_vector(28 - 1 downto 0));

end entity;

architecture behav of QuadSLAM_BD_v_tpg_0_1_v_tpg_mul_mul_9ns_20s_28_1_DSP48_6 is
    signal a_cvt: unsigned(9 - 1 downto 0);
    signal b_cvt: signed(20 - 1 downto 0);
    signal p_cvt: signed(28 - 1 downto 0);

begin

    a_cvt <= unsigned(a);
    b_cvt <= signed(b);
    p_cvt <= signed (resize(unsigned (signed ('0' & a_cvt) * signed (b_cvt)), 28));
    p <= std_logic_vector(p_cvt);

end architecture;

Library IEEE;
use IEEE.std_logic_1164.all;

entity QuadSLAM_BD_v_tpg_0_1_v_tpg_mul_mul_9ns_20s_28_1 is
    generic (
        ID : INTEGER;
        NUM_STAGE : INTEGER;
        din0_WIDTH : INTEGER;
        din1_WIDTH : INTEGER;
        dout_WIDTH : INTEGER);
    port (
        din0 : IN STD_LOGIC_VECTOR(din0_WIDTH - 1 DOWNTO 0);
        din1 : IN STD_LOGIC_VECTOR(din1_WIDTH - 1 DOWNTO 0);
        dout : OUT STD_LOGIC_VECTOR(dout_WIDTH - 1 DOWNTO 0));
end entity;

architecture arch of QuadSLAM_BD_v_tpg_0_1_v_tpg_mul_mul_9ns_20s_28_1 is
    component QuadSLAM_BD_v_tpg_0_1_v_tpg_mul_mul_9ns_20s_28_1_DSP48_6 is
        port (
            a : IN STD_LOGIC_VECTOR;
            b : IN STD_LOGIC_VECTOR;
            p : OUT STD_LOGIC_VECTOR);
    end component;



begin
    QuadSLAM_BD_v_tpg_0_1_v_tpg_mul_mul_9ns_20s_28_1_DSP48_6_U :  component QuadSLAM_BD_v_tpg_0_1_v_tpg_mul_mul_9ns_20s_28_1_DSP48_6
    port map (
        a => din0,
        b => din1,
        p => dout);

end architecture;


