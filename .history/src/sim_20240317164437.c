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
    int mem;
    int opcode;
    int imm;
    int option;

} inst_t;

inst_t decode(int inst) { // Esto debería 
    inst_t decoded;
    int mask = 0b01111111 << 24; // no todas las instrucciones tienen este opcode, cuidado.
    decoded.opcode = (inst & mask) >> 24;
    if (decoded.opcode == 0b0101011) {
        int r_mask = 0b11111;
        decoded.rm = (inst & (r_mask << 20));
        decoded.rn = (inst & (r_mask << 9));
        decoded.rd = (inst & (r_mask << 4));
        
        int three_mask = 0b111;
        decoded.imm = (inst & (three_mask << 12));
        decoded.option = (inst & (three_mask << 15));

        return decoded;
    }
    return decoded;
}

int inst = 0b1010101100010101010101001010101010;

int main(void) {
    inst_t i_prueba = decode(inst);
    printf("%d\n", i_prueba.opcode);
    printf("%d\n", i_prueba.rm);
    printf("%d\n", i_prueba.rn);
    printf("%d\n", i_prueba.rd);

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