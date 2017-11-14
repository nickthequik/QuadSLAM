`timescale 1ns / 1ps

module pwm_gen_submodule_sim;

reg [10:0] pwm_width;
reg pwm_en;
reg pwm_clk;
reg pwm_reset;
wire pwm_out;

pwm_generator_submodule pwm_gen_dut(
    .pwm_width(pwm_width),
    .pwm_en(pwm_en),
    .pwm_reset(pwm_reset),
    .pwm_clk(pwm_clk),
    .pwm_output(pwm_out)
);

// 65 MHz clock
always
    #5 pwm_clk = ~pwm_clk;
    
initial
begin
   pwm_clk = 0;
   pwm_en = 0;
   pwm_reset = 1;
   pwm_width = 0;
   #100 // 10 clock cycles
   pwm_reset = 0;
   #100 // 10 clock cycles
   pwm_en = 1;
   #22000000 
   pwm_width = 2000;
   #22000000
   pwm_en = 0;
   pwm_width = 500;
   #22000000
   pwm_reset = 1;
   #22000000
   pwm_reset = 0;
   pwm_width = 250;
   #2200000
   pwm_en = 1;
   $finish;
end

endmodule;

