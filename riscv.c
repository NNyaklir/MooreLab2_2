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
  //@maria this is for your method
}

//this is to check if an imm is by itself or from a register.
int checkPar(char *parcheck){
  for (int i=0;i<str_len(parcheck)-1;i++){
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

  sscanf(ADtok[1],"%d",imm);
  r[RD]=mem[r[RS1]+imm];
  return 1;
  }
  else{
    int RS1 = findRegister(tokens[2]);

    int imm;
    sscanf(tokens[3],"%d",imm);
    r[RD]=mem[r[RS1]+imm];
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
        sscanf(LBtok[1],"%d",IMM);
        int RS1 = findRegister(LBtok[0]);

        r[RD] = mem[r[RS1] + IMM];
        return 1;
      }
      else{
        int RD= findRegister(tokens[1]);
        int RS1= findRegister(tokens[2]);
        int IMM;
        sscanf(tokens[3],"%d",IMM);

        r[RD] = mem[r[RS1] + IMM];
        return 1;
      }
    }

    if(strcmp(*first,"LW")){
      if (checkPar(tokens[2])){
        char **LWtok = tokenize(tokens[2],"()");
        int RD = findRegister(tokens[1]);
        int IMM; 
        sscanf(LWtok[0],"%d",IMM);
        int RS1 = findRegister(LWtok[1]);

        r[RD] = mem[r[RS1] + IMM];
        return 1;
      }
      else{
        int RD= findRegister(tokens[1]);
        int RS1= findRegister(tokens[2]);
        int IMM;
        sscanf(tokens[3],"%d",IMM);

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
        sscanf(SBtok[0],"%d",IMM);
        int RS1 = findRegister(SBtok[1]);

        
        mem[r[RD]+IMM]=r[RS1];
        return 1;
      }
      else{
        int RD= findRegister(tokens[1]);
        int RS1= findRegister(tokens[2]);
        int IMM;
        sscanf(tokens[3],"%d",IMM);

        mem[r[RD]+IMM]=r[RS1];
        return 1; 
      } 
    }

    if(strcmp(*first,"SW")){
      if (checkPar(tokens[2])){
        char **SWtok = tokenize(tokens[2],"()");
        int RD = findRegister(tokens[1]);
        int IMM; 
        sscanf(SWtok[0],"%d",IMM);
        int RS1 = findRegister(SWtok[1]);

        mem[r[RD]+IMM]=r[RS1];
        return 1;
      }
      else{
        int RD= findRegister(tokens[1]);
        int RS1= findRegister(tokens[2]);
        int IMM;
        sscanf(tokens[3],"%d",IMM);

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
      sscanf(tokens[3],"%d",imm);

      r[RD] = mem[r[RS1]<<imm];
      //r[RD] = r[RS1]<<imm;
      return 1;
      
    }
    if(strcmp(*first,"SRLI")){
      int RD = findRegister(tokens[1]);
      int RS1 = findRegister(tokens[2]);
      
      int imm;
      sscanf(tokens[3],"%d",imm);

      r[RD] = mem[r[RS1]>>imm];
      //r[RD] = r[RS1]>>imm;
      return 1;
      
    }
    //fifth set of instructions  move immediate, load immediate, negate, ones compliment
    if(strcmp(*first,"MV")){
      char **additoks = malloc(sizeof(tokens)*4);
      additoks[1]=tokens[1];
      additoks[2]; //???? idk
      additoks[3];
    }
    if(strcmp(*first,"LI")){
      char **additoks = malloc(sizeof(tokens)*4);
      additoks[1]=tokens[1];
      additoks[2]=r[0];
      additoks[3]=tokens[2];
      return ADDI(additoks);
      
    }
    if(strcmp(*first,"NED")){
      
    }
    if(strcmp(*first,"NOT")){
      
    }
    //sixth set of instructions jump and link, and jump
    if(strcmp(*first,"LB")){
      
    }
    if(strcmp(*first,"LB")){
      
    }
    //seventh set of instructions,jump offset, and register
    if(strcmp(*first,"LB")){
      
    }
    if(strcmp(*first,"LB")){
      
    }

  return 1;
}

int main(int argc, char **argv) {
  FILE *file;
  char *buffer;

  if (argc != 2) {
    fprintf(stderr, "Only two parameters must be passed.\n");
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

  return 0;
}
