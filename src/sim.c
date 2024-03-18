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
    int rm;
    int rn;
    int rd;
    int rt;
    int mem;
    int opcode;
    int imm;
    int option;
    int shift;
    // Agregar la función misma que vamos a usar en el execute() en vez de tener que leer uno por uno los opcodes hardcodeados para determinar la operacion

} inst_t;

inst_t decode(int inst) { // Esto debería devolver un puntero
    inst_t decoded;

    // Caso optcode en bits 30-24
    int mask = 0b01111111 << 24; 
    decoded.opcode = (inst & mask) >> 24;

    // Check ADDS IMMEDIATE
    if (decoded.opcode == 0b0110001) {
        int r_mask = 0b11111;
        decoded.rd = (inst & r_mask);
        decoded.rn = (inst & (r_mask << 5)) >> 5;
        int shift_mask = 0b1 << 22;
        decoded.shift = (inst & shift_mask) >> 22;
        int imm_mask = 0b111111111111 << 10;
        decoded.imm = (inst & imm_mask) >> 10;

        // decoded.operation = funcion o lo que poronga sea

        return decoded;
    }
    // Check SUBS IMMEDIATE
    if (decoded.opcode == 0b1110001) {
        int r_mask = 0b11111;
        decoded.rd = (inst & r_mask);
        decoded.rn = (inst & (r_mask << 5)) >> 5;
        int shift_mask = 0b1 << 22;
        decoded.shift = (inst & shift_mask) >> 22;
        int imm_mask = 0b111111111111 << 10;
        decoded.imm = (inst & imm_mask) >> 10;

        // decoded.operation = funcion o lo que poronga sea

        return decoded;
    }


    // Caso optcode en bits 30-21
    int mask30_21 = 0b01111111111 << 21;
    decoded.opcode = (inst & mask30_21) >> 21;

    // Check ADDS ER
    if (decoded.opcode == 0b0101011001) {
        int r_mask = 0b11111;
        decoded.rd = (inst & r_mask);
        decoded.rn = (inst & (r_mask << 5)) >> 5;
        decoded.rm = (inst & (r_mask << 16)) >> 16;
        int three_mask = 0b111;
        decoded.option = (inst & (three_mask << 13)) >> 13;
        decoded.imm = (inst & (three_mask << 10)) >> 10;

        // decoded.operation = funcion o lo que poronga sea

        return decoded;
    }
    // Check SUBS ER
    if (decoded.opcode == 0b1101011001) {
        int r_mask = 0b11111;
        decoded.rd = (inst & r_mask);
        decoded.rn = (inst & (r_mask << 5)) >> 5;
        decoded.rm = (inst & (r_mask << 16)) >> 16;
        int three_mask = 0b111;
        decoded.option = (inst & (three_mask << 13)) >> 13;
        decoded.imm = (inst & (three_mask << 10)) >> 10;

        // decoded.operation = funcion o lo que poronga sea

        return decoded;
    }

    // Caso optcode en bits 30-23
    int mask30_23 = 0b011111111 << 23;
    decoded.opcode = (inst & mask30_23) >> 23;

    // Check MOVZ operation
    if (decoded.opcode == 0b10100101) {
        int r_mask = 0b11111;
        decoded.rd = (inst & r_mask);
        int imm_mask = 0b1111111111111111 << 5;
        decoded.imm = (inst & imm_mask) >> 5;
        return decoded;
    }
    // Check LSL/LSR
    if (decoded.opcode == 0b10100110) {
        int r_mask = 0b11111;
        decoded.rd = (inst & r_mask);
        decoded.rn =(inst & (r_mask << 5)) >> 5;
        int imm_mask = 0b111111 << 16;
        decoded.imm = (inst & imm_mask) >> 16;
        int variant_mask = 0b011111 << 10;
        int variant = (inst & variant_mask) >> 10;
        if (variant == 0b11111){
            // implement RIGHT shift logic
        } else {
            // implement LEFT shift logic
        }
        return decoded;
    }

    // Caso optcode en bits 29-21
    int mask29_21 = 0b00111111111 << 21;
    decoded.opcode = (inst & mask29_21) >> 21;
    // Check LDUR/LDURB/LDURH operations
    if (decoded.opcode == 0b111000010) {
        int r_mask = 0b11111;
        decoded.rt = (inst & r_mask);
        decoded.rn =(inst & (r_mask << 5)) >> 5;
        int imm_mask = 0b111111111 << 12;
        decoded.imm = (inst & imm_mask) >> 12;
        int variant_mask = 0b11 << 30;
        int variant = (variant_mask & inst) >> 30;
        if (variant == 0b11){
            // Implement LDUR
        } else if (variant ==0b00){
            // Implement LDURB
        } else if (variant == 0b01){
            // Implement LDURH
        }
        return decoded;
    }
    // Check STUR/STURB/STURH operations
    if (decoded.opcode == 0b111000000) {
        int r_mask = 0b11111;
        decoded.rt = (inst & r_mask);
        decoded.rn =(inst & (r_mask << 5)) >> 5;
        int imm_mask = 0b111111111 << 12;
        decoded.imm = (inst & imm_mask) >> 12;
        int variant_mask = 0b11 << 30;
        int variant = (variant_mask & inst) >> 30;
        if (variant == 0b11){
            // Implement STUR
        }else if (variant == 0b10) {
            // Implement STURB
        } else if (variant ==0b00){
            // Implement STURH
        } else if (variant == 0b01){}
        return decoded;
    }

    // Caso optcode en bits 31-24
    int mask = 0b11111111 << 24; 
    decoded.opcode = (inst & mask) >> 24;
     // Check B.Cond
    if (decoded.opcode == 0b01010100) {
        int imm_mask = 0b1111111111111111111<<5;
        decoded.imm = (inst & (imm_mask)) >> 5;
        int variant_mask = 0b1111;
        int variant = variant_mask & inst;
        if (variant == 0b0000){}else if (variant == 0b01) {}  // IMPLEMENTAR LOGICA DE LAS VARIANTES DEL B.COND



    return decoded;
}

    // Caso optcode en bits 31-21
    int mask30_21 = 0b11111111111 << 21;
    decoded.opcode = (inst & mask30_23) >> 21;
    // HALT o HLT
    if (decoded.opcode == 0b11010100010) {

        // setear nombre de la function a HALT o algo asi

        return decoded;
}

}

int inst = 0b10101011000101010101010010101010;

int main(void) {
    inst_t i_prueba = decode(inst);
    printf("%d\n", i_prueba.opcode);
    printf("%d\n", i_prueba.rm);
    printf("%d\n", i_prueba.rn);
    printf("%d\n", i_prueba.rd);
    printf("%d\n", i_prueba.imm);
    printf("%d\n", i_prueba.option);

    return 0;
}
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