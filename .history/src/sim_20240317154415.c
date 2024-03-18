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

int decode(int inst) {
    inst_t decoded;
    int mask = 0b01111111 << 24;
    decoded.opcode = (inst && mask) >> 24;
    if (decoded.opcode == 0b0101011) {
        int r_mask = 0b11111 << 20;
        decoded.rm = (inst && rm_mask);
        
        int rn_mask = rm_mask >> 11;
        decoded.rn = (inst && rn_mask);

        int rd_mask = rn_mask >> 5;
    }
}

int inst = 0b11101011000101010101010010101010;

int result = get_opc(inst);

printf("%i", result);

typedef struct inst {
    int rm;
    int rn;
    int rd;
    int mem;
    int opcode;
    int imm;
} inst_t;

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