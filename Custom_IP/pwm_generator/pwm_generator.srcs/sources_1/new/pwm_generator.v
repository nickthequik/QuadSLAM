`timescale 1ns / 1ps

module pwm_generator(
    output [3:0] pwm_out,
    input [23:0] pwm_ctrl_1,
    input [21:0] pwm_ctrl_2,
    input pwm_clk
    );
    
pwm_generator_submodule pwm_gen_1 (
    .pwm_width(pwm_ctrl_1[10:0]),
    .pwm_en(pwm_ctrl_1[23]),
    .pwm_reset(pwm_ctrl_1[22]),
    .pwm_clk(pwm_clk),
    .pwm_output(pwm_out[0])
);

pwm_generator_submodule pwm_gen_2 (
    .pwm_width(pwm_ctrl_1[21:11]),
    .pwm_en(pwm_ctrl_1[23]),
    .pwm_reset(pwm_ctrl_1[22]),
    .pwm_clk(pwm_clk),
    .pwm_output(pwm_out[1])
);

pwm_generator_submodule pwm_gen_3 (
    .pwm_width(pwm_ctrl_2[10:0]),
    .pwm_en(pwm_ctrl_1[23]),
    .pwm_reset(pwm_ctrl_1[22]),
    .pwm_clk(pwm_clk),
    .pwm_output(pwm_out[2])
);

pwm_generator_submodule pwm_gen_4 (
    .pwm_width(pwm_ctrl_2[21:11]),
    .pwm_en(pwm_ctrl_1[23]),
    .pwm_reset(pwm_ctrl_1[22]),
    .pwm_clk(pwm_clk),
    .pwm_output(pwm_out[3])
);
    
endmodule
