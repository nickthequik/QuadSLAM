`timescale 1ns / 1ps

module pwm_generator_submodule(
    input wire [10:0] pwm_width,
    input wire pwm_en,
    input wire pwm_reset,
    input wire pwm_clk,
    output reg pwm_output
    );
    
reg [10:0] pwm_target; 
reg [14:0] pwm_count;
reg overflow;
reg [6:0] clk_div_count;
reg clk_div;

/******** Clock Divider *********/
/****** 125 MHz -> 500 kHz *****/
// Clock Div Counter 
always @ (posedge(pwm_clk), posedge(pwm_reset))
begin
    if (pwm_reset == 1'b1)
        clk_div_count <= 7'b0;
    else if (clk_div_count == 'd124)
        clk_div_count <= 7'b0;
    else
        clk_div_count <= clk_div_count + 1'b1;
end

// Clock Div FF and Comparator
always @ (posedge(pwm_clk), posedge(pwm_reset))
begin
    if (pwm_reset == 1'b1)
        clk_div <= 1'b0;
    else if (clk_div_count == 'd124)
        clk_div <= ~clk_div;
    else
        clk_div <= clk_div;
end 

/******** PWM Counter *********/
always @ (posedge(clk_div), posedge(pwm_reset))
begin
    if (pwm_reset == 1'b1) begin
        pwm_count <= 15'b0;
        overflow <= 1'b0;
    end
    else if (pwm_count == 'd11000) begin // 22 ms update period
        pwm_count <= 15'b0;
        overflow <= 1'b1;
    end
    else begin
        pwm_count <= pwm_count + 1'b1;
        overflow <= 1'b0;
    end
end

/******** Output Logic *********/
always @(posedge clk_div, posedge pwm_reset) begin  
     if (pwm_reset == 1'b1)
        pwm_output <= 0;
     else if ((pwm_target > pwm_count) && (pwm_en == 1'b1))
        pwm_output <= 1'b1;
     else
        pwm_output <= 1'b0;
end

/******** PWM Target FF *********/
always @(posedge pwm_clk, posedge pwm_reset) begin  
    if (pwm_reset == 1'b1)
        pwm_target <= 1'b0;
    else if (overflow == 1'b1)
        pwm_target <= pwm_width;    
end
   
endmodule
