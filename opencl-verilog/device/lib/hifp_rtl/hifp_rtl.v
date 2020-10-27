module hifp_rtl_function
	(
		input 		clock,
		input 		resetn,
		input [31:0] 		m_input_global_id_0,
		input [31:0] 		m_input_local_id_3,
		output 		m_ready_out,
		input 		m_valid_in,
		output [31:0] 		m_output_0,
		output 		m_valid_out,
		input 		m_ready_in,
        
		input [31:0] 		m_workgroup_size,
        
		input [511:0] 		avm_local_bb1_ld__readdata,
		input 		avm_local_bb1_ld__readdatavalid,
		input 		avm_local_bb1_ld__waitrequest,
		output [31:0] 		avm_local_bb1_ld__address,
		output 		avm_local_bb1_ld__read,
		output 		avm_local_bb1_ld__write,
		input 		avm_local_bb1_ld__writeack,
		output [511:0] 		avm_local_bb1_ld__writedata,
		output [63:0] 		avm_local_bb1_ld__byteenable,
		output [4:0] 		avm_local_bb1_ld__burstcount,
		input [511:0] 		avm_local_bb1_st__readdata,
		input 		avm_local_bb1_st__readdatavalid,
		input 		avm_local_bb1_st__waitrequest,
		output [31:0] 		avm_local_bb1_st__address,
		output 		avm_local_bb1_st__read,
		output 		avm_local_bb1_st__write,
		input 		avm_local_bb1_st__writeack,
		output [511:0] 		avm_local_bb1_st__writedata,
		output [63:0] 		avm_local_bb1_st__byteenable,
		output [4:0] 		avm_local_bb1_st__burstcount,	
		input [31:0] 		m_input_wave,
		input [31:0] 		m_input_global_size_0,
		input [31:0] 		m_input_fpid
	);

endmodule