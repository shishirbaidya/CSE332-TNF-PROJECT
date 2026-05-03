#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define DIM     "\033[2m"

#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"


typedef enum { R_TYPE, I_TYPE, J_TYPE, INVALID } InstType;


int regToInt(char *reg) {
    if (reg[0] != 'R') return -1;
    return atoi(&reg[1]);
}

int funcCode(char *op) {
    if (!strcmp(op, "OR"))  return 0;
    if (!strcmp(op, "NOR")) return 1;
    if (!strcmp(op, "AND")) return 2;
    if (!strcmp(op, "ADD")) return 3;
    if (!strcmp(op, "SUB")) return 4;
    if (!strcmp(op, "SLL")) return 5;
    if (!strcmp(op, "SRL")) return 6;
    if (!strcmp(op, "SLT")) return 7;
    return -1;
}


int opcode(char *op) {
    if (!strcmp(op, "ADDI")) return 1;
    if (!strcmp(op, "LW"))   return 2;
    if (!strcmp(op, "SW"))   return 3;
    if (!strcmp(op, "BEQ"))  return 4;
    if (!strcmp(op, "BNE"))  return 5;
    if (!strcmp(op, "J"))    return 6;
    return 0;
}


InstType getType(char *op) {
    if (funcCode(op) != -1) return R_TYPE;

    if (!strcmp(op, "ADDI") || !strcmp(op, "LW") ||
        !strcmp(op, "SW")   || !strcmp(op, "BEQ") ||
        !strcmp(op, "BNE"))
        return I_TYPE;

    if (!strcmp(op, "J")) return J_TYPE;

    return INVALID;
}



unsigned int buildR(int func, int shamt, int rd, int rt, int rs) {
    return ((func & 0x7) << 20) |
           ((shamt & 0xF) << 16) |
           ((rd & 0xF) << 12) |
           ((rt & 0xF) << 8) |
           ((rs & 0xF) << 4);
}

unsigned int buildI(int imm, int rt, int rs, int opc) {
    if (imm < 0) imm = (1 << 11) + imm;

    return ((imm & 0x7FF) << 12) |
           ((rt & 0xF) << 8) |
           ((rs & 0xF) << 4) |
           (opc & 0xF);
}

unsigned int buildJ(int addr) {
    return ((addr & 0x7FFFF) << 4) | 6;
}



int main() {
    FILE *in  = fopen("assembly.txt", "r");
    FILE *out = fopen("output.hex", "w");

    if (!in || !out) {
        printf(RED BOLD "File error!\n" RESET);
        return 1;
    }

    char line[100];
    int addr = 0;

    fprintf(out, "v2.0 raw\n");

    
    printf(BOLD CYAN "\n╔══════════════════════════════════════╗\n");
    printf("║        23-bit ISA Assembler         ║\n");
    printf("╚══════════════════════════════════════╝\n\n" RESET);

    printf(DIM "Format:\n");
    printf("R: func|sh|rd|rt|rs|0000\n");
    printf("I: imm|rt|rs|opcode\n");
    printf("J: addr|0110\n\n" RESET);

   
    while (fgets(line, sizeof(line), in)) {

        char op[10], a[10], b[10], c[10];
        unsigned int machine = 0;

        sscanf(line, "%s %s %s %s", op, a, b, c);
        InstType type = getType(op);

        printf(DIM "Addr:%02d " RESET, addr);

        if (type == R_TYPE) {
            int rd = regToInt(a);
            int rt = regToInt(b);
            int rs = regToInt(c);
            int shamt = 0;

            if (!strcmp(op, "SLL") || !strcmp(op, "SRL")) {
                shamt = atoi(c);
                rs = 0;
            }

            int func = funcCode(op);
            machine = buildR(func, shamt, rd, rt, rs);

            printf(GREEN "[R] " RESET BOLD "%-4s " RESET, op);
            printf(DIM "| func:%d sh:%d rd:%d rt:%d rs:%d " RESET,
                   func, shamt, rd, rt, rs);
        }

        else if (type == I_TYPE) {
            int rt  = regToInt(a);
            int rs  = regToInt(b);
            int imm = atoi(c);
            int opc = opcode(op);

            machine = buildI(imm, rt, rs, opc);

            printf(YELLOW "[I] " RESET BOLD "%-4s " RESET, op);
            printf(DIM "| imm:%d rt:%d rs:%d opc:%d " RESET,
                   imm, rt, rs, opc);
        }

        else if (type == J_TYPE) {
            int addr_val = atoi(a);

            machine = buildJ(addr_val);

            printf(MAGENTA "[J] " RESET BOLD "%-4s " RESET, op);
            printf(DIM "| addr:%d opc:6 " RESET, addr_val);
        }

        else {
            printf(RED BOLD "ERROR: " RESET RED "Invalid instruction -> %s\n" RESET, op);
            continue;
        }

       
        printf("| HEX: " BOLD CYAN "%06X\n" RESET, machine);

        fprintf(out, "%06X\n", machine);
        addr++;
    }

  
    printf(BOLD CYAN "\n════════════════ DONE ════════════════\n" RESET);

    fclose(in);
    fclose(out);

    return 0;
}