
`timescale 1 ns / 1 ps

  module bd_776d_hsc_0_v_hscaler_mul_mul_6ns_11s_17_1_DSP48_2(a, b, p);
input [6 - 1 : 0] a; 
input signed [11 - 1 : 0] b; 
output signed [17 - 1 : 0] p; 

assign p = $signed ({1'b0, a}) * $signed (b);

endmodule

`timescale 1 ns / 1 ps
module bd_776d_hsc_0_v_hscaler_mul_mul_6ns_11s_17_1(
    din0,
    din1,
    dout);

parameter ID = 32'd1;
parameter NUM_STAGE = 32'd1;
parameter din0_WIDTH = 32'd1;
parameter din1_WIDTH = 32'd1;
parameter dout_WIDTH = 32'd1;
input[din0_WIDTH - 1:0] din0;
input[din1_WIDTH - 1:0] din1;
output[dout_WIDTH - 1:0] dout;



bd_776d_hsc_0_v_hscaler_mul_mul_6ns_11s_17_1_DSP48_2 bd_776d_hsc_0_v_hscaler_mul_mul_6ns_11s_17_1_DSP48_2_U(
    .a( din0 ),
    .b( din1 ),
    .p( dout ));

endmodule

