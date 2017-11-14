//Copyright 1986-2016 Xilinx, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2016.2 (win64) Build 1577090 Thu Jun  2 16:32:40 MDT 2016
//Date        : Thu Nov 09 12:17:48 2017
//Host        : nickthequik running 64-bit major release  (build 9200)
//Command     : generate_target line_buffer.bd
//Design      : line_buffer
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

(* CORE_GENERATION_INFO = "line_buffer,IP_Integrator,{x_ipVendor=xilinx.com,x_ipLibrary=BlockDiagram,x_ipName=line_buffer,x_ipVersion=1.00.a,x_ipLanguage=VERILOG,numBlks=1,numReposBlks=1,numNonXlnxBlks=0,numHierBlks=0,maxHierDepth=0,numSysgenBlks=0,numHlsBlks=0,numHdlrefBlks=0,numPkgbdBlks=0,bdsource=USER,synth_mode=Global}" *) (* HW_HANDOFF = "line_buffer.hwdef" *) 
module line_buffer
   (BRAM_PORTA_addr,
    BRAM_PORTA_clk,
    BRAM_PORTA_din,
    BRAM_PORTA_dout,
    BRAM_PORTA_we);
  input [12:0]BRAM_PORTA_addr;
  input BRAM_PORTA_clk;
  input [15:0]BRAM_PORTA_din;
  output [15:0]BRAM_PORTA_dout;
  input [0:0]BRAM_PORTA_we;

  wire [12:0]BRAM_PORTA_1_ADDR;
  wire BRAM_PORTA_1_CLK;
  wire [15:0]BRAM_PORTA_1_DIN;
  wire [15:0]BRAM_PORTA_1_DOUT;
  wire [0:0]BRAM_PORTA_1_WE;

  assign BRAM_PORTA_1_ADDR = BRAM_PORTA_addr[12:0];
  assign BRAM_PORTA_1_CLK = BRAM_PORTA_clk;
  assign BRAM_PORTA_1_DIN = BRAM_PORTA_din[15:0];
  assign BRAM_PORTA_1_WE = BRAM_PORTA_we[0];
  assign BRAM_PORTA_dout[15:0] = BRAM_PORTA_1_DOUT;
  line_buffer_blk_mem_gen_0_0 blk_mem_gen_0
       (.addra(BRAM_PORTA_1_ADDR),
        .clka(BRAM_PORTA_1_CLK),
        .dina(BRAM_PORTA_1_DIN),
        .douta(BRAM_PORTA_1_DOUT),
        .wea(BRAM_PORTA_1_WE));
endmodule
