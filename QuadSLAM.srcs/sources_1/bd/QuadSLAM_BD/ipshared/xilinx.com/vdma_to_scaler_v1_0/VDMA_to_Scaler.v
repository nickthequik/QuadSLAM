`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/26/2017 11:01:04 AM
// Design Name: 
// Module Name: VDMA_to_Scaler
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


module VDMA_to_Scaler(
    input [15:0] vdma_video_in,
    output [23:0] scaler_video_out
    );
    
    //Rearrange 5 Red bits
    assign scaler_video_out[20:16] = vdma_video_in[15:11];
    assign scaler_video_out[23:21] = {3{1'b0}};
    
    //Rearrange 6 Green bits
    assign scaler_video_out[13:8] = vdma_video_in[10:5];
    assign scaler_video_out[15:14] = {2{1'b0}};
    
    //Rearrange 5 Blue bits
    assign scaler_video_out[4:0] = vdma_video_in[4:0];
    assign scaler_video_out[7:5] = {3{1'b0}};
    
    
endmodule
