module microsequencer(
    input clk,
    input rst,
    input [15:0] IR,
    input r,
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

    reg BEN;
    reg [5:0] J;
    reg COND1;
    reg COND0;
    reg IRD;

    reg [34:0] datapath_cs [0:63];

    reg [5:0] state = 18;
    reg [5:0] next_state = 33;
    
    ucode_rom ucode(datapath_cs);
    
    // Assigns control store signals
    assign {IRD, COND1, COND0, J[5], J[4], J[3], J[2], J[1], J[0], LD_MAR, 
            LD_MDR, LD_IR, LD_BEN, LD_REG, LD_CC, LD_PC, GATE_PC, GATE_MDR,
            GATE_ALU, GATE_MARMUX, GATE_SHF, PCMUX1, PCMUX0, DRMUX, SR1MUX,
            ADDR1MUX, ADDR2MUX1, ADDR2MUX0, MARMUX, ALUK1, ALUK0, MIO_EN, R_W,
            DATA_SIZE, LSHF1} 
            
            = datapath_cs[state];



    // State Machine Logic

    always @(posedge clk) begin
        state <= next_state;
    end

    always @(*) begin
        if(IRD) next_state = IR[15:12];
        else begin
            next_state = J[5:0];
            case({COND1,COND0})
                2'b11 : begin
                    if(IR[11]) next_state = next_state | 6'b000001;
                end
                2'b10 : begin
                    if(BEN) next_state = next_state | 6'b000100;
                end
                2'b01 : begin
                    if(r) next_state = next_state | 6'b000010;
                end
            endcase
        end
    end
endmodule

