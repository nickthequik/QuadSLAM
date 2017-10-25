`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/24/2017 09:55:49 AM
// Design Name: 
// Module Name: vga_input_driver
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


module camera_input_driver(
    input [7:0] camera_vid_data_in,
    input camera_vid_hsync_in,
    input camera_vid_vsync_in,
    output camera_vid_active_video_out,
    output [7:0] camera_vid_data_out,
    output camera_vid_hsync_out,
    output camera_vid_vsync_out
    );
    
     assign camera_vid_data_out[7:0] = camera_vid_data_in[7:0];
     assign camera_vid_hsync_out = camera_vid_hsync_in;
     assign camera_vid_vsync_out = camera_vid_vsync_in;
     assign camera_vid_active_video_out = ~camera_vid_vsync_in & camera_vid_hsync_in;
     
endmodule