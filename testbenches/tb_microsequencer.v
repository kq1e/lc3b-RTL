`timescale 1ns/1ps
module tb_microsequencer();
    reg clk;
    reg rst;
    reg [15:0] IR;
    reg r;
    wire LD_MAR;
    wire LD_MDR;
    wire LD_IR;
    wire LD_BEN;
    wire LD_wire;
    wire LD_CC;
    wire LD_PC;
    wire GATE_PC;
    wire GATE_MDR;
    wire GATE_ALU;
    wire GATE_MARMUX;
    wire GATE_SHF;
    wire PCMUX1;
    wire PCMUX0;
    wire DRMUX;
    wire SR1MUX;
    wire ADDR1MUX;
    wire ADDR2MUX1;
    wire ADDR2MUX0;
    wire MARMUX;
    wire ALUK1;
    wire ALUK0;
    wire MIO_EN;
    wire R_W;
    wire DATA_SIZE;
    wire LSHF1;

    microsequencer dut1(clk,rst,IR[15:0],r,
    LD_MAR, LD_MDR, LD_IR, LD_BEN, LD_REG, LD_CC, LD_PC,
    GATE_PC, GATE_MDR, GATE_ALU, GATE_MARMUX, GATE_SHF, PCMUX1,
    PCMUX0, DRMUX, SR1MUX, ADDR1MUX, ADDR2MUX1, ADDR2MUX0, MARMUX,
    ALUK1, ALUK0, MIO_EN, R_W, DATA_SIZE, LSHF1
    );

    initial begin
        $monitor("RESET: %0b", r);
        clk = 0; rst = 0; IR[15:0] = 16'b1110011000010011; r = 0;

    end
    
    reg memen_cycle = 0;
    parameter mem_cycles = 5;

    always @(posedge clk) begin

        if(dut1.state == 33) begin
            if(memen_cycle == mem_cycles) begin
                r = 1;
                memen_cycle = 0;
            end
            else begin
                r = 0;
                memen_cycle = memen_cycle + 1;
            end
        end
        $display("------------------");
        $display("IR[15:12]: %0b", dut1.IR[15:12]);
        $display("IR[11]: %0b", dut1.IR[11]);
        $display("J: %0d", dut1.J);
        $display("COND1: %0b", dut1.COND1);
        $display("COND0: %0b", dut1.COND0);
        $display("IRD: %0b", dut1.IRD);
        $display("STATE: %0d", dut1.state);
        $display("NEXT_STATE: %0d", dut1.next_state);

    end

    always 
        #5 clk = ~clk;



endmodule