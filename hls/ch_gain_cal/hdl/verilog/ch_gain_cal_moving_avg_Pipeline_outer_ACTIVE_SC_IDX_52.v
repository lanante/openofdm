// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2021.1 (64-bit)
// Copyright 1986-2021 Xilinx, Inc. All Rights Reserved.
// ==============================================================
`timescale 1 ns / 1 ps
module ch_gain_cal_moving_avg_Pipeline_outer_ACTIVE_SC_IDX_52 (
address0, ce0, q0, address1, ce1, q1, reset,clk);

parameter DataWidth = 6;
parameter AddressWidth = 10;
parameter AddressRange = 564;

input[AddressWidth-1:0] address0;
input ce0;
output reg[DataWidth-1:0] q0;
input[AddressWidth-1:0] address1;
input ce1;
output reg[DataWidth-1:0] q1;
input reset;
input clk;

reg [DataWidth-1:0] ram[0:AddressRange-1];

initial begin
    $readmemh("./ch_gain_cal_moving_avg_Pipeline_outer_ACTIVE_SC_IDX_52.dat", ram);
end



always @(posedge clk)  
begin 
    if (ce0) 
    begin
        q0 <= ram[address0];
    end
end



always @(posedge clk)  
begin 
    if (ce1) 
    begin
        q1 <= ram[address1];
    end
end



endmodule

