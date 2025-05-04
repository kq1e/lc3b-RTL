module ALU(
    input [15:0] A,
    input [15:0] B,
    input[1:0] ALUK,
    output [15:0] ALU_out
);

    wire [15:0] ALU_ADD = A + B;
    wire [15:0] ALU_AND = A & B;
    wire [15:0] ALU_XOR = A ^ B;
    wire [15:0] ALU_PASSA = A;

    always @(*) begin
        case(ALUK)
            2'b00 : ALU_out = ALU_ADD;
            2'b01 : ALU_out = ALU_AND;
            2'b10 : ALU_out = ALU_XOR;
            2'b11 : ALU_out = ALU_PASSA;
        endcase
    end
endmodule