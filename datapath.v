module datapath(
    input CLK,
    input RESET,

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
    output [15:0] IR
);

    reg[15:0] IR_temp;
    reg[15:0] MAR;

    wire SR1_out;
    wire SR2_out;
    REG regmem(.RESET(RESET),
                .LD_REG(LD_REG),
                .BUS(BUS),
                .DRMUX(DRMUX),
                .SR1)

endmodule

module REG(
    input RESET,
    input LD_REG,
    input DRMUX,
    input SR1MUX,
    input SR2,
    input [15:0] IR,
    input [15:0] BUS,

    output [15:0] SR1_out,
    output [15:0] SR2_out
);

    reg[15:0] R0;
    reg[15:0] R1;
    reg[15:0] R2;
    reg[15:0] R3;
    reg[15:0] R4;
    reg[15:0] R5;
    reg[15:0] R6;
    reg[15:0] R7;

    reg [15:0] SR1_out_temp;
    reg [15:0] SR2_out_temp;

    assign SR1_out = SR1_out_temp;
    assign SR2_out = SR2_out_temp;

    always @(*) begin
        if(RESET) begin
            R0 = 16'h0000;
            R1 = 16'h0000;
            R2 = 16'h0000;
            R3 = 16'h0000;
            R4 = 16'h0000;
            R5 = 16'h0000;
            R6 = 16'h0000;
            R7 = 16'h0000;
        end
        else begin
            if(LD_REG) begin
                if(DR) R7 = BUS;
                else begin
                    case(IR[11:9])
                        3'b000 : R0 = BUS;
                        3'b001 : R1 = BUS;
                        3'b010 : R2 = BUS;
                        3'b011 : R3 = BUS;
                        3'b100 : R4 = BUS;
                        3'b101 : R5 = BUS;
                        3'b110 : R6 = BUS;
                        3'b111 : R7 = BUS;
                    endcase
                end

            end
            else begin
                case(SR1)
                    3'b000 : SR1_out_temp = R0;
                    3'b001 : SR1_out_temp = R1;
                    3'b010 : SR1_out_temp = R2;
                    3'b011 : SR1_out_temp = R3;
                    3'b100 : SR1_out_temp = R4;
                    3'b101 : SR1_out_temp = R5;
                    3'b110 : SR1_out_temp = R6;
                    3'b111 : SR1_out_temp = R7;
                endcase

                case(SR2)
                    3'b000 : SR2_out_temp = R0;
                    3'b001 : SR2_out_temp = R1;
                    3'b010 : SR2_out_temp = R2;
                    3'b011 : SR2_out_temp = R3;
                    3'b100 : SR2_out_temp = R4;
                    3'b101 : SR2_out_temp = R5;
                    3'b110 : SR2_out_temp = R6;
                    3'b111 : SR2_out_temp = R7;
                endcase
            end
        end
    end

endmodule
module ALU(
    input [15:0] A,
    input [15:0] B,
    input[1:0] ALUK,
    output [15:0] ALU_out
);

    reg [15:0] ALU_out_temp;
    always @(*) begin
        case(ALUK)
            2'b00 : ALU_out_temp = A + B;
            2'b01 : ALU_out_temp = A & B;
            2'b10 : ALU_out_temp = A ^ B;
            2'b11 : ALU_out_temp = A;
        endcase
    end
endmodule

module SHF(
    input [15:0] A,
    input [5:0] IR,
    output [15:0] SHF_out
);
    reg [15:0] SHF_temp;

    assign SHF_out = SHF_temp;

    always @(*) begin
        //RSHF
        if(IR[4]) begin
            //RSHFA
            if(IR[5]) SHF_temp = A >>> IR[3:0];
            //RSHFL
            else SHF_temp = A >> IR[3:0];
        end

        //LSHF
        else SHF_temp = A << IR[3:0];
    end
endmodule

module PC(
    input [15:0] BUS,
    input [15:0] ADDR,
    input [15:0] PC,
    input [2:0] PCMUX,

    input LD_PC,
    output [15:0] PC_out
);

    reg [15:0] PC_temp;
    assign PC_out = PC_temp;
    always @(*) begin
        if(LD_PC) begin
            case(PCMUX)
                2'b00 : PC_temp = PC + 2;
                2'b01 : PC_temp = BUS;
                2'b10 : PC_temp = ADDR;
            endcase
        end
    end
endmodule
