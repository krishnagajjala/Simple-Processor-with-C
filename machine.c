#include "machine.h"
#include <stdio.h>
int valid(Wrd instruction);



/*This programs function is to take instructions for a fictional
processor called N2 in 32-bit words and splits it up using bit
masking to get sections of its instruction format, such as opcode, 
extension, registers, and memory adress. These segments are then
used to both print and check the instructions validity.Validity 
for extensions,registers, and memory adress are determined by
which Opcode is being used in the instruction. The overall goal
of this program is to determine if a given array of instructions
had valid instructions and make sense in terms of register 
assignments..*/

/*Using masks to get the specific bits on instructions
opcode is 5 bits, exentions is 3 bits, then three registers
take up 9 bits, and mem address takes up 15 bits, so the 
corresponding masks are created.*/
#define mask_extension 0x07000000
#define mask_reg1 0x00e00000
#define mask_reg2 0x001c0000
#define mask_reg3 0x00038000
#define mask_address 0x00007fff

/*Prints one line of with OpCode, extensions, registers, and memory address 
used based on given 32 bit instructions. Returns 0 if invalid instructions*/

unsigned short print_instruction(Wrd instruction) {
  
  /*No need mask for opcode since its bits are first left in instructions*/
  Op_code my_opcode = instruction>>27;
  char extension = (instruction & mask_extension)>>24;
  char reg1 = (instruction & mask_reg1)>>21;
  char reg2 = (instruction & mask_reg2)>>18;
  char reg3 = (instruction & mask_reg3)>>15;
  short mem_address = instruction & mask_address;
  
  if(valid(instruction) == 0)
    return 0;
  


  if (my_opcode == PLUS) 
    printf("plus\tR%d\tR%d\tR%d", reg1, reg2, reg3);
  else if (my_opcode == MINUS) 
    printf("minus\tR%d\tR%d\tR%d", reg1, reg2, reg3);
  else if (my_opcode == TIMES) 
    printf("times\tR%d\tR%d\tR%d", reg1, reg2, reg3);
  else if (my_opcode == DIV)
    printf("div\tR%d\tR%d\tR%d", reg1, reg2, reg3);
  else if (my_opcode == MOD)
    printf("mod\tR%d\tR%d\tR%d", reg1, reg2, reg3);
  else if (my_opcode == NEG)
    printf("neg\tR%d\tR%d", reg1, reg2);
  else if (my_opcode == ABS)
    printf("abs\tR%d\tR%d", reg1, reg2);
  else if (my_opcode == SHL)
    printf("shl\tR%d\tR%d\tR%d", reg1, reg2, reg3);
  else if (my_opcode == SHR)
    printf("shr\tR%d\tR%d\tR%d", reg1, reg2, reg3);
  else if (my_opcode == BAND)
    printf("band\tR%d\tR%d\tR%d", reg1, reg2, reg3);
  else if (my_opcode == BOR)
    printf("bor\tR%d\tR%d\tR%d", reg1, reg2, reg3);
  else if (my_opcode == BXOR)
    printf("bxor\tR%d\tR%d\tR%d", reg1, reg2, reg3);
  else if (my_opcode == BNEG)
    printf("bneg\tR%d\tR%d", reg1, reg2);
  else if (my_opcode == AND)
    printf("and\tR%d\tR%d\tR%d", reg1, reg2, reg3);
  else if (my_opcode == OR)
    printf("or\tR%d\tR%d\tR%d", reg1, reg2, reg3);
  else if (my_opcode == NOT)
    printf("not\tR%d\tR%d", reg1, reg2);
  else if (my_opcode == EQ)
    printf("eq\tR%d\tR%d\t%05d", reg1, reg2, mem_address);
  else if (my_opcode == NEQ)
    printf("neq\tR%d\tR%d\t%05d", reg1, reg2, mem_address);
  else if (my_opcode == LE)
    printf("le\tR%d\tR%d\t%05d", reg1, reg2, mem_address);
  else if (my_opcode == LT)
    printf("lt\tR%d\tR%d\t%05d", reg1, reg2, mem_address);
  else if (my_opcode == GE)
    printf("ge\tR%d\tR%d\t%05d", reg1, reg2, mem_address);
  else if (my_opcode == GT)
    printf("gt\tR%d\tR%d\t%05d", reg1, reg2, mem_address);
  else if (my_opcode == MOVE)
    printf("move\tR%d\tR%d", reg1, reg2);
  else if (my_opcode == LW)
    printf("lw\tR%d\t%05d", reg1, mem_address);
  else if (my_opcode == SW)
    printf("sw\tR%d\t%05d", reg1, mem_address);
  else if (my_opcode == LI) {
    /*This prints immediate not mem address so not %05d*/
    printf("li\tR%d\t%d", reg1, mem_address);
  } else if (my_opcode == SYS) {
    if (extension == 4)
      printf("sys\t%d", extension);
    else
      printf("sys\t%d\tR%d", extension, reg1);
  }

  return 1;

}

/*Helper method that determines the validity of an instructions based on
opcodes, registers, and memory address used being valid. Returns 0 if 
instructions are invalid in any of those ways. */
int valid(Wrd instruction) {

  Op_code my_opcode = instruction>>27;
  char extension = (instruction & mask_extension)>>24;
  char reg1 = (instruction & mask_reg1)>>21;
  char reg2 = (instruction & mask_reg2)>>18;
  char reg3 = (instruction & mask_reg3)>>15;
  short mem_address = instruction & mask_address;
  
  /*Checking validity according to fictional N2 instruction
   requirements*/
  if (my_opcode>26)
      return 0;
  else if (my_opcode == SYS && (extension>4  
             ||(reg1 == 6 && (extension == 2 || extension == 0))))
    return 0;
  else if (my_opcode == LI && reg1>5)
    return 0;
  else if (my_opcode == SW && (reg1 > 6 || ((mem_address % 4) != 0)))
    return 0;
  else if (my_opcode == LW && (reg1 > 5 || ((mem_address % 4) != 0)))
    return 0;
  else if (my_opcode == MOVE && (reg1 > 5 || reg2 > 6))
    return 0;
  else if (my_opcode >= EQ && my_opcode <= GT  
	  && (reg1 > 6 || reg2 > 6 || ((mem_address % 4) != 0)))
    return 0;
  else if ((my_opcode == NOT || my_opcode == BNEG || my_opcode == ABS 
    || my_opcode == NEG) && (reg1 > 5 || reg2 > 6))
    return 0;
  else if (reg1 > 5 || reg2 > 6|| reg3 > 6)
    return 0;

  return 1;
}

/*Takes a program, array of instructions, and prints them all using print
instructions function after checking all instructions are valid beforehand.
Returns 0 if any instructions are invalid and changes pointer location 
value to array element value where instruction was invalid.*/
unsigned short disassemble(const Wrd program[], unsigned short num_instrs,
                           unsigned short *const bad_instr){
  int i, j;
  int address = 0;

  if (program == NULL || bad_instr == NULL || num_instrs > NUM_WORDS)
    return 0;
  
  /*Validity check of each instruction*/
  for (i = 0;i < num_instrs; i++) {
    if (valid(program[i]) == 0) {
      *bad_instr = i;
      return 0;
    }
  }

  /*Print of each instruction*/
  for (j = 0;j < num_instrs; j++) {
    printf("%04x: ", address);
    address += 4;
    print_instruction(program[j]);
    printf("\n");
  }
  return 1;

}

/*Takes a program, array of instructions, and checks whether the given
register is properly assigned a value before it is used in the program.
Returns -1 if register was not properly set beforehand. */
short where_set(const Wrd program[], unsigned short num_words,
                unsigned short reg_nbr){
  int i;
  char reg1, extension;
  Op_code my_opcode;

  if (program == NULL ||  num_words > NUM_WORDS|| reg_nbr > 5)
    return -1;
  
  /*Checks for all cases where register was set prior to being used*/
  for (i = 0;i < num_words; i++) {
    reg1 = (program[i] & mask_reg1)>>21;
    my_opcode = program[i]>>27;
    extension = (program[i] & mask_extension)>>24;
    
    /*Checks for any modifying instructions and if reg1 is the given
     register, to indicate it is being set*/
    if (reg1 == reg_nbr && ((my_opcode >= PLUS &&  my_opcode <= NOT )
       || my_opcode == MOVE || my_opcode == LW || my_opcode == LI
       || (my_opcode == SYS && (extension == 2 || extension == 0))))
      return i;
  }

  return -1;

}

/*Takes a program, array of instructions, and checks whether the given
program has valid instructions and that each instruction has registers
used that are assigned a value before its use,using the where set
function */
unsigned short is_safe(const Wrd program[], unsigned short pgm_size,
                       unsigned short *const bad_instr){
  int i;
  char reg1, reg2, reg3;
  Op_code my_opcode;
  char extension;

  if (program == NULL || bad_instr == NULL || pgm_size > NUM_WORDS)
    return 0;

  for (i = 0;i < pgm_size; i++) {
    reg1 = (program[i] & mask_reg1)>>21;
    reg2 = (program[i] & mask_reg2)>>18;
    reg3 = (program[i] & mask_reg3)>>15;
    my_opcode = program[i]>>27;
    extension = (program[i] & mask_extension)>>24;

    /*Checks register being used in instructions to determine if it has
      been set before by calling whereset on every register that is used
      by the instruction based on the opCode. Dont have to check 1st 
      register if its modifying opcode, */
    if (valid(program[i]) == 0) {
      *bad_instr = i;
      return 0;
    } else if (my_opcode == SYS && ((extension == 1 || extension == 3)
				    && where_set(program, i, reg1) == -1)) {
      *bad_instr = i;
      return 0;
    } else if ((my_opcode == MOVE|| my_opcode == NOT 
	       || my_opcode == BNEG || my_opcode == ABS || my_opcode == NEG) 
	       && where_set(program, i, reg2) == -1){
      *bad_instr = i;
      return 0;
    } else if(my_opcode == SW && where_set(program, i, reg1) == -1){
      *bad_instr = i;
      return 0;
    } else if(my_opcode >= EQ && my_opcode <= GT 
	      && (where_set(program, i, reg1) == -1 
		  || where_set(program, i, reg2) == -1)) {
      *bad_instr = i;
      return 0;
    } else if(my_opcode < NOT && (where_set(program, i, reg2) == -1 
				  || where_set(program, i, reg3)) == -1){
      *bad_instr = i;
      return 0;
    }
  }

  return 1;

  

}
