`timescale 1ns / 1ps

module pwm_detector_submodule(
    output reg [10:0] pwm_read,
    input wire pwm_reset,
    input wire pwm_clk,
    input wire pwm_in
    );
   
parameter IDLE   = 2'b00,
          COUNT  = 2'b01,
          OUTPUT = 2'b10,
          RSTCNT = 2'b11;   
     
reg [10:0] pwm_count;
reg [6:0] clk_div_count;
reg clk_div;
reg [1:0] state;

// State Machine logic
always @(posedge pwm_clk, posedge(pwm_reset)) 
begin  
  if (pwm_reset == 1) 
      state <= RSTCNT;
  else
    case (state)
      IDLE: 
          if (pwm_in == 1)
            state <= COUNT;  
          else
            state <= IDLE;
              
      COUNT: 
        if (pwm_in == 0)
            state <= OUTPUT;
        else
            state <= COUNT;
            
      OUTPUT:
        state <= RSTCNT;
        
      RSTCNT:
        if (pwm_count == 0)
            state <= IDLE;
        else
            state <= RSTCNT;
      
    endcase
end

/******** Clock Divider *********/
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
    if (pwm_reset == 1'b1 || state == RSTCNT)
        pwm_count <= 11'b0;
    else if (state == COUNT)
        pwm_count <= pwm_count + 1'b1;
end

/******** PWM Target FF *********/
always @(posedge pwm_clk, posedge pwm_reset) begin  
    if (pwm_reset == 1'b1)
        pwm_read <= 1'b0;
    else if (state == OUTPUT)
        pwm_read <= pwm_count;    
end
        
endmodule
