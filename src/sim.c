#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "shell.h"

void process_instruction()
{
    /* execute one instruction here. You should use CURRENT_STATE and modify
     * values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
     * access memory. 
     * 
     * Sugerencia: hagan una funcion para decode()
     *             y otra para execute()
     * 
     * */

}

typedef struct inst {
    uint32_t rm;
    uint32_t rn;
    uint32_t rd;
    uint32_t rt;
    char method_name[32];
    uint32_t imm;
    uint32_t option;
    uint32_t shift;
    uint32_t variant;
    // Agregar la función misma que vamos a usar en el execute() en vez de tener que leer uno por uno los opcodes hardcodeados para determinar la operacion

} inst_t;

inst_t decode(uint32_t inst) { // Esto debería devolver un puntero
    inst_t decoded; //ESTO NO DEBERIA SER UN MALLOC?
    uint32_t opcode;

    // Caso optcode en bits 30-24
    uint32_t mask30_24 = 0b01111111 << 24; 
    opcode = (inst & mask30_24) >> 24;

    // Check ADDS IMMEDIATE
    if (opcode == 0b0110001) {
        uint32_t r_mask = 0b11111;
        decoded.rd = (inst & r_mask);
        decoded.rn = (inst & (r_mask << 5)) >> 5;
        uint32_t shift_mask = 0b1 << 22;
        decoded.shift = (inst & shift_mask) >> 22;
        uint32_t imm_mask = 0b111111111111 << 10;
        decoded.imm = (inst & imm_mask) >> 10;
        strcpy(decoded.method_name, "adds_imm");
        return decoded;
    }

    // Check SUBS IMMEDIATE / CMP IMMEDIATE
    if (opcode == 0b1110001) {
        uint32_t r_mask = 0b11111;
        decoded.rd = (inst & r_mask);
        if (decoded.rd == 0b11111){
            strcpy(decoded.method_name, "cmp_imm");
        } else {
            strcpy(decoded.method_name, "subs_imm");
        }
        decoded.rn = (inst & (r_mask << 5)) >> 5;
        uint32_t shift_mask = 0b1 << 22;
        decoded.shift = (inst & shift_mask) >> 22;
        uint32_t imm_mask = 0b111111111111 << 10;
        decoded.imm = (inst & imm_mask) >> 10;
        return decoded;
    }

    // Check ANDS SHIFTED REG
    if (opcode == 0b0110001) {
        uint32_t r_mask = 0b11111;
        decoded.rd = (inst & r_mask);
        decoded.rn = (inst & (r_mask << 5)) >> 5;
        uint32_t shift_mask = 0b1 << 22;
        decoded.shift = (inst & shift_mask) >> 22;
        uint32_t imm_mask = 0b111111111111 << 10;
        decoded.imm = (inst & imm_mask) >> 10;
        strcpy(decoded.method_name, "ands_sr");
        return decoded;
    }

    // Check EOR
    if (opcode == 0b1001010) {
        uint32_t r_mask = 0b11111;
        decoded.rd = (inst & r_mask);
        decoded.rn = (inst & (r_mask << 5)) >> 5;
        decoded.rm = (inst & (r_mask << 16)) >> 16;
        uint32_t shift_mask = 0b11 << 22;
        decoded.shift = (inst & shift_mask) >> 22;
        uint32_t imm_mask = 0b111111 << 10;
        decoded.imm = (inst & imm_mask) >> 10;
        strcpy(decoded.method_name, "eor");
        return decoded;
    }
        // Check ORR
    if (opcode == 0b0101010) {
        uint32_t r_mask = 0b11111;
        decoded.rd = (inst & r_mask);
        decoded.rn = (inst & (r_mask << 5)) >> 5;
        decoded.rm = (inst & (r_mask << 16)) >> 16;
        uint32_t shift_mask = 0b11 << 22;
        decoded.shift = (inst & shift_mask) >> 22;
        uint32_t imm_mask = 0b111111 << 10;
        decoded.imm = (inst & imm_mask) >> 10;
        strcpy(decoded.method_name, "orr");
        return decoded;
    }

    // Caso optcode en bits 31-26
    uint32_t mask31_26 = 0b111111 << 26;
    opcode = (inst & mask31_26) >> 26;

     // Check B
    if (opcode == 0b000101) {
        uint32_t imm_mask = 0b11111111111111111111111111;
        decoded.imm = inst & imm_mask;
        strcpy(decoded.method_name, "b");
        return decoded;
    }

    // Caso optcode en bits 31-10
    uint32_t mask31_10 = 0b1111111111111111111111 << 10;
    opcode = (inst & mask31_10) >> 10;

     // Check BR
    if (opcode == 0b1101011000011111000000) {
        uint32_t tail_mask = 0b11111;
        uint32_t tail = inst & tail_mask;
        if (tail == 0b00000){
            uint32_t r_mask = 0b11111 << 5;
            decoded.rm = inst & r_mask;
            strcpy(decoded.method_name, "br");
            return decoded;
        }
    }

    // Caso optcode en bits 30-21
    uint32_t mask30_21 = 0b01111111111 << 21;
    opcode = (inst & mask30_21) >> 21;

    // Check ADDS ER
    if (opcode == 0b0101011001) {
        uint32_t r_mask = 0b11111;
        decoded.rd = (inst & r_mask);
        decoded.rn = (inst & (r_mask << 5)) >> 5;
        decoded.rm = (inst & (r_mask << 16)) >> 16;
        uint32_t three_mask = 0b111;
        decoded.option = (inst & (three_mask << 13)) >> 13;
        decoded.imm = (inst & (three_mask << 10)) >> 10;
        strcpy(decoded.method_name, "adds_er");
        return decoded;
    }

    // Check SUBS ER / CMP ER
    if (opcode == 0b1101011001) {
        uint32_t r_mask = 0b11111;
        decoded.rd = (inst & r_mask);
        if (decoded.rd == 0b11111){
             strcpy(decoded.method_name, "subs_er");
        } else {
            strcpy(decoded.method_name, "cmp_er");
        }
        decoded.rn = (inst & (r_mask << 5)) >> 5;
        decoded.rm = (inst & (r_mask << 16)) >> 16;
        uint32_t three_mask = 0b111;
        decoded.option = (inst & (three_mask << 13)) >> 13;
        decoded.imm = (inst & (three_mask << 10)) >> 10;
        return decoded;
    }

    // Caso optcode en bits 30-23
    uint32_t mask30_23 = 0b011111111 << 23;
    opcode = (inst & mask30_23) >> 23;

    // Check MOVZ operation
    if (opcode == 0b10100101) {
        uint32_t r_mask = 0b11111;
        decoded.rd = (inst & r_mask);
        uint32_t imm_mask = 0b1111111111111111 << 5;
        decoded.imm = (inst & imm_mask) >> 5;
        strcpy(decoded.method_name, "movz");
        return decoded;
    }

    // Check LSL/LSR
    if (opcode == 0b10100110) {
        uint32_t r_mask = 0b11111;
        decoded.rd = (inst & r_mask);
        decoded.rn =(inst & (r_mask << 5)) >> 5;
        uint32_t imm_mask = 0b111111 << 16;
        decoded.imm = (inst & imm_mask) >> 16;
        uint32_t variant_mask = 0b011111 << 10;
        uint32_t variant = (inst & variant_mask) >> 10;
        if (variant == 0b11111){
            strcpy(decoded.method_name, "lsr");
        } else {
            strcpy(decoded.method_name, "lsl");
        }
        return decoded;
    }

    // Caso optcode en bits 29-21
    uint32_t mask29_21 = 0b00111111111 << 21;
    opcode = (inst & mask29_21) >> 21;
    // Check LDUR/LDURB/LDURH operations
    if (opcode == 0b111000010) {
        uint32_t r_mask = 0b11111;
        decoded.rt = (inst & r_mask);
        decoded.rn =(inst & (r_mask << 5)) >> 5;
        uint32_t imm_mask = 0b111111111 << 12;
        decoded.imm = (inst & imm_mask) >> 12;
        uint32_t variant_mask = 0b11 << 30;
        decoded.variant = (variant_mask & inst) >> 30;
        strcpy(decoded.method_name, "ldur");
        return decoded;
    }

    // Check STUR/STURB/STURH operations
    if (opcode == 0b111000000) {
        uint32_t r_mask = 0b11111;
        decoded.rt = (inst & r_mask);
        decoded.rn =(inst & (r_mask << 5)) >> 5;
        uint32_t imm_mask = 0b111111111 << 12;
        decoded.imm = (inst & imm_mask) >> 12;
        uint32_t variant_mask = 0b11 << 30;
        decoded.variant = (variant_mask & inst) >> 30;
        strcpy(decoded.method_name, "stur");
        return decoded;
    }

    // Caso optcode en bits 31-24
    uint32_t mask = 0b11111111 << 24; 
    opcode = (inst & mask) >> 24;

     // Check B.Cond
    if (opcode == 0b01010100) {
        uint32_t imm_mask = 0b1111111111111111111<<5;
        decoded.imm = (inst & (imm_mask)) >> 5;
        uint32_t variant_mask = 0b1111;
        decoded.variant = variant_mask & inst;
        strcpy(decoded.method_name, "bcond");
        return decoded;
    }

    // Caso optcode en bits 31-21
    uint32_t mask31_21 = 0b11111111111 << 21;
    opcode = (inst & mask31_21) >> 21;
    // HALT o HLT
    if (opcode == 0b11010100010) {
        uint32_t end_mask = 0b11111;
        uint32_t tail = inst & end_mask;
        if (tail == 0b00000){
            strcpy(decoded.method_name, "hlt");
            return decoded;
        }
    }
    printf("No hay compatibilidad con ningun método\n");
}

uint32_t inst = 0b00110001010000000001000010000100;



int main(void) {
    inst_t i_prueba = decode(inst);
    printf("Nombre del método: %s\n", i_prueba.method_name);
    printf("RM: %d\n", i_prueba.rm);
    printf("RN: %d\n", i_prueba.rn);
    printf("RD: %d\n", i_prueba.rd);
    printf("RT: %d\n", i_prueba.rt);
    printf("IMM: %d\n", i_prueba.imm);
    printf("OPTION: %d\n", i_prueba.option);
    printf("SHIFT: %d\n", i_prueba.shift);
    printf("VARIANT: %d\n", i_prueba.variant);

    return 0;
}




/* 
El estado del CPU está modelado por la struct CPU_STATE. En este struct hay un vector de 32 enteros de 64 bits cada uno. Cada
posición del vector representa un registro y cada número representa el valor que toma su respectivo registro. Cuando una instrucción
involucra guardar o modificar algo en un registro, debemos actualizar este vector con el nuevo valor que toma/n el/los registro/s.
*/

/*
El program counter (PC) que también se encuentra en CPU_STATE se incrementa por su cuenta cuando se corre la función cylce()
*/

/*
El flag_n se tiene que actualizar a 1 cuando el resultado de alguna instrucción es negativo y a 0 cuando no
El flag_z se tiene que actualizar a 1 cuando el resultado de alguna instrucción es cero y a 0 cuando no
*/

/* 
Preguntas:

- Los registros del inst_t deberían ser de tipo uint64 o ints? Yo creo que no importa porque es algo interno. Los registros en sí están
definidos en shell.c
*/