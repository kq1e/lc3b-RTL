module datapath(
    input clk,
    input rst,

    // CONTROL SIGNALS
    input LD_MAR,
    input LD_MAR,
    input LD_IR,
    input LD_BEN,
    input LD_REG,
    input LD_CC,
    input LD_PC,

    input GatePC,
    input GateMDR,
    input GateALU,
    input GateMARMUX,
    input GateSHF,

    input [1:0] PCMUX,
    input DRMUX,
    input SR1MUX,
    input ADDR1MUX,
    input [1:0] ADDR2MUX,
    input MARMUX,

    input ALUK,

    input MIO_EN,
    input R_W,
    input DATA_SIZE,
    input LSHF1

    inout [15:0] BUS,
    output [15:0] MAR,
    output [15:0] IR,
    output R
);
    reg [15:0] PC_temp;
    reg [15:0] MDR_temp;
    reg [15:0] MAR_temp;
    reg [15:0] IR_temp;
    wire N_temp;
    wire Z_temp;
    wire P_temp;
    reg BEN;
    

    wire SR1_out;
    wire SR2_out;

    REG regmem (
        .clk(clk)
        .rst(rst),
        .LD_REG(LD_REG),
        .BUS(BUS),
        .DRMUX(DRMUX),
        .SR1
        );

endmodule

