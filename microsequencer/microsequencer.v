module microsequencer(
    input clk,
    input rst,
    input [15:0] IR,
    output LD_MAR,
    output LD_MDR,
    output LD_IR,
    output LD_BEN,
    output LD_REG,
    output LD_CC,
    output LD_PC,
    output GATE_PC,
    output GATE_MDR,
    output GATE_ALU,
    output GATE_MARMUX,
    output GATE_SHF,
    output PCMUX1, PCMUX0,
    output DRMUX,
    output SR1MUX,
    output ADDR1MUX,
    output ADDR2MUX1, ADDR2MUX0,
    output MARMUX,
    output ALUK1, ALUK0,
    output MIO_EN,
    output R_W,
    output DATA_SIZE,
    output LSHF1
);

    reg [25:0] datapath_cs;
    
    assign {LD_MAR, LD_MDR, LD_IR, LD_BEN, LD_REG, LD_CC, LD_PC, GATE_PC, GATE_MDR,
            GATE_ALU, GATE_MARMUX, GATE_SHF, PCMUX1, PCMUX0, DRMUX, SR1MUX, ADDR1MUX,
            ADDR2MUX1, ADDR2MUX0, MARMUX, ALUK1, ALUK0, MIO_EN, R_W, DATA_SIZE, LSHF1} 
            = datapath_cs;

    reg [15:11] IR_15_11 = IR[15:11];
    reg BEN;
    reg [5:0] J; //STATE NUMBER
    reg COND1;
    reg COND0;
    reg R;
    reg IRD;



    always @(posedge clk) begin
        case(J)
            6'd00 : begin
            end
            6'd00 : begin
            end            
        endcase
    end
endmodule

