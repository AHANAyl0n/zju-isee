// Copyright 1986-2017 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2017.4 (win64) Build 2086221 Fri Dec 15 20:55:39 MST 2017
// Date        : Fri Dec 30 19:45:42 2022
// Host        : LAPTOP-OMEN-BAO running 64-bit major release  (build 9200)
// Command     : write_verilog -force -mode synth_stub
//               d:/Desktop/COD_Lab/lab30_Risc5CPU/vivado201704/Risc5CPU.srcs/sources_1/ip/DataRAM/DataRAM_stub.v
// Design      : DataRAM
// Purpose     : Stub declaration of top-level module interface
// Device      : xc7a200tfbg484-1
// --------------------------------------------------------------------------------

// This empty module with port declaration file causes synthesis tools to infer a black box for IP.
// The synthesis directives are for Synopsys Synplify support to prevent IO buffer insertion.
// Please paste the declaration into a Verilog source file or add the file as an additional source.
(* x_core_info = "dist_mem_gen_v8_0_12,Vivado 2017.4" *)
module DataRAM(a, d, clk, we, spo)
/* synthesis syn_black_box black_box_pad_pin="a[5:0],d[31:0],clk,we,spo[31:0]" */;
  input [5:0]a;
  input [31:0]d;
  input clk;
  input we;
  output [31:0]spo;
endmodule
