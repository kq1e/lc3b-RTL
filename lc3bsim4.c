/*
    Name 1: Kyle Le
    UTEID 1: KL35298
*/


/*                      */
/*  FIX RSHFA!!!!!      */
/*                      */ 

/***************************************************************/
/*                                                             */
/*   LC-3b Simulator                                           */
/*                                                             */
/*   EE 460N                                                   */
/*   The University of Texas at Austin                         */
/*                                                             */
/***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/***************************************************************/
/*                                                             */
/* Files:  ucode        Microprogram file                      */
/*         isaprogram   LC-3b machine language program file    */
/*                                                             */
/***************************************************************/

/***************************************************************/
/* These are the functions you'll have to write.               */
/***************************************************************/

void eval_micro_sequencer();
void cycle_memory();
void eval_bus_drivers();
void drive_bus();
void latch_datapath_values();

/***************************************************************/
/* A couple of useful definitions.                             */
/***************************************************************/
#define FALSE 0
#define TRUE  1

/***************************************************************/
/* Use this to avoid overflowing 16 bits on the bus.           */
/***************************************************************/
#define Low16bits(x) ((x) & 0xFFFF)

/***************************************************************/
/* Definition of the control store layout.                     */
/***************************************************************/
#define CONTROL_STORE_ROWS 64
#define INITIAL_STATE_NUMBER 18

/***************************************************************/
/* Definition of bit order in control store word.              */
/***************************************************************/
enum CS_BITS {                                                  
    IRD,
    COND2, COND1, COND0,
    J5, J4, J3, J2, J1, J0,
    LD_MAR,
    LD_MDR,
    LD_IR,
    LD_BEN,
    LD_REG,
    LD_CC,
    LD_PC,
    GATE_PC,
    GATE_MDR,
    GATE_ALU,
    GATE_MARMUX,
    GATE_SHF,
    PCMUX1, PCMUX0,
    DRMUX1, DRMUX0,
    SR1MUX1, SR1MUX0,
    ADDR1MUX,
    ADDR2MUX1, ADDR2MUX0,
    MARMUX,
    ALUK1, ALUK0,
    MIO_EN,
    R_W,
    DATA_SIZE,
    LSHF1,
    LD_TEMPPC,
    GATE_TEMPPC,
    GATE_SPMUX,
    SPMUX,
    LD_USP,
    GATE_USP,
    LD_SSP,
    GATE_SSP,
    LD_VECTOR,
    GATE_VECTOR,
    VECTORMUX,
    LD_PRIV,
    PRIV,
    LD_PSR,
    GATE_PSR,
/* MODIFY: you have to add all your new control signals */
    CONTROL_STORE_BITS
} CS_BITS;

/***************************************************************/
/* Functions to get at the control bits.                       */
/***************************************************************/
int GetIRD(int *x)           { return(x[IRD]); }
int GetCOND(int *x)          { return((x[COND2] << 2) + (x[COND1] << 1) + x[COND0]); }
int GetJ(int *x)             { return((x[J5] << 5) + (x[J4] << 4) +
				      (x[J3] << 3) + (x[J2] << 2) +
				      (x[J1] << 1) + x[J0]); }
int GetLD_MAR(int *x)        { return(x[LD_MAR]); }
int GetLD_MDR(int *x)        { return(x[LD_MDR]); }
int GetLD_IR(int *x)         { return(x[LD_IR]); }
int GetLD_BEN(int *x)        { return(x[LD_BEN]); }
int GetLD_REG(int *x)        { return(x[LD_REG]); }
int GetLD_CC(int *x)         { return(x[LD_CC]); }
int GetLD_PC(int *x)         { return(x[LD_PC]); }
int GetGATE_PC(int *x)       { return(x[GATE_PC]); }
int GetGATE_MDR(int *x)      { return(x[GATE_MDR]); }
int GetGATE_ALU(int *x)      { return(x[GATE_ALU]); }
int GetGATE_MARMUX(int *x)   { return(x[GATE_MARMUX]); }
int GetGATE_SHF(int *x)      { return(x[GATE_SHF]); }
int GetPCMUX(int *x)         { return((x[PCMUX1] << 1) + x[PCMUX0]); }
int GetDRMUX1(int *x)         { return(x[DRMUX1]); }
int GetDRMUX0(int *x)         { return(x[DRMUX0]); }
int GetSR1MUX1(int *x)        { return(x[SR1MUX1]); }
int GetSR1MUX0(int *x)        { return(x[SR1MUX0]); }
int GetADDR1MUX(int *x)      { return(x[ADDR1MUX]); }
int GetADDR2MUX(int *x)      { return((x[ADDR2MUX1] << 1) + x[ADDR2MUX0]); }
int GetMARMUX(int *x)        { return(x[MARMUX]); }
int GetALUK(int *x)          { return((x[ALUK1] << 1) + x[ALUK0]); }
int GetMIO_EN(int *x)        { return(x[MIO_EN]); }
int GetR_W(int *x)           { return(x[R_W]); }
int GetDATA_SIZE(int *x)     { return(x[DATA_SIZE]); } 
int GetLSHF1(int *x)         { return(x[LSHF1]); }
int GetLD_TEMPPC(int *x)     { return(x[LD_TEMPPC]);}
int GetGATE_TEMPPC(int *x)     { return(x[GATE_TEMPPC]);}
int GetGATE_SPMUX(int *x)      { return(x[GATE_SPMUX]);}
int GetSPMUX(int *x)           { return(x[SPMUX]);}
int GetLD_USP(int *x)          { return(x[LD_USP]);}
int GetGATE_USP(int *x)        { return(x[GATE_USP]);}
int GetLD_SSP(int *x)          { return(x[LD_SSP]);}
int GetGATE_SSP(int *x)        { return(x[GATE_SSP]);}
int GetLD_VECTOR(int *x)       { return(x[LD_VECTOR]);}
int GetGATE_VECTOR(int *x)     { return(x[GATE_VECTOR]);}
int GetVECTORMUX(int *x)       { return(x[VECTORMUX]);}
int GetLD_PRIV(int *x)         { return(x[LD_PRIV]);}
int GetPRIV(int *x)            { return(x[PRIV]);}
int GetLD_PSR(int *x)          { return(x[LD_PSR]);}
int GetGATE_PSR(int *x)        { return(x[GATE_PSR]);}











/* MODIFY: you can add more Get functions for your new control signals */

/***************************************************************/
/* The control store rom.                                      */
/***************************************************************/
int CONTROL_STORE[CONTROL_STORE_ROWS][CONTROL_STORE_BITS];

/***************************************************************/
/* Main memory.                                                */
/***************************************************************/
/* MEMORY[A][0] stores the least significant byte of word at word address A
   MEMORY[A][1] stores the most significant byte of word at word address A 
   There are two write enable signals, one for each byte. WE0 is used for 
   the least significant byte of a word. WE1 is used for the most significant 
   byte of a word. */

#define WORDS_IN_MEM    0x08000 
#define MEM_CYCLES      5
int MEMORY[WORDS_IN_MEM][2];

/***************************************************************/

/***************************************************************/

/***************************************************************/
/* LC-3b State info.                                           */
/***************************************************************/
#define LC_3b_REGS 8

int RUN_BIT;	/* run bit */
int BUS;	/* value of the bus */

typedef struct System_Latches_Struct{

int PC,		/* program counter */
    MDR,	/* memory data register */
    MAR,	/* memory address register */
    IR,		/* instruction register */
    N,		/* n condition bit */
    Z,		/* z condition bit */
    P,		/* p condition bit */
    BEN;        /* ben register */

int READY;	/* ready bit */
  /* The ready bit is also latched as you dont want the memory system to assert it 
     at a bad point in the cycle*/

int REGS[LC_3b_REGS]; /* register file. */

int MICROINSTRUCTION[CONTROL_STORE_BITS]; /* The microintruction */

int STATE_NUMBER; /* Current State Number - Provided for debugging */ 

/* For lab 4 */
int INTV; /* Interrupt vector register */
int EXCV; /* Exception vector register */
int SSP; /* Initial value of system stack pointer */

/* MODIFY: You may add system latches that are required by your implementation */
int PSR; /* Program Status Register; PSR[15] = 0 (SUPERVISOR) = 1 (USER), PSR[2:0] = NZP */
int USP; /* User Stack Pointer */

int INT; /* Interrupt */
int EXC; /* Exception */

int TEMP_PC; /* Holds old PC */
int VECTOR; /* Vector value */


} System_Latches;

/* Data Structure for Latch */

System_Latches CURRENT_LATCHES, NEXT_LATCHES;

/***************************************************************/
/* A cycle counter.                                            */
/***************************************************************/
int CYCLE_COUNT;

/***************************************************************/
/*                                                             */
/* Procedure : help                                            */
/*                                                             */
/* Purpose   : Print out a list of commands.                   */
/*                                                             */
/***************************************************************/
void help() {                                                    
    printf("----------------LC-3bSIM Help-------------------------\n");
    printf("go               -  run program to completion       \n");
    printf("run n            -  execute program for n cycles    \n");
    printf("mdump low high   -  dump memory from low to high    \n");
    printf("rdump            -  dump the register & bus values  \n");
    printf("?                -  display this help menu          \n");
    printf("quit             -  exit the program                \n\n");
}

/***************************************************************/
/*                                                             */
/* Procedure : cycle                                           */
/*                                                             */
/* Purpose   : Execute a cycle                                 */
/*                                                             */
/***************************************************************/
void cycle() {                                                

  eval_micro_sequencer();   
  cycle_memory();
  eval_bus_drivers();
  drive_bus();
  latch_datapath_values();

  CURRENT_LATCHES = NEXT_LATCHES;

  CYCLE_COUNT++;

  if(CYCLE_COUNT == 299){
    NEXT_LATCHES.INT = 1;
    NEXT_LATCHES.INTV = 0x01;
  }
}

/***************************************************************/
/*                                                             */
/* Procedure : run n                                           */
/*                                                             */
/* Purpose   : Simulate the LC-3b for n cycles.                */
/*                                                             */
/***************************************************************/
void run(int num_cycles) {                                      
    int i;

    if (RUN_BIT == FALSE) {
	printf("Can't simulate, Simulator is halted\n\n");
	return;
    }

    printf("Simulating for %d cycles...\n\n", num_cycles);
    for (i = 0; i < num_cycles; i++) {
	if (CURRENT_LATCHES.PC == 0x0000) {
	    RUN_BIT = FALSE;
	    printf("Simulator halted\n\n");
	    break;
	}
	cycle();
    }
}

/***************************************************************/
/*                                                             */
/* Procedure : go                                              */
/*                                                             */
/* Purpose   : Simulate the LC-3b until HALTed.                 */
/*                                                             */
/***************************************************************/
void go() {                                                     
    if (RUN_BIT == FALSE) {
	printf("Can't simulate, Simulator is halted\n\n");
	return;
    }

    printf("Simulating...\n\n");
    while (CURRENT_LATCHES.PC != 0x0000)
	cycle();
    RUN_BIT = FALSE;
    printf("Simulator halted\n\n");
}

/***************************************************************/ 
/*                                                             */
/* Procedure : mdump                                           */
/*                                                             */
/* Purpose   : Dump a word-aligned region of memory to the     */
/*             output file.                                    */
/*                                                             */
/***************************************************************/
void mdump(FILE * dumpsim_file, int start, int stop) {          
    int address; /* this is a byte address */

    printf("\nMemory content [0x%0.4x..0x%0.4x] :\n", start, stop);
    printf("-------------------------------------\n");
    for (address = (start >> 1); address <= (stop >> 1); address++)
	printf("  0x%0.4x (%d) : 0x%0.2x%0.2x\n", address << 1, address << 1, MEMORY[address][1], MEMORY[address][0]);
    printf("\n");

    /* dump the memory contents into the dumpsim file */
    fprintf(dumpsim_file, "\nMemory content [0x%0.4x..0x%0.4x] :\n", start, stop);
    fprintf(dumpsim_file, "-------------------------------------\n");
    for (address = (start >> 1); address <= (stop >> 1); address++)
	fprintf(dumpsim_file, " 0x%0.4x (%d) : 0x%0.2x%0.2x\n", address << 1, address << 1, MEMORY[address][1], MEMORY[address][0]);
    fprintf(dumpsim_file, "\n");
    fflush(dumpsim_file);
}

/***************************************************************/
/*                                                             */
/* Procedure : rdump                                           */
/*                                                             */
/* Purpose   : Dump current register and bus values to the     */   
/*             output file.                                    */
/*                                                             */
/***************************************************************/
void rdump(FILE * dumpsim_file) {                               
    int k; 

    printf("\nCurrent register/bus values :\n");
    printf("-------------------------------------\n");
    printf("Cycle Count  : %d\n", CYCLE_COUNT);
    printf("PC           : 0x%0.4x\n", CURRENT_LATCHES.PC);
    printf("IR           : 0x%0.4x\n", CURRENT_LATCHES.IR);
    printf("STATE_NUMBER : 0x%0.4x\n\n", CURRENT_LATCHES.STATE_NUMBER);
    printf("BUS          : 0x%0.4x\n", BUS);
    printf("MDR          : 0x%0.4x\n", CURRENT_LATCHES.MDR);
    printf("MAR          : 0x%0.4x\n", CURRENT_LATCHES.MAR);
    printf("CCs: N = %d  Z = %d  P = %d\n", CURRENT_LATCHES.N, CURRENT_LATCHES.Z, CURRENT_LATCHES.P);
    printf("Registers:\n");
    for (k = 0; k < LC_3b_REGS; k++)
	printf("%d: 0x%0.4x\n", k, CURRENT_LATCHES.REGS[k]);
    printf("\n");

    /* dump the state information into the dumpsim file */
    fprintf(dumpsim_file, "\nCurrent register/bus values :\n");
    fprintf(dumpsim_file, "-------------------------------------\n");
    fprintf(dumpsim_file, "Cycle Count  : %d\n", CYCLE_COUNT);
    fprintf(dumpsim_file, "PC           : 0x%0.4x\n", CURRENT_LATCHES.PC);
    fprintf(dumpsim_file, "IR           : 0x%0.4x\n", CURRENT_LATCHES.IR);
    fprintf(dumpsim_file, "STATE_NUMBER : 0x%0.4x\n\n", CURRENT_LATCHES.STATE_NUMBER);
    fprintf(dumpsim_file, "BUS          : 0x%0.4x\n", BUS);
    fprintf(dumpsim_file, "MDR          : 0x%0.4x\n", CURRENT_LATCHES.MDR);
    fprintf(dumpsim_file, "MAR          : 0x%0.4x\n", CURRENT_LATCHES.MAR);
    fprintf(dumpsim_file, "CCs: N = %d  Z = %d  P = %d\n", CURRENT_LATCHES.N, CURRENT_LATCHES.Z, CURRENT_LATCHES.P);
    fprintf(dumpsim_file, "Registers:\n");
    for (k = 0; k < LC_3b_REGS; k++)
	fprintf(dumpsim_file, "%d: 0x%0.4x\n", k, CURRENT_LATCHES.REGS[k]);
    fprintf(dumpsim_file, "\n");
    fflush(dumpsim_file);
}

/***************************************************************/
/*                                                             */
/* Procedure : get_command                                     */
/*                                                             */
/* Purpose   : Read a command from standard input.             */  
/*                                                             */
/***************************************************************/
void get_command(FILE * dumpsim_file) {                         
    char buffer[20];
    int start, stop, cycles;

    printf("LC-3b-SIM> ");

    scanf("%s", buffer);
    printf("\n");

    switch(buffer[0]) {
    case 'G':
    case 'g':
	go();
	break;

    case 'M':
    case 'm':
	scanf("%i %i", &start, &stop);
	mdump(dumpsim_file, start, stop);
	break;

    case '?':
	help();
	break;
    case 'Q':
    case 'q':
	printf("Bye.\n");
	exit(0);

    case 'R':
    case 'r':
	if (buffer[1] == 'd' || buffer[1] == 'D')
	    rdump(dumpsim_file);
	else {
	    scanf("%d", &cycles);
	    run(cycles);
	}
	break;

    default:
	printf("Invalid Command\n");
	break;
    }
}

/***************************************************************/
/*                                                             */
/* Procedure : init_control_store                              */
/*                                                             */
/* Purpose   : Load microprogram into control store ROM        */ 
/*                                                             */
/***************************************************************/
void init_control_store(char *ucode_filename) {                 
    FILE *ucode;
    int i, j, index;
    char line[200];

    printf("Loading Control Store from file: %s\n", ucode_filename);

    /* Open the micro-code file. */
    if ((ucode = fopen(ucode_filename, "r")) == NULL) {
	printf("Error: Can't open micro-code file %s\n", ucode_filename);
	exit(-1);
    }

    /* Read a line for each row in the control store. */
    for(i = 0; i < CONTROL_STORE_ROWS; i++) {
	if (fscanf(ucode, "%[^\n]\n", line) == EOF) {
	    printf("Error: Too few lines (%d) in micro-code file: %s\n",
		   i, ucode_filename);
	    exit(-1);
	}

	/* Put in bits one at a time. */
	index = 0;

	for (j = 0; j < CONTROL_STORE_BITS; j++) {
	    /* Needs to find enough bits in line. */
	    if (line[index] == '\0') {
		printf("Error: Too few control bits in micro-code file: %s\nLine: %d\n",
		       ucode_filename, i);
		exit(-1);
	    }
	    if (line[index] != '0' && line[index] != '1') {
		printf("Error: Unknown value in micro-code file: %s\nLine: %d, Bit: %d\n",
		       ucode_filename, i, j);
		exit(-1);
	    }

	    /* Set the bit in the Control Store. */
	    CONTROL_STORE[i][j] = (line[index] == '0') ? 0:1;
	    index++;
	}

	/* Warn about extra bits in line. */
	if (line[index] != '\0')
	    printf("Warning: Extra bit(s) in control store file %s. Line: %d\n",
		   ucode_filename, i);
    }
    printf("\n");
}

/***************************************************************/
/*                                                             */
/* Procedure : init_memory                                     */
/*                                                             */
/* Purpose   : Zero out the memory array                       */
/*                                                             */
/***************************************************************/
void init_memory() {                                           
    int i;

    for (i=0; i < WORDS_IN_MEM; i++) {
	MEMORY[i][0] = 0;
	MEMORY[i][1] = 0;
    }
}

/**************************************************************/
/*                                                            */
/* Procedure : load_program                                   */
/*                                                            */
/* Purpose   : Load program and service routines into mem.    */
/*                                                            */
/**************************************************************/
void load_program(char *program_filename) {                   
    FILE * prog;
    int ii, word, program_base;

    /* Open program file. */
    prog = fopen(program_filename, "r");
    if (prog == NULL) {
	printf("Error: Can't open program file %s\n", program_filename);
	exit(-1);
    }

    /* Read in the program. */
    if (fscanf(prog, "%x\n", &word) != EOF)
	program_base = word >> 1;
    else {
	printf("Error: Program file is empty\n");
	exit(-1);
    }

    ii = 0;
    while (fscanf(prog, "%x\n", &word) != EOF) {
	/* Make sure it fits. */
	if (program_base + ii >= WORDS_IN_MEM) {
	    printf("Error: Program file %s is too long to fit in memory. %x\n",
		   program_filename, ii);
	    exit(-1);
	}

	/* Write the word to memory array. */
	MEMORY[program_base + ii][0] = word & 0x00FF;
	MEMORY[program_base + ii][1] = (word >> 8) & 0x00FF;
	ii++;
    }

    if (CURRENT_LATCHES.PC == 0) CURRENT_LATCHES.PC = (program_base << 1);

    printf("Read %d words from program into memory.\n\n", ii);
}

/***************************************************************/
/*                                                             */
/* Procedure : initialize                                      */
/*                                                             */
/* Purpose   : Load microprogram and machine language program  */ 
/*             and set up initial state of the machine.        */
/*                                                             */
/***************************************************************/
void initialize(char *argv[], int num_prog_files) { 
    int i;
    init_control_store(argv[1]);

    init_memory();
    for ( i = 0; i < num_prog_files; i++ ) {
	load_program(argv[i + 2]);
    }
    CURRENT_LATCHES.Z = 1;
    CURRENT_LATCHES.STATE_NUMBER = INITIAL_STATE_NUMBER;
    memcpy(CURRENT_LATCHES.MICROINSTRUCTION, CONTROL_STORE[INITIAL_STATE_NUMBER], sizeof(int)*CONTROL_STORE_BITS);
    CURRENT_LATCHES.SSP = 0x3000; /* Initial value of system stack pointer */
    CURRENT_LATCHES.INT = 0;
    CURRENT_LATCHES.PSR = 0x8002;
    CURRENT_LATCHES.USP = 0xFE00;

    NEXT_LATCHES = CURRENT_LATCHES;

    RUN_BIT = TRUE;
}

/***************************************************************/
/*                                                             */
/* Procedure : main                                            */
/*                                                             */
/***************************************************************/
int main(int argc, char *argv[]) {                              
    FILE * dumpsim_file;

    /* Error Checking */
    if (argc < 3) {
	printf("Error: usage: %s <micro_code_file> <program_file_1> <program_file_2> ...\n",
	       argv[0]);
	exit(1);
    }

    printf("LC-3b Simulator\n\n");

    initialize(argv, argc - 2);

    if ( (dumpsim_file = fopen( "dumpsim", "w" )) == NULL ) {
	printf("Error: Can't open dumpsim file\n");
	exit(-1);
    }

    while (1)
	get_command(dumpsim_file);

}

/***************************************************************/
/* Do not modify the above code, except for the places indicated 
   with a "MODIFY:" comment.

   Do not modify the rdump and mdump functions.

   You are allowed to use the following global variables in your
   code. These are defined above.

   CONTROL_STORE
   MEMORY
   BUS

   CURRENT_LATCHES
   NEXT_LATCHES

   You may define your own local/global variables and functions.
   You may use the functions to get at the control bits defined
   above.

   Begin your code here 	  			       */
/***************************************************************/


int drivers;
int SEXT(int num, int numbit){
    int mask = 0xFFFF << numbit;
    numbit = numbit-1;
    if( (num >> numbit) & 0x0001){ //is negative number
        return Low16bits(num |= mask);
    }
    return Low16bits(num);
}


void eval_micro_sequencer() {

  /* 
   * Evaluate the address of the next state according to the 
   * micro sequencer logic. Latch the next microinstruction.
   */
    
  
  if( GetIRD(CURRENT_LATCHES.MICROINSTRUCTION) ){
    NEXT_LATCHES.STATE_NUMBER = Low16bits((CURRENT_LATCHES.IR & 0xF000) >> 12);
  }

  else{
    NEXT_LATCHES.STATE_NUMBER = GetJ(CURRENT_LATCHES.MICROINSTRUCTION);
    int COND_bits = GetCOND(CURRENT_LATCHES.MICROINSTRUCTION);
    if(CURRENT_LATCHES.STATE_NUMBER == 47) NEXT_LATCHES.INT = 0;
    switch(COND_bits){
        case 4:
            if(CURRENT_LATCHES.EXC) NEXT_LATCHES.STATE_NUMBER = 10;
            break;
        case 5:
            if(CURRENT_LATCHES.INT) NEXT_LATCHES.STATE_NUMBER |= 0x0008;
            break;
        case 3:
            if(CURRENT_LATCHES.IR & 0x0800) NEXT_LATCHES.STATE_NUMBER |= 0x0001;
            break;
        case 2: 
            if(CURRENT_LATCHES.BEN) NEXT_LATCHES.STATE_NUMBER |= 0x0004; 
            break;
        case 1: 
            if(CURRENT_LATCHES.READY) NEXT_LATCHES.STATE_NUMBER |= 0x0002;
            break;
        }  

        if(NEXT_LATCHES.STATE_NUMBER == 0x1600) printf("ENTERING PROTECTION VECTOR\n");
        if(NEXT_LATCHES.STATE_NUMBER == 0x1A00) printf("ENTERING UNALIGNED VECTOR\n");

    }

    
    for(int i = 0; i < CONTROL_STORE_BITS; i++) NEXT_LATCHES.MICROINSTRUCTION[i] = CONTROL_STORE[NEXT_LATCHES.STATE_NUMBER][i];

}

int MEMEN_CYCLE = 0;


void cycle_memory() {
 
  /* 
   * This function emulates memory and the WE logic. 
   * Keep track of which cycle of MEMEN we are dealing with.  
   * If fourth, we need to latch Ready bit at the end of 
   * cycle to prepare microsequencer for the fifth cycle.  
   */

  if(CURRENT_LATCHES.STATE_NUMBER == 36){
    printf("MDR <- (PSR) 0x%.4x\n", CURRENT_LATCHES.PSR);
  }
  if(CURRENT_LATCHES.STATE_NUMBER == 54){
    printf("PSR <- (MDR) 0x%.4x\n", CURRENT_LATCHES.MDR);
  }

  if(GetMIO_EN(CURRENT_LATCHES.MICROINSTRUCTION)){
    if(MEMEN_CYCLE == MEM_CYCLES){
        MEMEN_CYCLE = 1;
    } else {
        MEMEN_CYCLE++;
    }
  }
  else return;

}



enum gateENUM {
    none,
    gateMARMUX,
    gatePC,
    gateALU,
    gateSHF,
    gateMDR,
    gateTempPC,
    gateSPMUX,
    gateUSP,
    gateSSP,
    gateVECTOR,
    gatePSR
} gateENUM;

void eval_bus_drivers() {

  /* 
   * Datapath routine emulating operations before driving the bus.
   * Evaluate the input of tristate drivers 
   *             Gate_MARMUX,
   *		 Gate_PC,
   *		 Gate_ALU,
   *		 Gate_SHF,
   *		 Gate_MDR.
   */    
  if( GetGATE_MARMUX(CURRENT_LATCHES.MICROINSTRUCTION) ){
    drivers = gateMARMUX;
  } else if( GetGATE_PC(CURRENT_LATCHES.MICROINSTRUCTION) ){
    drivers = gatePC;
  } else if( GetGATE_ALU(CURRENT_LATCHES.MICROINSTRUCTION)){
    drivers = gateALU;
  } else if( GetGATE_SHF(CURRENT_LATCHES.MICROINSTRUCTION) ){
    drivers = gateSHF;
  } else if( GetGATE_MDR(CURRENT_LATCHES.MICROINSTRUCTION) ){
    drivers = gateMDR;
  } else if( GetGATE_TEMPPC(CURRENT_LATCHES.MICROINSTRUCTION) ){
    drivers = gateTempPC;
  } else if( GetGATE_SPMUX(CURRENT_LATCHES.MICROINSTRUCTION) ){
    drivers = gateSPMUX;
  } else if( GetGATE_USP(CURRENT_LATCHES.MICROINSTRUCTION) ){
    drivers = gateUSP;
  } else if( GetGATE_SSP(CURRENT_LATCHES.MICROINSTRUCTION) ){
    drivers = gateSSP;
  } else if( GetGATE_VECTOR(CURRENT_LATCHES.MICROINSTRUCTION) ){
    drivers = gateVECTOR;
  } else if( GetGATE_PSR(CURRENT_LATCHES.MICROINSTRUCTION) ){
    drivers = gatePSR;
  } else {
    drivers = none;
  }

}

void drive_bus() {

  /* 
   * Datapath routine for driving the bus from one of the 5 possible 
   * tristate drivers. 
   *
   */
  
  if(drivers == none) {
    BUS = 0;
  }

  else if(drivers == gateMARMUX) {
   if( GetMARMUX(CURRENT_LATCHES.MICROINSTRUCTION) ){
    int OP1, OP2;

    switch( GetADDR2MUX(CURRENT_LATCHES.MICROINSTRUCTION) ){
        case 0: OP1 = 0; break;
        case 1: OP1 = Low16bits(SEXT((CURRENT_LATCHES.IR & 0x003F),6)); break;
        case 2: OP1 = Low16bits(SEXT((CURRENT_LATCHES.IR & 0x01FF),9));break;
        case 3: OP1 = Low16bits(SEXT((CURRENT_LATCHES.IR & 0x07FF),11));break;
    }
    switch( GetADDR1MUX(CURRENT_LATCHES.MICROINSTRUCTION) ){
        
        case 0: OP2 = CURRENT_LATCHES.PC; break;
        case 1:
            if(GetSR1MUX0(CURRENT_LATCHES.MICROINSTRUCTION)) OP2 = CURRENT_LATCHES.REGS[(CURRENT_LATCHES.IR>>6) & 0x0007];
            else OP2 = CURRENT_LATCHES.REGS[(CURRENT_LATCHES.IR>>9) & 0x0007];
            break;
    }
    if(GetLSHF1(CURRENT_LATCHES.MICROINSTRUCTION)) OP1 = Low16bits(OP1 << 1);

    BUS = Low16bits(OP1 + OP2);
   }
   else {
    BUS = Low16bits((CURRENT_LATCHES.IR & 0x00FF) << 1);
   }
  }

  else if(drivers == gatePC){
    BUS = CURRENT_LATCHES.PC;
  }

  else if(drivers == gateALU){
    int OP1;
    int OP2;

    if(GetSR1MUX1(CURRENT_LATCHES.MICROINSTRUCTION)) OP1 = CURRENT_LATCHES.REGS[6];
    else if(GetSR1MUX0(CURRENT_LATCHES.MICROINSTRUCTION)) OP1 = CURRENT_LATCHES.REGS[(CURRENT_LATCHES.IR>>6) & 0x0007];
    else OP1 = CURRENT_LATCHES.REGS[(CURRENT_LATCHES.IR>>9) & 0x0007];

    //Check Immediate Bit
    if(((CURRENT_LATCHES.IR>>5) & 0x0001)==1){
        OP2 = Low16bits( SEXT((CURRENT_LATCHES.IR & 0x001F), 5) );
    }
    else {
        OP2 = CURRENT_LATCHES.REGS[CURRENT_LATCHES.IR & 0x0007];
    }

    switch (GetALUK(CURRENT_LATCHES.MICROINSTRUCTION)) {
        case 0: BUS = Low16bits(OP1 + OP2); break;
        case 1: BUS = Low16bits(OP1 & OP2); break;
        case 2: BUS = Low16bits(OP1 ^ OP2); break;
        case 3: BUS = OP1; break;
    }
  }

  else if(drivers == gateSHF){
    int shift_mode = (CURRENT_LATCHES.IR >> 4) & 0x0003;
    int shift_amnt = CURRENT_LATCHES.IR & 0x000F;
    int SR = CURRENT_LATCHES.REGS[(CURRENT_LATCHES.IR>>6) & 0x0007];

    switch (shift_mode) {
        case 0: SR = SR << shift_amnt; break;
        case 1: SR = (unsigned int) SR >> shift_amnt; break;
        case 3: SR = SR >> shift_amnt;
            SR |= Low16bits((0xFFFF << (16-shift_amnt)));break;
    }

    BUS = Low16bits(SR);
  }

  else if(drivers == gateMDR){
    if(GetDATA_SIZE(CURRENT_LATCHES.MICROINSTRUCTION)) BUS = CURRENT_LATCHES.MDR;
    else BUS = Low16bits(SEXT(CURRENT_LATCHES.MDR & 0x00FF, 8));
  }

  else if(drivers == gateTempPC){
    BUS = CURRENT_LATCHES.TEMP_PC;
  }

  else if(drivers == gateSPMUX){
    int sp = CURRENT_LATCHES.REGS[6];
    if(GetSPMUX(CURRENT_LATCHES.MICROINSTRUCTION)) sp = sp+2;
    else sp=sp-2;

    BUS = Low16bits(sp);
  }

  else if(drivers == gateUSP){
    BUS = CURRENT_LATCHES.USP;
  }

  else if(drivers == gateSSP){
    BUS = CURRENT_LATCHES.SSP;
  }

  else if(drivers == gateVECTOR){
    BUS = CURRENT_LATCHES.VECTOR;
  }

  else if(drivers == gatePSR){
    BUS = CURRENT_LATCHES.PSR;
  }

}

void setCC(int num){
    //16 bit num from bus, if num[15] = 1, negative, if num = 0, zero, else positive
    if( ((num>>15) & 0x0001) == 1) {
        NEXT_LATCHES.N = 1; NEXT_LATCHES.Z = 0; NEXT_LATCHES.P = 0;
        NEXT_LATCHES.PSR |= 0x0004;
    } else if(num == 0){
        NEXT_LATCHES.N = 0; NEXT_LATCHES.Z = 1; NEXT_LATCHES.P = 0;
        NEXT_LATCHES.PSR |= 0x0002;
    } else {
        NEXT_LATCHES.N = 0; NEXT_LATCHES.Z = 0; NEXT_LATCHES.P = 1;
        NEXT_LATCHES.PSR |= 0x0001;
    }
}

void latch_datapath_values() {

  /* 
   * Datapath routine for computing all functions that need to latch
   * values in the data path at the end of this cycle.  Some values
   * require sourcing the bus; therefore, this routine has to come 
   * after drive_bus.
   */

  if( GetMIO_EN(CURRENT_LATCHES.MICROINSTRUCTION)){
    if(MEMEN_CYCLE == MEM_CYCLES-1) {
        NEXT_LATCHES.READY = 1;
    }
    else if(MEMEN_CYCLE == MEM_CYCLES) {
        NEXT_LATCHES.READY = 0;
    }
  }

  if(GetR_W(CURRENT_LATCHES.MICROINSTRUCTION)){
    if(GetDATA_SIZE(CURRENT_LATCHES.MICROINSTRUCTION)){
          MEMORY[CURRENT_LATCHES.MAR>>1][0] = Low16bits(CURRENT_LATCHES.MDR & 0x00FF);
          MEMORY[CURRENT_LATCHES.MAR>>1][1] = Low16bits((CURRENT_LATCHES.MDR & 0xFF00) >> 8);
      } else{
          MEMORY[CURRENT_LATCHES.MAR>>1][(CURRENT_LATCHES.MAR & 0x0001)] = Low16bits(CURRENT_LATCHES.MDR & 0x00FF);
      }
  }

  if( GetLD_MAR(CURRENT_LATCHES.MICROINSTRUCTION) ) NEXT_LATCHES.MAR = BUS;
  if( GetLD_MDR(CURRENT_LATCHES.MICROINSTRUCTION) ) {
    if( GetMIO_EN(CURRENT_LATCHES.MICROINSTRUCTION) ){
        if(CURRENT_LATCHES.READY) {
            if(GetDATA_SIZE(CURRENT_LATCHES.MICROINSTRUCTION)){
                NEXT_LATCHES.MDR = MEMORY[CURRENT_LATCHES.MAR>>1][0] & 0x00FF;
                NEXT_LATCHES.MDR |= ( (MEMORY[CURRENT_LATCHES.MAR>>1][1] & 0x00FF) << 8);
            } else{
                NEXT_LATCHES.MDR = SEXT( (MEMORY[CURRENT_LATCHES.MAR>>1][(CURRENT_LATCHES.MAR & 0x0001)] & 0x00FF),8 );
            }
            
        }
    } else {
        if(GetDATA_SIZE(CURRENT_LATCHES.MICROINSTRUCTION)) NEXT_LATCHES.MDR = BUS;
        else NEXT_LATCHES.MDR = (BUS & 0x00FF);
    }
  }
  if( GetLD_IR(CURRENT_LATCHES.MICROINSTRUCTION) ) {
    NEXT_LATCHES.IR = BUS;
    int inst = Low16bits(NEXT_LATCHES.IR >> 12);
    // printf("INSTRUCTION: ");
    // switch (inst){
    //     case 1 : printf("ADD\n"); break;
    //     case 5 : printf("AND\n"); break;
    //     case 0 : printf("BR\n"); break;
    //     case 12 : printf("JMP\n"); break;
    //     case 4 : printf("JSR\n"); break;
    //     case 2 : printf("LDB\n"); break;
    //     case 6 : printf("LDW\n"); break;
    //     case 14 : printf("LEA\n"); break;
    //     case 8 : printf("RTI\n"); break;
    //     case 13 : printf("SHF\n"); break;
    //     case 3 : printf("STB\n"); break;
    //     case 7 : printf("STW\n"); break;
    //     case 15 : printf("TRAP\n"); break;
    //     case 9 : printf("XOR\n"); break;
    // }

  }
  

  if( GetLD_BEN(CURRENT_LATCHES.MICROINSTRUCTION) )
    NEXT_LATCHES.BEN = ( ((CURRENT_LATCHES.IR & 0x0800) && CURRENT_LATCHES.N) ||
                         ((CURRENT_LATCHES.IR & 0x0400) && CURRENT_LATCHES.Z) ||
                         ((CURRENT_LATCHES.IR & 0x0200) && CURRENT_LATCHES.P) );
  if( GetLD_PC(CURRENT_LATCHES.MICROINSTRUCTION) ){
    switch( GetPCMUX(CURRENT_LATCHES.MICROINSTRUCTION) ){
        int OP1;
        int OP2;
        case 0: NEXT_LATCHES.PC = CURRENT_LATCHES.PC + 2; break;
        case 1: NEXT_LATCHES.PC = BUS; break;
        case 2: 
            switch( GetADDR2MUX(CURRENT_LATCHES.MICROINSTRUCTION) ){
                case 0: OP1 = 0; break;
                case 1: OP1 = Low16bits(SEXT((CURRENT_LATCHES.IR & 0x003F),6)); break;
                case 2: OP1 = Low16bits(SEXT((CURRENT_LATCHES.IR & 0x01FF),9));break;
                case 3: OP1 = Low16bits(SEXT((CURRENT_LATCHES.IR & 0x07FF),11));break;
            }
            switch( GetADDR1MUX(CURRENT_LATCHES.MICROINSTRUCTION) ){

                case 0: OP2 = CURRENT_LATCHES.PC; break;
                case 1:
                    if(GetSR1MUX0(CURRENT_LATCHES.MICROINSTRUCTION)) OP2 = CURRENT_LATCHES.REGS[(CURRENT_LATCHES.IR>>6) & 0x0007];
                    else OP2 = CURRENT_LATCHES.REGS[(CURRENT_LATCHES.IR>>9) & 0x0007];
                    break;
            }

            if(GetLSHF1(CURRENT_LATCHES.MICROINSTRUCTION)) OP1 = Low16bits(OP1 << 1);

            NEXT_LATCHES.PC = Low16bits(OP1 + OP2);

            break;
    }
  }

  if( GetLD_REG(CURRENT_LATCHES.MICROINSTRUCTION) ){
    int DR;
    if(GetDRMUX1(CURRENT_LATCHES.MICROINSTRUCTION)) DR = 6;
    else if(GetDRMUX0(CURRENT_LATCHES.MICROINSTRUCTION)) DR = 7;
    else DR = Low16bits( (CURRENT_LATCHES.IR >> 9) & 0x0007);
    NEXT_LATCHES.REGS[DR] = BUS;
  }
  
  if( GetLD_CC(CURRENT_LATCHES.MICROINSTRUCTION) ) setCC(BUS);

  if( GetLD_TEMPPC(CURRENT_LATCHES.MICROINSTRUCTION) ){
    NEXT_LATCHES.TEMP_PC = CURRENT_LATCHES.PC;
  }

  if( GetLD_USP(CURRENT_LATCHES.MICROINSTRUCTION) ){
    NEXT_LATCHES.USP = CURRENT_LATCHES.REGS[6];
  }

  if( GetLD_SSP(CURRENT_LATCHES.MICROINSTRUCTION) ){
    NEXT_LATCHES.SSP = CURRENT_LATCHES.REGS[6];
  }

  if( GetLD_VECTOR(CURRENT_LATCHES.MICROINSTRUCTION) ){
    NEXT_LATCHES.VECTOR = 0x0200;
    int val;
    if(GetVECTORMUX(CURRENT_LATCHES.MICROINSTRUCTION))
        val = 0x0001;
    else
        val = CURRENT_LATCHES.EXCV;
    val = val << 1;
    NEXT_LATCHES.VECTOR |= val;
  }

  if( GetLD_PRIV(CURRENT_LATCHES.MICROINSTRUCTION) ){
    if(GetPRIV(CURRENT_LATCHES.MICROINSTRUCTION)){
        NEXT_LATCHES.PSR = Low16bits(CURRENT_LATCHES.PSR |= 0x8000);
    }
    else 
        NEXT_LATCHES.PSR = Low16bits(CURRENT_LATCHES.PSR &= 0x7FFF);
  }

  if( GetLD_PSR(CURRENT_LATCHES.MICROINSTRUCTION) ){
    NEXT_LATCHES.PSR = BUS;
    NEXT_LATCHES.N = (NEXT_LATCHES.PSR >> 2) & 0x0001;
    NEXT_LATCHES.Z = (NEXT_LATCHES.PSR >> 1) & 0x0001;
    NEXT_LATCHES.P = NEXT_LATCHES.PSR & 0x0001;
  }

  int PROT, UNALI, UNKWNOP;

  if( (((NEXT_LATCHES.PC >> 12) & 0x000F) < 0x3 || ((NEXT_LATCHES.MAR >> 12) & 0x000F) < 0x3) && (CURRENT_LATCHES.PSR>>15)) {
    PROT = 1;
  }
  else PROT = 0;
  if( ((CURRENT_LATCHES.STATE_NUMBER == 7) || CURRENT_LATCHES.STATE_NUMBER == 6) & (NEXT_LATCHES.MAR&0x0001) )
      UNALI = 1;
  else
      UNALI = 0;

  if( ((NEXT_LATCHES.IR >> 12) & 0x000F) == 0xB || ((NEXT_LATCHES.IR >> 12) & 0x000F) == 0xA)
      UNKWNOP = 1;
  else
      UNKWNOP = 0;
  
  //printf("PROT: %i\nOP: %i\nUNALIGNED: %i\n",PROT,UNKWNOP,UNALI);
  NEXT_LATCHES.EXC = (PROT || UNALI || UNKWNOP);

  if(PROT)
    NEXT_LATCHES.EXCV = 0x02;
  else if(UNALI)
    NEXT_LATCHES.EXCV = 0x03;
  else if(UNKWNOP)
    NEXT_LATCHES.EXCV = 0x04;

}
