module Briey_SOC(
	//////////// CLOCK //////////
	input	CLOCK_50,

	//////////// LED //////////
	output	[3:0]LED,
	
	//////////// UART //////////
	output  UART_TXD,
	input   UART_RXD,
	
	//////////// KEY //////////
	input	[1:0]KEY,

	////////// SW //////////
//	input	[3:0]SW,

	////////// SDRAM //////////
//	output	[12:0]	DRAM_ADDR,
//	output	[1:0]	DRAM_BA,
//	output			DRAM_CAS_N,
//	output			DRAM_CKE,
//	output			DRAM_CLK,
//	output			DRAM_CS_N,
//	inout	[15:0]	DRAM_DQ,
//	output	[1:0]	DRAM_DQM,
//	output			DRAM_RAS_N,
//	output			DRAM_WE_N,


	//////////// GPIO_0, GPIO_0 connect to GPIO Default //////////
	inout mcu_TDO, //F14
    inout mcu_TCK, //C17
    inout mcu_TDI, //C18
    inout mcu_TMS  //G14

);

	wire			clk_core;
	wire			clk_sdram;
	wire			clk_25M;
	
	wire	[31:0]	gpioA_read;
	wire	[31:0]	gpioA_write;
	wire	[31:0]	gpioA_writeEnable;
  
//	wire	[15:0]	io_sdram_DQ_write;
//	wire			io_sdram_DQ_writeEnable;
	
    /////////////////////////
//	pll pll_inst (
//		.inclk0		(CLOCK_50),
//		.areset		(KEY[0]),
//		.c0			(clk_core),
//		.c1			(clk_sdram),
//		.c2			(clk_25M),
//		.locked		()
//	);
	////////////////////////

    Gowin_PLL u_Gowin_PLL(
        .lock(), 
        .clkout0(clk_core), 
        .clkout1(clk_sdram), 
        .clkout2(clk_25M), 
        .clkin(CLOCK_50), 
        .reset(~KEY[0])
    );


//	assign DRAM_CLK = clk_sdram;
	assign LED[3:0] = gpioA_write[3:0];   
//	assign gpioA_read[3:0] = SW[3:0];

//	assign DRAM_DQ = (io_sdram_DQ_writeEnable) ? io_sdram_DQ_write : 16'bz;

//	assign GPIO_0_D[0]  = 1'b1;		// VCC
//	assign GPIO_0_D[10] = 1'b0;		// reset
//	
//	 GND
//	assign GPIO_0_D[1]  = 1'b0;
//	assign GPIO_0_D[3]  = 1'b0;
//	assign GPIO_0_D[5]  = 1'b0;
//	assign GPIO_0_D[7]  = 1'b0;
//	assign GPIO_0_D[9]  = 1'b0;
//	assign GPIO_0_D[11] = 1'b0;
//	assign GPIO_0_D[13] = 1'b0;
//	assign GPIO_0_D[15] = 1'b0;

	Briey briey_inst (
		.io_asyncReset				(~KEY[0]),
		.io_axiClk					(clk_core),
		.io_vgaClk					(clk_25M),
		.io_jtag_tck				(mcu_TCK),
		.io_jtag_tms				(mcu_TMS),
		.io_jtag_tdi				(mcu_TDI),
		.io_jtag_tdo				(mcu_TDO),
		.io_gpioA_read				(gpioA_read),
		.io_gpioA_write				(gpioA_write),
		.io_gpioA_writeEnable		(gpioA_writeEnable),
		.io_gpioB_read				(32'b0),
		.io_gpioB_write				(),
		.io_gpioB_writeEnable		(),
		.io_timerExternal_clear		(1'b0),
		.io_timerExternal_tick		(1'b0),
		.io_uart_txd				(UART_TXD),
		.io_uart_rxd				(UART_RXD),
		.io_coreInterrupt			(~KEY[1])
//		.io_sdram_ADDR				(DRAM_ADDR),
//		.io_sdram_BA				(DRAM_BA),
//		.io_sdram_DQ_read			(DRAM_DQ),
//		.io_sdram_DQ_write			(io_sdram_DQ_write),
//		.io_sdram_DQ_writeEnable	(io_sdram_DQ_writeEnable),
//		.io_sdram_DQM				(DRAM_DQM),
//		.io_sdram_CASn				(DRAM_CAS_N),
//		.io_sdram_CKE				(DRAM_CKE),
//		.io_sdram_CSn				(DRAM_CS_N),
//		.io_sdram_RASn				(DRAM_RAS_N),
//		.io_sdram_WEn				(DRAM_WE_N)
    );

endmodule