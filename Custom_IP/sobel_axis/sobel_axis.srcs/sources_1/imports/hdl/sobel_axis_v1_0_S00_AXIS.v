
`timescale 1 ns / 1 ps

	module sobel_axis_v1_0_S00_AXIS #
	(
		parameter integer C_S_AXIS_TDATA_WIDTH	= 16
	)
	(
		input wire   S_AXIS_ACLK,
		input wire   S_AXIS_ARESETN,
		output wire  S_AXIS_TREADY,
		input wire   [C_S_AXIS_TDATA_WIDTH-1 : 0] S_AXIS_TDATA,
		input wire   S_AXIS_TLAST,
		input wire   S_AXIS_TVALID,
		
		input wire   FIFO_FULL,
		output wire   WRITE_DATA,
		output wire   WRITE_EN,
		output wire  CLK,
		output wire  RESET
	);
	
	// State Machine States
	parameter [1:0] IDLE = 1'b0,        
	                WRITE_FIFO  = 1'b1; 
	                
	wire axis_tready; 
	reg fifo_wren;
	reg writes_done;
	reg mst_exec_state;	 

	assign S_AXIS_TREADY = axis_tready;
	assign CLK = S_AXIS_ACLK;
	assign RESET = ~S_AXIS_ARESETN;
	assign WRITE_DATA = S_AXIS_TDATA;
	
	// State Machine logic
	always @(posedge S_AXIS_ACLK) 
	begin  
	  if (!S_AXIS_ARESETN) 
	      mst_exec_state <= IDLE;
	  else
	    case (mst_exec_state)
	      IDLE: 
	          if (S_AXIS_TVALID)
	              mst_exec_state <= WRITE_FIFO;
	          else
	              mst_exec_state <= IDLE;
	              
	      WRITE_FIFO: 
	        if (writes_done)
	            mst_exec_state <= IDLE;
	        else
	            mst_exec_state <= WRITE_FIFO;

	    endcase
	end

    assign axis_tready = ((mst_exec_state == WRITE_FIFO) && !FIFO_FULL);
    assign WRITE_EN = axis_tready;

    // FIFO Writing Logic
	always@(posedge S_AXIS_ACLK)
	begin
	   if(!S_AXIS_ARESETN)
	      writes_done <= 1'b0;
	      
	   else if (S_AXIS_TVALID && axis_tready)
	      writes_done <= 1'b0;
	        
       if (S_AXIS_TLAST || FIFO_FULL)
          writes_done <= 1'b1;
	end   
	  
	endmodule
