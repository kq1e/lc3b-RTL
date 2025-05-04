parameter DATA_WIDTH = 8;
parameter ADDR_WIDTH = 16;

module memory(
    input clk,
    input rst,
    input [ADDR_WIDTH-1:0] ADDR,
    input we1, we0,
    output r,
    output [DATA_WIDTH-1] mem_out
);

    reg [DATA_WIDTH-1:0] RAM [0:ADDR_WIDTH-1];

    initial begin
        r = 0;
        for(integer j = 0; j < ADDR_WIDTH; j=j+1)
            RAM[j] = 0;
    end

    always @(*) begin
        if
    end



endmodule