// ==============================================================
// RTL generated by Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC
// Version: 2016.2
// Copyright (C) 1986-2016 Xilinx, Inc. All Rights Reserved.
// 
// ===========================================================

`timescale 1 ns / 1 ps 

module QuadSLAM_BD_v_tpg_0_1_v_tpg_reg_int_s (
        ap_clk,
        ap_rst,
        in_r,
        ap_return,
        ap_ce
);


input   ap_clk;
input   ap_rst;
input  [30:0] in_r;
output  [30:0] ap_return;
input   ap_ce;

reg[30:0] ap_return;

always @ (posedge ap_clk) begin
    if ((1'b1 == ap_ce)) begin
        ap_return <= in_r;
    end
end

endmodule //QuadSLAM_BD_v_tpg_0_1_v_tpg_reg_int_s
