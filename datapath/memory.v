parameter DATA_WIDTH = 8;
parameter ADDR_WIDTH = 16;

module memory(
    input clk,
    input rst,
    input [ADDR_WIDTH-1:0] ADDR,
    input we1, we0,
    output R,
    output [DATA_WIDTH-1] 
);

    reg [DATA_WIDTH-1:0] RAM [0:ADDR_WIDTH-1];

    initial begin
        R = 0;
        for(integer j = 0; j < ADDR_WIDTH; j=j+1)
            RAM[j] = 8'h0;

    end

    always @(*)



endmodule