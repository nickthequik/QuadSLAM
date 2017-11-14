
`timescale 1 ns / 1 ps

	module sobel_axis_v1_0 #
	(
		parameter integer C_S00_AXIS_TDATA_WIDTH	= 16,
		parameter integer C_M00_AXIS_TDATA_WIDTH	= 16,
		parameter integer C_M00_AXIS_START_COUNT	= 32
	)
	(
		// Ports of Axi Slave Bus Interface S00_AXIS
		input wire [C_S00_AXIS_TDATA_WIDTH-1 : 0] s00_axis_tdata,
		input wire  s00_axis_tlast,
		output wire s00_axis_tready,
		input wire  s00_axis_tuser,
		input wire  s00_axis_tvalid,
		input wire  s00_axis_aclk,
		input wire  s00_axis_aresetn,

		// Ports of Axi Master Bus Interface M00_AXIS
		input wire  m00_axis_aclk,
		input wire  m00_axis_aresetn,
		output wire  m00_axis_tvalid,
		output wire [C_M00_AXIS_TDATA_WIDTH-1 : 0] m00_axis_tdata,
		output wire  m00_axis_tlast,
		input wire  m00_axis_tready
		
	);
	
	wire fifo_rd_empty;
	wire fifo_rd_data;
	wire fifo_rd_en;
	wire fifo_wr_full;
	wire fifo_wr_almost_full;
	wire fifo_wr_data;
	wire fifo_wr_en;
	wire sobel_clk;
	wire sobel_rst;
	
	wire line_buffer_addr;
	wire line_buffer_write;
	wire line_buffer_read;
	wire line_buffer_wr_en;
	
    // Instantiation of Axi Bus Interface S00_AXIS
	sobel_axis_v1_0_S00_AXIS # ( 
		.C_S_AXIS_TDATA_WIDTH(C_S00_AXIS_TDATA_WIDTH)
	) sobel_axis_v1_0_S00_AXIS_inst (
		.S_AXIS_ACLK(s00_axis_aclk),
		.S_AXIS_ARESETN(s00_axis_aresetn),
		.S_AXIS_TREADY(s00_axis_tready),
		.S_AXIS_TDATA(s00_axis_tdata),
		.S_AXIS_TLAST(s00_axis_tlast),
		.S_AXIS_TVALID(s00_axis_tvalid),
		.FIFO_FULL(fifo_wr_almost_full),
        .WRITE_DATA(fifo_wr_data),
        .WRITE_EN(fifo_wr_en),
        .CLK(sobel_clk),
        .RESET(sobel_rst)
	);

// Instantiation of Axi Bus Interface M00_AXIS
	sobel_axis_v1_0_M00_AXIS # ( 
		.C_M_AXIS_TDATA_WIDTH(C_M00_AXIS_TDATA_WIDTH),
		.C_M_START_COUNT(C_M00_AXIS_START_COUNT)
	) sobel_axis_v1_0_M00_AXIS_inst (
		.M_AXIS_ACLK(m00_axis_aclk),
		.M_AXIS_ARESETN(m00_axis_aresetn),
		.M_AXIS_TVALID(m00_axis_tvalid),
		.M_AXIS_TDATA(m00_axis_tdata),
		.M_AXIS_TLAST(m00_axis_tlast),
		.M_AXIS_TREADY(m00_axis_tready)
	);

    // Input FIFO
	sobel_input_fifo sobel_input_fifo_inst (
	    .FIFO_READ_empty(fifo_rd_empty),
        .FIFO_READ_rd_data(fifo_rd_data),
        .FIFO_READ_rd_en(fifo_rd_en),
        .FIFO_WRITE_almost_full(fifo_wr_almost_full),
        .FIFO_WRITE_full(fifo_wr_full),
        .FIFO_WRITE_wr_data(fifo_wr_data),
        .FIFO_WRITE_wr_en(fifo_wr_en),
        .clk(sobel_clk),
        .srst(fifo_rst)
    );
    
    // Line Buffer
    line_buffer line_buffer_inst (
        .BRAM_PORTA_addr(line_buffer_addr),
        .BRAM_PORTA_clk(sobel_clk),
        .BRAM_PORTA_din(line_buffer_write),
        .BRAM_PORTA_dout(line_buffer_read),
        .BRAM_PORTA_we(line_buffer_wr_en)
    );
    
	
	endmodule
