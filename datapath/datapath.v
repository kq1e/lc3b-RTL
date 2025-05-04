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

    input PCMUX1,
    input PCMUX0,
    input DRMUX,
    input SR1MUX,
    input ADDR1MUX,
    input ADDR2MUX1,
    input ADDR2MUX0,
    input MARMUX,

    input ALUK1,
    input ALUK0, 

    input MIO_EN,
    input R_W,
    input DATA_SIZE,
    input LSHF1

    inout [15:0] BUS,
    output reg [15:0] MAR,
    output reg [15:0] IR,
    output R
);
    wire [15:0] PC_temp;
    wire [15:0] MDR_temp;
    wire [15:0] MAR_temp;
    wire [15:0] IR_temp;

    assign PC = PC_temp;
    assign MDR = MDR_temp;
    assign MAR = MAR_temp;
    assign IR = IR_temp;

    wire N_temp;
    wire Z_temp;
    wire P_temp;
    reg BEN;
    

    wire [15:0] SR1_out;
    wire [15:0] SR2_out;

    reg LC3b_Registers(
        clk,
        rst,
        LD_REG
        DRMUX,
        SR1MUX,
        SR2,
        IR_temp[11:9],
        BUS,
        SR1_out,
        SR2_out
    ); 
    

    /*
    BUS DRIVERS
    */
    wire [15:0] MARMUX_op_1;
    wire [15:0] MARMUX_op_2;
    wire [15:0] MARMUX_out;
    wire [15:0] PC_out;
    always @(*) begin
        if(GateMARMUX) begin
            if(MARMUX) begin
                case({ADDR2MUX1,ADDR2MUX0})
                    0 : MARMUX_op_1 = 0;
                    1 : MARMUX_op_1 = {{5{IR_temp[10]}},IR_temp[10:0]};
                    2 : MARMUX_op_1 = {{7{IR_temp[8]}},IR_temp[8:0]};
                    3 : MARMUX_op_1 = {{9{IR_temp[5]}},IR_temp[5:0]};
                endcase

                if(LSHF1) MARMUX_op_1 = MARMUX_op_1 << 1;

                case(ADDR1MUX)
                    0 : MARMUX_op_2 = PC_temp;
                    1 : MARMUX_op_2 = SR1_out;
                endcase

                MARMUX_out = MARMUX_op_1 + MARMUX_op_2;
            end

            if(~MARMUX) begin
                MARMUX_out = {7{0},IR_temp[7:0] << 1};
            end
        end

        if(GatePC) begin
            PC_out = PC_temp;
        end

        if(GateALU) begin
            
        end
    end

endmodule

