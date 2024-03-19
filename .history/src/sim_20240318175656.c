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
    // Agregar la función misma que vamos a usar en el execute() en vez de tener que leer uno por uno los opcodes hardcodeados para determinar la operacion

} inst_t;

inst_t decode(int inst) { // Esto debería devolver un puntero
    inst_t decoded;

    // Caso optcode en bits 30-2
    int mask = 0b01111111 << 24; // no todas las instrucciones tienen este opcode, cuidado.
    decoded.opcode = (inst & mask) >> 24;
    if (decoded.opcode == 0b0101011) {
        int r_mask = 0b11111;
        decoded.rm = (inst & (r_mask << 16)) >> 16;
        decoded.rn = (inst & (r_mask << 5)) >> 5;
        decoded.rd = (inst & r_mask);
        
        int three_mask = 0b111;
        decoded.imm = (inst & (three_mask << 10)) >> 10;
        decoded.option = (inst & (three_mask << 13)) >> 13;

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
        int variant = variant_mask & inst;
        if (variant == 0b11){}else if (variant == 0b10) {} else if (variant ==0b00){} else if (variant == 0b01){} // IMPLEMENTAR LOGICA DE LAS VARIANTES
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
        if (variant == 0b11){}else if (variant == 0b10) {} else if (variant ==0b00){} else if (variant == 0b01){} // IMPLEMENTAR LOGICA DE LAS VARIANTES
        return decoded;
    }

    // Caso optcode en bits 27-20
    int mask27_20 = 0b000011111111 << 20;
    decoded.opcode = (inst & mask27_20) >> 20;
    // Check LSL/LSR
    if (decoded.opcode == 0b00011010) {
        int r_mask = 0b1111;
        decoded.rm = (inst & r_mask);
        decoded.rd =(inst & (r_mask << 12)) >> 12;
        int imm_mask = 0b11111 << 12;
        decoded.imm = (inst & imm_mask) >> 12;
         int variant_mask = 0b11 << 5;
        int variant = (variant_mask & inst) >> 5;
        if (variant == 0b00){}else if (variant == 0b01) {}  // IMPLEMENTAR LOGICA DE LAS VARIANTES RIGHT Y LEFT

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

// ejecuta la instrucción
CPU_State execute(inst_t decoded) {
    if (decoded.opcode == 0b0101011) {

    }
}

void adds_ext_r(CPU_STATE* next_state, int rm, int option, int imm3, int rn, int rd) {
    *next_state.REGS[rd] = (*next_state.REGS[rm] + *next_state.REGS[rn]) << option * imm3; // puede ser que no haya que shiftear nada
    if (*next_state.REGS[rd] == 0) *next_state.FLAG_Z = 1;
    else if (*next_state.REGS[rd] < 0) *next_state.FLAG_N = 1;

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