`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 11/02/2017 12:57:04 PM
// Design Name: 
// Module Name: tpg_to_vdma
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


module tpg_to_vdma(
    input [23:0] tpg_axis_input,
    output [7:0] vdma_axis_output
    );
    
    assign vdma_axis_output[7:0] = tpg_axis_input[7:0];

endmodule
