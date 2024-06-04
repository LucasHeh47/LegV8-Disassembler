#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 33

unsigned int binToDec(char *bin) {
    unsigned int decimal = 0;
    while (*bin != '\0') {
        decimal = (decimal << 1) + (*bin++ - '0');
    }
    return decimal;
}

void decodeAndWriteInstruction(char *binary, FILE *file) {
    char opcode[11], immediate[13], sourceReg[6], destReg[6], secondReg[6], shift[7];
    strncpy(opcode, binary, 10);
    opcode[10] = '\0';

    if (strcmp(opcode, "1001000100") == 0 || strcmp(opcode, "1101000100") == 0) { // ADDI and SUBI
        strncpy(immediate, binary + 10, 12);
        immediate[12] = '\0';
        strncpy(sourceReg, binary + 22, 5);
        sourceReg[5] = '\0';
        strncpy(destReg, binary + 27, 5);
        destReg[5] = '\0';
        unsigned int imm = binToDec(immediate);
        unsigned int src = binToDec(sourceReg);
        unsigned int dest = binToDec(destReg);

        if (strcmp(opcode, "1001000100") == 0) {
            fprintf(file, "ADD X%d, X%d, #%d\n", dest, src, imm);
        } else if (strcmp(opcode, "1101000100") == 0) {
            fprintf(file, "SUB X%d, X%d, #%d\n", dest, src, imm);
        }
    } else if (strcmp(opcode, "10001011000") == 0 || strcmp(opcode, "11001011000") == 0 || strcmp(opcode, "10101011000") == 0 || strcmp(opcode, "11101011000") == 0) { // ADD, SUB, ADDS, SUBS
        strncpy(secondReg, binary + 10, 5);
        secondReg[5] = '\0';
        strncpy(shift, binary + 15, 6);
        shift[6] = '\0';
        strncpy(sourceReg, binary + 21, 5);
        sourceReg[5] = '\0';
        strncpy(destReg, binary + 27, 5);
        destReg[5] = '\0';
        unsigned int second = binToDec(secondReg);
        unsigned int src = binToDec(sourceReg);
        unsigned int dest = binToDec(destReg);

        if (strcmp(opcode, "10001011000") == 0) {
            fprintf(file, "ADD X%d, X%d, X%d\n", dest, src, second);
        } else if (strcmp(opcode, "11001011000") == 0) {
            fprintf(file, "SUB X%d, X%d, X%d\n", dest, src, second);
        } else if (strcmp(opcode, "10101011000") == 0) {
            fprintf(file, "ADDS X%d, X%d, X%d\n", dest, src, second);
        } else if (strcmp(opcode, "11101011000") == 0) {
            fprintf(file, "SUBS X%d, X%d, X%d\n", dest, src, second);
        }
    } else if (strcmp(opcode, "11111000010") == 0) { // LDUR
        strncpy(immediate, binary + 11, 9);
        immediate[9] = '\0';
        strncpy(sourceReg, binary + 20, 5);
        sourceReg[5] = '\0';
        strncpy(destReg, binary + 25, 5);
        destReg[5] = '\0';
        unsigned int imm = binToDec(immediate);
        unsigned int src = binToDec(sourceReg);
        unsigned int dest = binToDec(destReg);
        fprintf(file, "LDUR X%d, [X%d, #%d]\n", dest, src, imm);
    } else if (strcmp(opcode, "11111000000") == 0) { // STUR
        strncpy(immediate, binary + 11, 9);
        immediate[9] = '\0';
        strncpy(sourceReg, binary + 20, 5);
        sourceReg[5] = '\0';
        strncpy(destReg, binary + 25, 5);
        destReg[5] = '\0';
        unsigned int imm = binToDec(immediate);
        unsigned int src = binToDec(sourceReg);
        unsigned int dest = binToDec(destReg);
        fprintf(file, "STUR X%d, [X%d, #%d]\n", dest, src, imm);
    } else {
        fprintf(file, "Unknown instruction\n");
    }
}

int main() {
    char binary[MAX_LENGTH];
    FILE *file = fopen("output.txt", "w");

    if (!file) {
        printf("Error opening file.\n");
        return 1;
    }

    while (1) {
        printf("Enter a 32-bit binary number (or type 'exit' to quit): ");
        scanf("%32s", binary);

        if (strcmp(binary, "exit") == 0) {
            break;
        }

        if (strlen(binary) != 32) {
            printf("Invalid input length. Must be 32 bits long.\n");
            continue;
        }

        decodeAndWriteInstruction(binary, file);
    }

    fclose(file);
    printf("Instructions written to output.txt\n");
    return 0;
}
