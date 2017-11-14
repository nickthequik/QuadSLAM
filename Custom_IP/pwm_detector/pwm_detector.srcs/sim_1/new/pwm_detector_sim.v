`timescale 1ns / 1ps

module pwm_detector_sim;

reg pwm_in;
reg pwm_clk;
reg pwm_reset;
wire [10:0] pwm_read;

pwm_detector_submodule pwm_detector_dut(
    .pwm_read(pwm_read),
    .pwm_reset(pwm_reset),
    .pwm_clk(pwm_clk),
    .pwm_in(pwm_in)
);

// 125 MHz clock
always
    #4 pwm_clk = ~pwm_clk;
    
initial
begin
   pwm_clk = 0;
   pwm_in = 0;
   pwm_reset = 1;
   #160 // 20 clock cycles
   pwm_reset = 0;
   #160
   pwm_in = 1;
   #900000
   pwm_in = 0;
   #900000
   $finish;
end

endmodule;
