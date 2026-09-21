#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define ENCODE_A(opcode, rd, rs1, rs2) (((opcode) << 27) | ((rd) << 23) | ((rs1) << 19) | ((rs2) << 15))
#define ENCODE_B(opcode, rd, rs1, imm) (((opcode) << 27) | ((rd) << 23) | ((rs1) << 19) | ((imm) & 0x7FFFF))

typedef struct {
    const char *name;
    int opcode;
    char format; // 'A' or 'B'
    char type;
} InstructionDef;

InstructionDef instruction_table[] = {
    {"HALT", 0, 'B', 'N'},  {"NOP", 1, 'B', 'N'},   {"ADD", 2, 'A', 'T'},   {"SUB", 3, 'A', 'T'},
    {"AND", 4, 'A', 'T'},   {"OR", 5, 'A', 'T'},    {"XOR", 6, 'A', 'T'},   {"NOT", 7, 'A', 'W'},
    {"MOV", 8, 'A', 'W'},   {"LOADI", 9, 'B', 'F'}, {"SHL", 10, 'B', 'D'},  {"SHR", 11, 'B', 'D'},
    {"LOAD", 12, 'B', 'D'}, {"STORE", 13, 'B', 'D'},{"JMP", 14, 'B', 'I'},  {"BEQ", 15, 'B', 'D'},
    {"BNE", 16, 'B', 'D'},  {"BLT", 17, 'B', 'D'},  {"BGT", 18, 'B', 'D'},  {"ADDI", 19, 'B', 'D'},
    {"MUL", 20, 'A', 'T'},  {"JAL", 21, 'B', 'F'},  {"JR", 22, 'B', 'R'},
};
#define NUM_INSTRUCTIONS 23

// finding the instructions from the list
InstructionDef* lookup_instruction(const char *name) {
    for (int i = 0; i < NUM_INSTRUCTIONS; i++) {
        if (strcmp(name, instruction_table[i].name) == 0) {
            return &instruction_table[i];
        }
    }
    return NULL; // not found
}

// getting rid of the R in the register name
int parse_operand(const char *token) {
    if (token[0] == 'R' || token[0] == 'r') {
        return atoi(token + 1); // skip the 'R', parse what's left
    }
    return atoi(token); // just the number
}

int main(int argc, char *argv[]) {

    // reading the files
    if (argc < 3) {
        printf("Usage: %s input.asm output.bin\n", argv[0]);
        return 1;
    }

    FILE *input_file = fopen(argv[1], "r");
    FILE *output_file = fopen(argv[2], "wb");
    if (!input_file || !output_file) {
        printf("Error opening files\n");
        return 1;
    }

    char line[256];
    while (fgets(line, sizeof(line), input_file)) {
        char *mnemonic = strtok(line, " ,\n");
        if (mnemonic == NULL) continue; // skip blank lines
        //printf("Processing: %s\n", mnemonic);   

        InstructionDef *def = lookup_instruction(mnemonic);
        if (def == NULL) {
            printf("Unknown instruction: %s\n", mnemonic);
            continue;
        }

        // parsing the line to get the tokens ready for conversion 

        char *rd_tok = strtok(NULL, " ,\n");
        char *rs1_tok = strtok(NULL, " ,\n");
        char *rs2_tok = strtok(NULL, " ,\n");

        // convert the register name to a normal number 

        int rd = 0;
        if(rd_tok != NULL){ 
             rd = parse_operand(rd_tok); 
         }

        int rs1 = 0;
        if(rs1_tok != NULL){ 
             rs1 = parse_operand(rs1_tok); 
         }

        int rs2 = 0;
        if(rs2_tok != NULL){ 
             rs2 = parse_operand(rs2_tok); 
         }
        

        // writing the encode for each format 

        uint32_t instruction;
        if(def->format == 'A')  { 
            switch(def->type){
                case 'T':
                    instruction = ENCODE_A(def->opcode, rd, rs1, rs2);
                    break;
                case 'W':
                    instruction = ENCODE_A(def->opcode, rd, rs1, 0);
                    break;
            }
         } 
        else { switch(def->type){
            case 'F':
                instruction = ENCODE_B(def->opcode, rd, 0, rs1);
                break;
            case 'D':
                instruction = ENCODE_B(def->opcode, rd, rs1, rs2);
                break;
            case 'I':
                instruction = ENCODE_B(def->opcode, 0, 0, rd);
                break;
            case 'R':
                instruction = ENCODE_B(def->opcode, 0, rd, 0);
                break;
            case 'N':
                instruction = ENCODE_B(def->opcode, 0, 0, 0);
                break;
        } }

        
        fwrite(&instruction, sizeof(uint32_t), 1, output_file);
    }

    fclose(input_file);
    fclose(output_file);
    return 0;
}