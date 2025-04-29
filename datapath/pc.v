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
