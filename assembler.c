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
} InstructionDef;

InstructionDef instruction_table[] = {
    {"HALT", 0, 'B'},  {"NOP", 1, 'B'},   {"ADD", 2, 'A'},   {"SUB", 3, 'A'},
    {"AND", 4, 'A'},   {"OR", 5, 'A'},    {"XOR", 6, 'A'},   {"NOT", 7, 'A'},
    {"MOV", 8, 'A'},   {"LOADI", 9, 'B'}, {"SHL", 10, 'B'},  {"SHR", 11, 'B'},
    {"LOAD", 12, 'B'}, {"STORE", 13, 'B'},{"JMP", 14, 'B'},  {"BEQ", 15, 'B'},
    {"BNE", 16, 'B'},  {"BLT", 17, 'B'},  {"BGT", 18, 'B'},  {"ADDI", 19, 'B'},
    {"MUL", 20, 'A'},  {"JAL", 21, 'B'},  {"JR", 22, 'B'},
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

        int rd = parse_operand(rd_tok);
        int rs1 = parse_operand(rs1_tok);
        int rs2 = parse_operand(rs2_tok);

        // writing the encode for each format 

        uint32_t instruction;
        if(def->format == 'A')  { instruction = ENCODE_A(def->opcode, rd, rs1, rs2); } 
        else { instruction = ENCODE_B(def->opcode, rd, rs1, rs2); }

        // TODO: write it out with:
        fwrite(&instruction, sizeof(uint32_t), 1, output_file);
    }

    fclose(input_file);
    fclose(output_file);
    return 0;
}