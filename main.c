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
            cpu->regs[rd] = cpu->memory[cpu->reg[rs1] + imm];
            break;
        //STORE
        case 13:
            cpu->memory[cpu->reg[rs1] + imm] = cpu->regs[rd];
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

    }
    


    // Advancing the pc 
    cpu->pc++;
}

int main() {
    CPU cpu;
    cpu_init(&cpu);

    // TODO: load a test program into cpu.memory

    while (!cpu.halted) {
        cpu_step(&cpu);
    }

    printf("Halted. Final register state:\n");
    // TODO: print registers

    return 0;
}