`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/26/2017 11:14:28 AM
// Design Name: 
// Module Name: Scaler_to_Video_Out
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


module Scaler_to_Video_Out(
    input [23:0] scaler_video_in,
    output [15:0] video_out
    );
    
    //Rearrange 5 Read bits
    assign video_out[15:11] = scaler_video_in[20:16];
    
    //Rearrange 6 Green bits
    assign video_out[10:5] = scaler_video_in[13:8];
    
    //Rearrange 5 Blue bits
    assign video_out[4:0] = scaler_video_in[4:0];
endmodule
