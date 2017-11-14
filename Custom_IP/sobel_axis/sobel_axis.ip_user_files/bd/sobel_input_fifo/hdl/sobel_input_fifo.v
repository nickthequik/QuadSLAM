//Copyright 1986-2016 Xilinx, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2016.2 (win64) Build 1577090 Thu Jun  2 16:32:40 MDT 2016
//Date        : Thu Nov 09 10:38:24 2017
//Host        : nickthequik running 64-bit major release  (build 9200)
//Command     : generate_target sobel_input_fifo.bd
//Design      : sobel_input_fifo
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

(* CORE_GENERATION_INFO = "sobel_input_fifo,IP_Integrator,{x_ipVendor=xilinx.com,x_ipLibrary=BlockDiagram,x_ipName=sobel_input_fifo,x_ipVersion=1.00.a,x_ipLanguage=VERILOG,numBlks=1,numReposBlks=1,numNonXlnxBlks=0,numHierBlks=0,maxHierDepth=0,numSysgenBlks=0,numHlsBlks=0,numHdlrefBlks=0,numPkgbdBlks=0,bdsource=USER,synth_mode=Global}" *) (* HW_HANDOFF = "sobel_input_fifo.hwdef" *) 
module sobel_input_fifo
   (FIFO_READ_empty,
    FIFO_READ_rd_data,
    FIFO_READ_rd_en,
    FIFO_WRITE_almost_full,
    FIFO_WRITE_full,
    FIFO_WRITE_wr_data,
    FIFO_WRITE_wr_en,
    clk,
    srst);
  output FIFO_READ_empty;
  output [7:0]FIFO_READ_rd_data;
  input FIFO_READ_rd_en;
  output FIFO_WRITE_almost_full;
  output FIFO_WRITE_full;
  input [7:0]FIFO_WRITE_wr_data;
  input FIFO_WRITE_wr_en;
  input clk;
  input srst;

  wire FIFO_READ_1_EMPTY;
  wire [7:0]FIFO_READ_1_RD_DATA;
  wire FIFO_READ_1_RD_EN;
  wire FIFO_WRITE_1_ALMOST_FULL;
  wire FIFO_WRITE_1_FULL;
  wire [7:0]FIFO_WRITE_1_WR_DATA;
  wire FIFO_WRITE_1_WR_EN;
  wire clk_1;
  wire srst_1;

  assign FIFO_READ_1_RD_EN = FIFO_READ_rd_en;
  assign FIFO_READ_empty = FIFO_READ_1_EMPTY;
  assign FIFO_READ_rd_data[7:0] = FIFO_READ_1_RD_DATA;
  assign FIFO_WRITE_1_WR_DATA = FIFO_WRITE_wr_data[7:0];
  assign FIFO_WRITE_1_WR_EN = FIFO_WRITE_wr_en;
  assign FIFO_WRITE_almost_full = FIFO_WRITE_1_ALMOST_FULL;
  assign FIFO_WRITE_full = FIFO_WRITE_1_FULL;
  assign clk_1 = clk;
  assign srst_1 = srst;
  sobel_input_fifo_fifo_generator_0_0 fifo_generator_0
       (.almost_full(FIFO_WRITE_1_ALMOST_FULL),
        .clk(clk_1),
        .din(FIFO_WRITE_1_WR_DATA),
        .dout(FIFO_READ_1_RD_DATA),
        .empty(FIFO_READ_1_EMPTY),
        .full(FIFO_WRITE_1_FULL),
        .rd_en(FIFO_READ_1_RD_EN),
        .srst(srst_1),
        .wr_en(FIFO_WRITE_1_WR_EN));
endmodule
