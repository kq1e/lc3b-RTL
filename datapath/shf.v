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