`timescale 1ns / 1ps

module pwm_detector(
    output wire [21:0] pwm_read_1,
    output wire [21:0] pwm_read_2,
    output wire [21:0] pwm_read_3,
    input wire pwm_reset,
    input wire pwm_clk,
    input wire [5:0] pwm_in
    );
    
    pwm_detector_submodule pwm_det_1 (
        .pwm_read(pwm_read_1[10:0]),
        .pwm_reset(pwm_reset),
        .pwm_clk(pwm_clk),
        .pwm_in(pwm_in[0])
    );
    
    pwm_detector_submodule pwm_det_2 (
        .pwm_read(pwm_read_1[21:11]),
        .pwm_reset(pwm_reset),
        .pwm_clk(pwm_clk),
        .pwm_in(pwm_in[1])
    );
        
    pwm_detector_submodule pwm_det_3 (
        .pwm_read(pwm_read_2[10:0]),
        .pwm_reset(pwm_reset),
        .pwm_clk(pwm_clk),
        .pwm_in(pwm_in[2])
    );
    
    pwm_detector_submodule pwm_det_4 (
        .pwm_read(pwm_read_2[21:11]),
        .pwm_reset(pwm_reset),
        .pwm_clk(pwm_clk),
        .pwm_in(pwm_in[3])
    );
    
    pwm_detector_submodule pwm_det_5 (
        .pwm_read(pwm_read_3[10:0]),
        .pwm_reset(pwm_reset),
        .pwm_clk(pwm_clk),
        .pwm_in(pwm_in[4])
    );
                    
    pwm_detector_submodule pwm_det_6 (
        .pwm_read(pwm_read_3[21:11]),
        .pwm_reset(pwm_reset),
        .pwm_clk(pwm_clk),
        .pwm_in(pwm_in[5])
    );
    
endmodule
