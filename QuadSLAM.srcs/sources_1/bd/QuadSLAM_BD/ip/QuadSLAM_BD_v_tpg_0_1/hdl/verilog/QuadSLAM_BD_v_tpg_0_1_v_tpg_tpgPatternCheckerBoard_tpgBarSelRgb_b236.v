// ==============================================================
// File generated by Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC
// Version: 2016.2
// Copyright (C) 1986-2016 Xilinx, Inc. All Rights Reserved.
// 
// ==============================================================

`timescale 1 ns / 1 ps
(* rom_style = "distributed" *) module QuadSLAM_BD_v_tpg_0_1_v_tpg_tpgPatternCheckerBoard_tpgBarSelRgb_b236_rom (
addr0, ce0, q0, clk);

parameter DWIDTH = 1;
parameter AWIDTH = 3;
parameter MEM_SIZE = 8;

input[AWIDTH-1:0] addr0;
input ce0;
output reg[DWIDTH-1:0] q0;
input clk;

(* ram_style = "distributed" *)reg [DWIDTH-1:0] ram[MEM_SIZE-1:0];

initial begin
    $readmemh("./QuadSLAM_BD_v_tpg_0_1_v_tpg_tpgPatternCheckerBoard_tpgBarSelRgb_b236_rom.dat", ram);
end



always @(posedge clk)  
begin 
    if (ce0) 
    begin
        q0 <= ram[addr0];
    end
end



endmodule


`timescale 1 ns / 1 ps
module QuadSLAM_BD_v_tpg_0_1_v_tpg_tpgPatternCheckerBoard_tpgBarSelRgb_b236(
    reset,
    clk,
    address0,
    ce0,
    q0);

parameter DataWidth = 32'd1;
parameter AddressRange = 32'd8;
parameter AddressWidth = 32'd3;
input reset;
input clk;
input[AddressWidth - 1:0] address0;
input ce0;
output[DataWidth - 1:0] q0;



QuadSLAM_BD_v_tpg_0_1_v_tpg_tpgPatternCheckerBoard_tpgBarSelRgb_b236_rom QuadSLAM_BD_v_tpg_0_1_v_tpg_tpgPatternCheckerBoard_tpgBarSelRgb_b236_rom_U(
    .clk( clk ),
    .addr0( address0 ),
    .ce0( ce0 ),
    .q0( q0 ));

endmodule

