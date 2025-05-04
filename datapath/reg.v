module reg(
    input clk,
    input rst,
    input LD_REG,
    input DRMUX,
    input SR1MUX,
    input SR2,
    input [11:9] IR,
    
    inout [15:0] BUS,

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

    always @(posedge clk) begin
        if(rst) begin
            R0 <= 16'h0000;
            R1 <= 16'h0000;
            R2 <= 16'h0000;
            R3 <= 16'h0000;
            R4 <= 16'h0000;
            R5 <= 16'h0000;
            R6 <= 16'h0000;
            R7 <= 16'h0000;
        end
        else begin
            if(LD_REG) begin
                if(DR) R7 <= BUS;
                else begin
                    case(IR[11:9])
                        3'b000 : R0 <= BUS;
                        3'b001 : R1 <= BUS;
                        3'b010 : R2 <= BUS;
                        3'b011 : R3 <= BUS;
                        3'b100 : R4 <= BUS;
                        3'b101 : R5 <= BUS;
                        3'b110 : R6 <= BUS;
                        3'b111 : R7 <= BUS;
                    endcase
                end

            end
            else begin
                case(SR1)
                    3'b000 : SR1_out <= R0;
                    3'b001 : SR1_out <= R1;
                    3'b010 : SR1_out <= R2;
                    3'b011 : SR1_out <= R3;
                    3'b100 : SR1_out <= R4;
                    3'b101 : SR1_out <= R5;
                    3'b110 : SR1_out <= R6;
                    3'b111 : SR1_out <= R7;
                endcase

                case(SR2)
                    3'b000 : SR2_out <= R0;
                    3'b001 : SR2_out <= R1;
                    3'b010 : SR2_out <= R2;
                    3'b011 : SR2_out <= R3;
                    3'b100 : SR2_out <= R4;
                    3'b101 : SR2_out <= R5;
                    3'b110 : SR2_out <= R6;
                    3'b111 : SR2_out <= R7;
                endcase
            end
        end
    end

endmodule




