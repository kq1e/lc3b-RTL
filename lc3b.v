module lc3b(
    input clk;
    input rst
);

    wire [15:0] BUS;

    /* 
    System Latches
    */
    reg [15:0] PC;
    reg [15:0] MDR;
    reg [15:0] MAR;
    reg [15:0] IR;
    wire N;
    wire Z;
    wire P;
    reg BEN;
    wire R;


    /*
    Control Signals
    */
    wire LD_MAR;
    wire LD_MAR;
    wire LD_IR;
    wire LD_BEN;
    wire LD_REG;
    wire LD_CC;
    wire LD_PC;

    wire GatePC;
    wire GateMDR;
    wire GateALU;
    wire GateMARMUX;
    wire GateSHF;

    wire [1:0] PCMUX;
    wire DRMUX;
    wire SR1MUX;
    wire ADDR1MUX;
    wire ADDR2MUX1;
    wire ADDR2MUX0;
    wire MARMUX;

    wire ALUK;

    wire MIO_EN;
    wire R_W;
    wire DATA_SIZE;
    wire LSHF1;

    datapath lc3b_datapath(
        clk,
        rst,
        LD_MAR,
        LD_MAR,
        LD_IR,
        LD_BEN,
        LD_REG,
        LD_CC,
        LD_PC,
        GatePC,
        GateMDR,
        GateALU,
        GateMARMUX,
        GateSHF,
        PCMUX1,
        PCMUX0,
        DRMUX,
        SR1MUX,
        ADDR1MUX,
        ADDR2MUX1,
        ADDR2MUX0,
        MARMUX,
        ALUK1,
        ALUK0,
        MIO_EN,
        R_W,
        DATA_SIZE,
        LSHF1
        BUS[15:0],
        MAR[15:0],
        IR[15:0],
        R
    );
    microsequencer lc3b_microsequencer(
        clk,
        rst,
        IR[15:0],
        r,
        LD_MAR,
        LD_MDR,
        LD_IR,
        LD_BEN,
        LD_REG,
        LD_CC,
        LD_PC,
        GATE_PC,
        GATE_MDR,
        GATE_ALU,
        GATE_MARMUX,
        GATE_SHF,
        PCMUX1,
        PCMUX0,
        DRMUX,
        SR1MUX,
        ADDR1MUX,
        ADDR2MUX1,
        ADDR2MUX0,
        MARMUX,
        ALUK1,
        ALUK0,
        MIO_EN,
        R_W,
        DATA_SIZE,
        LSHF1
    );

endmodule