module ALU(
    input [15:0] A,
    input [15:0] B,
    input[1:0] ALUK,
    output [15:0] ALU_out
);

    reg [15:0] ALU_out_temp;

    assign ALU_out = ALU_out_temp;

    always @(*) begin
        case(ALUK)
            2'b00 : ALU_out_temp = A + B;
            2'b01 : ALU_out_temp = A & B;
            2'b10 : ALU_out_temp = A ^ B;
            2'b11 : ALU_out_temp = A;
        endcase
    end
endmodule