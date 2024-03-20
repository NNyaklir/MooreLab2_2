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
      
    }
    if(strcmp(*first,"LW")){
      
    }
    //second set of instructions store byte and store word
    if(strcmp(*first,"SB")){
      
    }
    if(strcmp(*first,"SW")){
      
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
      int RD = findRegister(tokens[1]);
      int RS1 = findRegister(tokens[2]);
  
      int imm;
      sscanf(tokens[3],"%d",imm);
      r[RD]=r[RS1]+imm;
      return 1;
    }
    if(strcmp(*first,"SUB")){
      int RD = findRegister(tokens[1]);
      int RS1 = findRegister(tokens[2]);
      int RS2 = findRegister(tokens[3]);

      r[RD]=r[RS1]-r[RS2];
      return 1;
      
      
    }
    // fourth set of instructions exclusive or, exclusive or immediate, shift left immediate, and shift right immediate
    if(strcmp(*first,"XOR")){
      
    }
    if(strcmp(*first,"XORI")){
      
    }
    if(strcmp(*first,"SLLI")){
      
    }
    if(strcmp(*first,"SRLI")){
      
    }
    //fifth set of instructions  move immediate, load immediate, negate, ones compliment
    if(strcmp(*first,"MV")){
      
    }
    if(strcmp(*first,"LI")){
      
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

    interpret(buffer);
    i++;
  }

  /* --- Your code ends here. --- */

  close_file();
  free(buffer);

  return 0;
}
