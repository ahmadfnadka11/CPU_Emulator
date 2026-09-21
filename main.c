#include <stdio.h>
#include <stdint.h>

#define NUM_REGS 16
#define MEM_SIZE 1024

typedef struct {
    uint32_t regs[NUM_REGS];
    uint32_t pc;
    uint32_t memory[MEM_SIZE];
    int halted;
} CPU;







void cpu_init(CPU *cpu) {
    for (int i = 0; i < NUM_REGS; i++){
        cpu->regs[i] = 0; 
    }
    for (int i = 0; i < MEM_SIZE; i++){
        cpu->memory[i] = 0;
    }
    cpu->halted = 0;
    cpu->pc = 0; 
}

void cpu_step(CPU *cpu) {
    // FETCHing instruction
    uint32_t instruction = cpu->memory[cpu->pc];


    // DECODEing instrcution 
    uint32_t opcode = (instruction >> 27 ) & 0x1F;


    // Getting the registers, same for format A or B 
    uint32_t rd = (instruction >> 23 ) & 0xF;
    uint32_t rs1 = (instruction >> 19 ) & 0xF;

    // Format A - getting third register
    uint32_t rs2 = (instruction >> 15 ) & 0xF;

    // Format B - getting imm field
    uint32_t imm = instruction & 0x7FFFF;


    //printf("PC=%u opcode=%u rd=%u rs1=%u rs2=%u imm=%u\n", cpu->pc, opcode, rd, rs1, rs2, imm);
    // Execute
    switch (opcode){
        //HALT
        case 0:
            cpu->halted = 1;
            break;
        //NOP
        case 1:
            break;
        //ADD
        case 2:
            cpu->regs[rd] = cpu->regs[rs1] + cpu->regs[rs2];
            break;
        //SUB
        case 3:
            cpu->regs[rd] = cpu->regs[rs1] - cpu->regs[rs2];
            break;
        //AND
        case 4:
            cpu->regs[rd] = cpu->regs[rs1] & cpu->regs[rs2];
            break;
        //OR
        case 5:
            cpu->regs[rd] = cpu->regs[rs1] | cpu->regs[rs2];
            break;
        //XOR
        case 6:
            cpu->regs[rd] = cpu->regs[rs1] ^ cpu->regs[rs2];
            break;
        //NOT
        case 7:
            cpu->regs[rd] = ~cpu->regs[rs1];
            break;
        //MOV
        case 8: 
            cpu->regs[rd] = cpu->regs[rs1];
            break;
        //LOADI
        case 9: 
            cpu->regs[rd] = imm;
            break;
        // SHL
        case 10: 
            cpu->regs[rd] = cpu->regs[rs1] << imm;  // you had: cpu->regs[rd] << imm;  (no assignment, and shifts rd instead of rs1)
            break;
        // SHR
        case 11: 
            cpu->regs[rd] = cpu->regs[rs1] >> imm;
            break;
        //LOAD
        case 12:
            cpu->regs[rd] = cpu->memory[cpu->regs[rs1] + imm];
            break;
        //STORE
        case 13:
            cpu->memory[cpu->regs[rs1] + imm] = cpu->regs[rd];
            break;
        //JMP
        case 14:
            cpu->pc = imm;
            return;
        //BEQ
        case 15:
            if(cpu->regs[rd] == cpu->regs[rs1]) {
                cpu->pc = imm;
                return;
            }
            break;
        //BNE
        case 16:
            if(cpu->regs[rd] != cpu->regs[rs1]) {
                cpu->pc = imm;
                return;
            }
            break;
        //BLT
        case 17:
            if(cpu->regs[rd] > cpu->regs[rs1]) {
                cpu->pc = imm;
                return;
            }
            break;
        //BGT
        case 18:
            if(cpu->regs[rd] < cpu->regs[rs1]) {
                cpu->pc = imm;
                return;
            }
            break;
        //ADDI
        case 19:
            cpu->regs[rd] = cpu->regs[rs1] + imm;
            break;
        //MUL
        case 20:
            cpu->regs[rd] = cpu->regs[rs1] * cpu->regs[rs2];
            break;
        //JAL
        case 21:
            cpu->regs[rd] = cpu->pc + 1;
            cpu->pc = imm;
            return;
        //JR
        case 22:
            cpu->pc = cpu->regs[rs1];
            return;
    }
    
    // Advancing the pc 
    cpu->pc++;
}

#define ENCODE_A(opcode, rd, rs1, rs2) (((opcode) << 27) | ((rd) << 23) | ((rs1) << 19) | ((rs2) << 15))
#define ENCODE_B(opcode, rd, rs1, imm) (((opcode) << 27) | ((rd) << 23) | ((rs1) << 19) | ((imm) & 0x7FFFF))


int main(int argc, char *argv[]) {

    // reading the files and doing checks
    if (argc < 2) {
        printf("Usage: %s input.asm \n", argv[0]);
        return 1;
    }

    FILE *input_file = fopen(argv[1], "rb");
    if (!input_file) {
        printf("Error opening files\n");
        return 1;
    }
    
    CPU cpu;
    cpu_init(&cpu);

    
    
    size_t count = fread(cpu.memory, sizeof(uint32_t), MEM_SIZE, input_file);


    fclose(input_file);


    while (!cpu.halted) {
        cpu_step(&cpu);
    }

        for (int i = 0; i < NUM_REGS; i++) {
        printf("R%d = %u\n", i, cpu.regs[i]);
    }

    return 0;
}