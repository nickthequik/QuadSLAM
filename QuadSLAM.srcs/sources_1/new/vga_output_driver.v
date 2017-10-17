`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/15/2017 06:14:08 PM
// Design Name: 
// Module Name: vga_output_driver
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module vga_output_driver(
    input  [15:0] vga_data_in,
    input         vga_hsync_in,
    input         vga_vsync_in,
    input         vga_hblank_in,
    input         vga_vblank_in,
    output [15:0] vga_data_out,
    output        vga_hsync_out,
    output        vga_vsync_out
    );

    assign vga_data_out[15:0] = vga_data_in[15:0] & {16{(vga_hblank_in & vga_vblank_in)}};
    assign vga_hsync_out = vga_hsync_in;
    assign vga_vsync_out = vga_vsync_in;
    
endmodule

