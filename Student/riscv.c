#include <errno.h>   //errno
#include <stdint.h>  //uintptr_t
#include <stdio.h>   //printf() fprintf()
#include <stdlib.h>  //malloc() & free()
#include <string.h>  //strerror()

#include "my_string.h"
#include "process_file.h"
#include "tokenizer.h"

#define N_REGISTERS ((size_t)32)
#define MEM_SIZE ((size_t)1024)

uintptr_t r[N_REGISTERS];
uintptr_t pc;
unsigned char mem[MEM_SIZE];

static int print_registers(char *fd_name){
  FILE *fptr;

  if((fptr= fopen(fd_name,"w"))==NULL) return 1;
  
  //print all registers
  fprintf(fptr, "Registers:\n");
  for (size_t i=0; i<N_REGISTERS;i++){
    fprintf(fptr, "X[%zu] = %u\n", i, r[i]);
  }
  return 0;
}

static void error_no_memory(void) {
  fprintf(stderr, "No more memory available. Error: %s\n", strerror(errno));
}

void init_memory_elements(void) {
  // Initialize registers
  for (size_t i = 0; i < N_REGISTERS; i++) r[i] = ((uintptr_t)0);

  // Initialize PC to 0
  pc = ((uintptr_t)0);

  // Initialize all memory to 0
  for (size_t i = 0; i < MEM_SIZE; ++i) mem[i] = ((unsigned char)0);

  // Set sp to be the top part of the memory
  r[2] = (uintptr_t)&mem[MEM_SIZE];
}

int findRegister(char *section){
  if (strcmp(section,"0")) return 0;
  if (strcmp(section,"ZERO")) return 0;
  if (strcmp(section,"RA")) return 1;
  if (strcmp(section,"SP")) return 2;
  if (strcmp(section,"GP")) return 3;
  if (strcmp(section,"TP")) return 4;
  if (strcmp(section,"T0")) return 5;
  if (strcmp(section,"T1")) return 6;
  if (strcmp(section,"T2")) return 7;
  if (strcmp(section,"S0")) return 8;
  if (strcmp(section,"FP")) return 8;
  if (strcmp(section,"S1")) return 9;
  if (strcmp(section,"A0")) return 10;
  if (strcmp(section,"A1")) return 11;
  if (strcmp(section,"A2")) return 12;
  if (strcmp(section,"A3")) return 13;
  if (strcmp(section,"A4")) return 14;
  if (strcmp(section,"A5")) return 15;
  if (strcmp(section,"A6")) return 16;
  if (strcmp(section,"A7")) return 17;
  if (strcmp(section,"S2")) return 18;
  if (strcmp(section,"S3")) return 19;
  if (strcmp(section,"S4")) return 20;
  if (strcmp(section,"S5")) return 21;
  if (strcmp(section,"S6")) return 22;
  if (strcmp(section,"S7")) return 23;
  if (strcmp(section,"S8")) return 24;
  if (strcmp(section,"S9")) return 25;
  if (strcmp(section,"S10")) return 26;
  if (strcmp(section,"S11")) return 27;
  if (strcmp(section,"T3")) return 28;
  if (strcmp(section,"T4")) return 29;
  if (strcmp(section,"T5")) return 30;
  if (strcmp(section,"T6")) return 31;
  else {return -1;}
}

//this is to check if an imm is by itself or from a register.
int checkPar(char *parcheck){
  for (size_t i=0;i<str_len(parcheck)-1;i++){
    if(strcmp(i,"(")){
      return 1;
    }
  }
  return 0;
}

int ADDI(char **tokens){
  int RD = findRegister(tokens[1]);
  if (checkPar(tokens[2])){
  char **ADtok = tokenize(tokens[2],"()");
  int imm;
  int RS1 = findRegister(ADtok[0]);

  sscanf(ADtok[1],"%d",&imm);
  r[RD]=mem[r[RS1]+imm];
  return 1;
  }
  else{
    int RS1 = findRegister(tokens[2]);

    int imm;
    sscanf(tokens[3],"%d",&imm);
    r[RD]=mem+r[RS1]+imm;
    return 1;

  }
}

int SUB(char **tokens){
  int RD = findRegister(tokens[1]);
  int RS1 = findRegister(tokens[2]);
  int RS2 = findRegister(tokens[3]);

  r[RD]=r[RS1]-r[RS2];
  return 1;
  }
  
int XORI(char **tokens){
  int RD = findRegister(tokens[1]);
  int RS1 = findRegister(tokens[2]);
  int RS2 = findRegister(tokens[3]);

  r[RD] = r[RS1]^r[RS2];
  return 1;
}
  
int JAL( char **tokens){
  int RD = findRegister(tokens[1]);
  int imm;
  sscanf(tokens[2],"%d",imm);

  r[RD] = (pc+imm);
  pc += imm;
  return 1;
}

int JALR( char **tokens){
  int RD = findRegister(tokens[1]);
  int RS1 = findRegister(tokens[2]);
  int imm;
  sscanf(tokens[3],"%d",&imm);

  r[RD] = (pc+imm);
  pc = mem[r[RS1]] + imm;
  return 1;
}

/**
 * Fill out this function and use it to read interpret user input to execute
 * RV32 instructions. You may expect that a single, properly formatted RISC-V
 * instruction string will be passed as a parameter to this function.
 */
int interpret(char *instr) {
  // TODO:
  char **tokens= tokenize(instr, " ,"); //tokenizes the instruction
  char *first = tokens[0]; //gets the initial inistruction


    //first set of instructions load byte and load word
    if(strcmp(*first,"LB")){
      if (checkPar(tokens[2])){
        char **LBtok = tokenize(tokens[2],"()");
        int RD = findRegister(tokens[1]);
        int IMM; 
        sscanf(LBtok[1],"%d",&IMM);
        int RS1 = findRegister(LBtok[0]);

        r[RD] = mem[r[RS1] + IMM];
        return 1;
      }
      else{
        int RD= findRegister(tokens[1]);
        int RS1= findRegister(tokens[2]);
        int IMM;
        sscanf(tokens[3],"%d",&IMM);

        r[RD] = mem[r[RS1] + IMM];
        return 1;
      }
    }

    if(strcmp(*first,"LW")){
      if (checkPar(tokens[2])){
        char **LWtok = tokenize(tokens[2],"()");
        int RD = findRegister(tokens[1]);
        int IMM; 
        sscanf(LWtok[0],"%d",&IMM);
        int RS1 = findRegister(LWtok[1]);

        r[RD] = mem[r[RS1] + IMM];
        return 1;
      }
      else{
        int RD= findRegister(tokens[1]);
        int RS1= findRegister(tokens[2]);
        int IMM;
        sscanf(tokens[3],"%d",&IMM);

        r[RD] = mem[r[RS1] + IMM];
        return 1;
      }
    }
    //second set of instructions store byte and store word
    if(strcmp(*first,"SB")){
      if (checkPar(tokens[2])){
        char **SBtok = tokenize(tokens[2],"()");
        int RD = findRegister(tokens[1]);
        int IMM; 
        sscanf(SBtok[0],"%d",&IMM);
        int RS1 = findRegister(SBtok[1]);

        
        mem[r[RD]+IMM]=r[RS1];
        return 1;
      }
      else{
        int RD= findRegister(tokens[1]);
        int RS1= findRegister(tokens[2]);
        int IMM;
        sscanf(tokens[3],"%d",&IMM);

        mem[r[RD]+IMM]=r[RS1];
        return 1; 
      } 
    }

    if(strcmp(*first,"SW")){
      if (checkPar(tokens[2])){
        char **SWtok = tokenize(tokens[2],"()");
        int RD = findRegister(tokens[1]);
        int IMM; 
        sscanf(SWtok[0],"%d",&IMM);
        int RS1 = findRegister(SWtok[1]);

        mem[r[RD]+IMM]=r[RS1];
        return 1;
      }
      else{
        int RD= findRegister(tokens[1]);
        int RS1= findRegister(tokens[2]);
        int IMM;
        sscanf(tokens[3],"%d",&IMM);

        mem[r[RD]+IMM]=r[RS1];
        return 1;
    }
    }

    //third set of insctructions add, add immediate, and sub
    if(strcmp(*first,"ADD")){
      int RD = findRegister(tokens[1]);
      int RS1 = findRegister(tokens[2]);
      int RS2 = findRegister(tokens[3]);

      r[RD]=r[RS1]+r[RS2];
      return 1;
      
    }
    if(strcmp(*first,"ADDI")){
      return ADDI(tokens);
    }
    if(strcmp(*first,"SUB")){
      return SUB(tokens);
      
    }
    // fourth set of instructions exclusive or, exclusive or immediate, shift left immediate, and shift right immediate
    if(strcmp(*first,"XOR")){
      int RD = findRegister(tokens[1]);
      int RS1 = findRegister(tokens[2]);
      int RS2 = findRegister(tokens[3]);

      r[RD] = r[RS1]^r[RS2];
      return 1;
      
    }
    if(strcmp(*first,"XORI")){
      return XORI(tokens);
      
    }
    if(strcmp(*first,"SLLI")){
      int RD = findRegister(tokens[1]);
      int RS1 = findRegister(tokens[2]);
      
      int imm;
      sscanf(tokens[3],"%d",&imm);

      r[RD] = mem[r[RS1]<<imm];
      //r[RD] = r[RS1]<<imm;
      return 1;
      
    }
    if(strcmp(*first,"SRLI")){
      int RD = findRegister(tokens[1]);
      int RS1 = findRegister(tokens[2]);
      
      int imm;
      sscanf(tokens[3],"%d",&imm);

      r[RD] = mem[r[RS1]>>imm];
      //r[RD] = r[RS1]>>imm;
      return 1;
      
    }
    //fifth set of instructions  move immediate, load immediate, negate, ones compliment
    if(strcmp(*first,"MV")){
      char **additoks = malloc(sizeof(tokens)*4);
      additoks[1]=tokens[1];
      additoks[2]; 
      additoks[3];

      int RD= findRegister(tokens[1]);
      int RS1 = findRegister(tokens[2]);

      r[RD] = r[RS1];
      return 1;
    }
    if(strcmp(*first,"LI")){
      char **additoks = malloc(sizeof(tokens)*4);
      additoks[1]=tokens[1];
      additoks[2]= "0";
      additoks[3]=tokens[2];
      return ADDI(additoks);
      
    }
    if(strcmp(*first,"NED")){
      char **subitoks = malloc(sizeof(tokens)*4);
      subitoks[1]= tokens[1];
      subitoks[2]= "0";
      subitoks[3]= tokens[2];
      return SUB(subitoks);
      
    }
    if(strcmp(*first,"NOT")){
      char **xoritoks = malloc(sizeof(tokens)*4);
      xoritoks[1] = tokens[1];
      xoritoks[2] = tokens[2];
      xoritoks[3] = "-1";

      return XORI(xoritoks);
    }
    //sixth set of instructions jump and link, and jump
    if(strcmp(*first,"JAL")){
      JAL(tokens);

    }
    if(strcmp(*first,"J")){
      char **jaltoks = malloc(sizeof(tokens)*4);
      jaltoks[1]= "0";
      jaltoks[2]= tokens[1];

      JAL(jaltoks);
    }
    //seventh set of instructions,jump offset, and register
    if(strcmp(*first,"JALR")){
      JALR(tokens);
    }
    if(strcmp(*first,"JR")){
      char **jalrtoks = malloc(sizeof(tokens)*4);
      jalrtoks[1]= "0";
      jalrtoks[2]= tokens[1];
      jalrtoks[3]= "0";

      JALR(jalrtoks);
      return 1;
    }

  return 1;
}

int main(int argc, char **argv) {
  FILE *file;
  char *buffer;

  if (argc != 3) {
    fprintf(stderr, "Only three parameters must be passed where argv[1]=file name and"
    "with the assembly instructions and argv[2]=filename to same the registers status\n");
    return 1;
  }

  if (process_file(argv[1])) return 1;
  if (open_file()) return 1;

  // Initialize PC, registers and memory
  init_memory_elements();

  buffer = (char *)malloc((LINE_SIZE + 1) * sizeof(char));

  if (buffer == NULL) {
    close_file();
    return 1;
  }

  /* --- Your code starts here. --- */

  // For each line in the file, interpret it.
  // NOTE: Use get_line() function in process_file.h
  int i=0;
  while(1){
    get_line(buffer, i); 
    //check to see if there is something in the buffer, first character 
    //if there is nothing \0, break loop
    if (strcmp(buffer[0],"\0")){
      break;
    }

    interpret(buffer);
    i++;
  }

  /* --- Your code ends here. --- */

  close_file();
  free(buffer);

  return print_registers(argv[2]);
}

