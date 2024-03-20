#include <stdio.h>
#include <assert.h>
#include <string.h>
#include<stdlib.h>
#include "shell.h"

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

inst_t decode(uint32_t inst);
void execute(inst_t decoded);


void process_instruction()
{
 /* execute one instruction here. You should use CURRENT_STATE and modify
 * values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
 * access memory. 
 * 
 * Sugerencia: hagan una funcion para decode()
 * y otra para execute()
 * 
 * */
 uint32_t curr_inst = mem_read_32(CURRENT_STATE.PC);
 inst_t next_inst = decode (curr_inst);
 execute(next_inst);

}



inst_t decode(uint32_t inst) {
 inst_t decoded;
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
 if (opcode == 0b1101010) {
 uint32_t r_mask = 0b11111;
 decoded.rd = (inst & r_mask);
 decoded.rn = (inst & (r_mask << 5)) >> 5;
 decoded.rm = (inst & (r_mask << 16)) >> 16;
 uint32_t shift_mask = 0b11 << 22;
 decoded.shift = (inst & shift_mask) >> 22;
 uint32_t imm_mask = 0b111111 << 10;
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
 decoded.rn = inst & r_mask;
 strcpy(decoded.method_name, "br");
 return decoded;
 }
 }

 // Caso optcode en bits 30-21
 uint32_t mask30_21 = 0b01111111111 << 21;
 opcode = (inst & mask30_21) >> 21;

 // Check ADDS ER
 if (opcode == 0b0101011000) {
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
 if (opcode == 0b1101011000) {
 uint32_t r_mask = 0b11111;
 decoded.rd = (inst & r_mask);
 if (decoded.rd == 0b11111){
 strcpy(decoded.method_name, "cmp_er");
 } else {
 strcpy(decoded.method_name, "subs_er");
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
 return decoded;
}

void adds_ext_r(int rm, int option, int imm3, int rn, int rd) {
 NEXT_STATE.REGS[rd] = (CURRENT_STATE.REGS[rm] + CURRENT_STATE.REGS[rn]); // puede ser que no haya que shiftear nada
 if (NEXT_STATE.REGS[rd] == 0) { 
 NEXT_STATE.FLAG_Z = 1;
 NEXT_STATE.FLAG_N = 0;
 }
 else if (NEXT_STATE.REGS[rd] < 0) { 
 NEXT_STATE.FLAG_N = 1;
 NEXT_STATE.FLAG_Z = 0;
 }
 else {
 NEXT_STATE.FLAG_N = 0;
 NEXT_STATE.FLAG_Z = 0;}
}

void movz(int imm, int rd) {
 NEXT_STATE.REGS[rd] = (int64_t) imm;
}

void ld(int rt, int rn, int variant) {

 if (variant == 0b01) { //LDURH
 
 int64_t value = (int64_t) (mem_read_32(rn) & 0x00000000ffffffff);
 NEXT_STATE.REGS[rt] = value;
 }
 else if (variant == 0b00) { //LDURB
 int64_t value = (int64_t) (mem_read_32(rn) & 0x000000000000ffff);
 NEXT_STATE.REGS[rt] = value;
 }
 else if (variant == 0b10 || 0b11) { //LDUR
 int64_t value = (int64_t) mem_read_32(rn);
 NEXT_STATE.REGS[rt] = value;
 }
}

void st(int rt, int rn, int variant, int imm) {
 
 int64_t address = CURRENT_STATE.REGS[rn] + (int64_t)imm;
 printf("address: %ld", address);
 
 if (variant == 0b01) { // STURH (2 bytes)
 uint32_t hw_ld = (uint32_t) (CURRENT_STATE.REGS[rt] & 0xffff);
 uint32_t hw_mem = (uint32_t) (mem_read_32(address) & 0x0000ffff);
 mem_write_32(CURRENT_STATE.REGS[rn] + imm, hw_ld | hw_mem); // no sé si estoy almacenando bien acá o si entra en juego little endian.// HAY QUE HACER 2 WRITES, PORQUE ESTAS GUARDANDO 64 BITS. PRIMERO LOS MAS SIGNIFIC Y DESPUES LOS MENOS
 }
 else if (variant == 0b00) { // STURB (1 byte)
 uint32_t hw_ld = (uint32_t) (CURRENT_STATE.REGS[rt] & 0b11111111);
 uint32_t hw_mem = (uint32_t) (mem_read_32(address) & 0x00ffffff);
 mem_write_32(address, hw_ld | hw_mem);
 }
 else if (variant == 0b11 || variant == 0b10) { // STUR (4 bytes)
 uint32_t most_sig = (CURRENT_STATE.REGS[rt] & (0xffffffff00000000)) >>32;
 uint32_t less_sig = CURRENT_STATE.REGS[rt] & 0x00000000ffffffff;
 printf("LESS SIG: %d\n", less_sig);
 printf("RN: %d\n", rn);
 
 mem_write_32(address, less_sig);
 mem_write_32(address + 4, most_sig);
 }
}

void hlt() {
 RUN_BIT = 0;
}

void adds_imm(int rn, int rd, int imm, int shift) {
 NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rn] + (imm << 12 * shift);
 if (NEXT_STATE.REGS[rd] == 0) {
 NEXT_STATE.FLAG_Z = 1;
 NEXT_STATE.FLAG_N = 0;
 }
 else if (NEXT_STATE.REGS[rd] < 0) {
 NEXT_STATE.FLAG_N = 1; 
 NEXT_STATE.FLAG_Z = 0;
 }
 else {
 NEXT_STATE.FLAG_N = 0;
 NEXT_STATE.FLAG_Z = 0;
 }
}

void subs_er(int rm, int rn, int rd) {
 NEXT_STATE.REGS[rd] = (CURRENT_STATE.REGS[rn] - CURRENT_STATE.REGS[rm]); // puede ser que no haya que shiftear nada
 if (NEXT_STATE.REGS[rd] == 0) { 
 NEXT_STATE.FLAG_Z = 1;
 NEXT_STATE.FLAG_N = 0;
 }
 else if (NEXT_STATE.REGS[rd] < 0) { 
 NEXT_STATE.FLAG_N = 1;
 NEXT_STATE.FLAG_Z = 0;
 }
 else {
 NEXT_STATE.FLAG_N = 0;
 NEXT_STATE.FLAG_Z = 0;}
}


void cmp_ext_r(int rn, int rm) {
 int64_t aux = CURRENT_STATE.REGS[rn] - CURRENT_STATE.REGS[rm];
 if (aux == 0) {
 NEXT_STATE.FLAG_Z = 1;
 NEXT_STATE.FLAG_N = 0;
 }
 else if (aux < 0) {
 NEXT_STATE.FLAG_N = 1; 
 NEXT_STATE.FLAG_Z = 0;
 }
 else {
 NEXT_STATE.FLAG_N = 0;
 NEXT_STATE.FLAG_Z = 0;
 }
}

void cmp_imm(int rn, int imm, int shift) {
 int64_t aux = CURRENT_STATE.REGS[rn] - (imm << (12 * shift));
 if (aux == 0) {
 NEXT_STATE.FLAG_Z = 1;
 NEXT_STATE.FLAG_N = 0;
 }
 else if (aux < 0) {
 NEXT_STATE.FLAG_N = 1; 
 NEXT_STATE.FLAG_Z = 0;
 }
 else {
 NEXT_STATE.FLAG_N = 0;
 NEXT_STATE.FLAG_Z = 0;
 }
}

void eor(int rm, int rn, int rd) {
 NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rn] ^ CURRENT_STATE.REGS[rm];
}
void orr(int rm, int rn, int rd) {
 NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rn] | CURRENT_STATE.REGS[rm];
}
void ands_sr(int rm, int rn, int rd) {
 NEXT_STATE.REGS[rd] = (CURRENT_STATE.REGS[rm] & CURRENT_STATE.REGS[rn]); 
 if (NEXT_STATE.REGS[rd] == 0) { 
 NEXT_STATE.FLAG_Z = 1;
 NEXT_STATE.FLAG_N = 0;
 }
 else if (NEXT_STATE.REGS[rd] < 0) { 
 NEXT_STATE.FLAG_N = 1;
 NEXT_STATE.FLAG_Z = 0;
 }
 else {
 NEXT_STATE.FLAG_N = 0;
 NEXT_STATE.FLAG_Z = 0;}
}
void b(int imm) {
 NEXT_STATE.PC = CURRENT_STATE.PC + (imm << 2) ;
}
void br(int rn) {
 NEXT_STATE.PC = rn;
}
void lsl(int rn, int rd, int imm) {
 NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rn] << (64-imm);
}
void lsr(int rn, int rd, int imm) {
 NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rn] >> imm;
}
void bcond(int imm, int variant) {
 // BEQ
 if (variant == 0b0000){
 if (CURRENT_STATE.FLAG_Z == 1){
 NEXT_STATE.PC = CURRENT_STATE.PC + (imm << 2); 
 }
 // BNE
 } else if (variant == 0b0001){
 if (CURRENT_STATE.FLAG_Z == 0){
 NEXT_STATE.PC = CURRENT_STATE.PC + (imm << 2); 
 }
 // BGT
 } else if (variant == 0b1100){
 if ((CURRENT_STATE.FLAG_Z == 0) && (CURRENT_STATE.FLAG_N == 0)){
 NEXT_STATE.PC = CURRENT_STATE.PC + (imm << 2); 
 }
 // BLT
 } else if (variant == 0b1011){
 if (CURRENT_STATE.FLAG_N == 1){
 NEXT_STATE.PC = CURRENT_STATE.PC + (imm << 2); 
 }
 // BGE
 } else if (variant == 0b1010){
 if (CURRENT_STATE.FLAG_N == 0){
 NEXT_STATE.PC = CURRENT_STATE.PC + (imm << 2); 
 }
 // BLE
 } else if (variant == 0b1101){
 if (!((CURRENT_STATE.FLAG_Z == 0) && (CURRENT_STATE.FLAG_N == 0))){
 NEXT_STATE.PC = CURRENT_STATE.PC + (imm << 2); 
 }
 }
 
}

void execute(inst_t decoded) {
 if (strcmp(decoded.method_name, "adds_er") == 0) {
 adds_ext_r(decoded.rm, decoded.option, decoded.imm, decoded.rn, decoded.rd);
 }
 else if (strcmp(decoded.method_name, "movz") == 0) {
 //printf("Rd: %d, IMM: %d\n", decoded.rd, decoded.imm);
 movz(decoded.imm, decoded.rd);
 }
 else if (strcmp(decoded.method_name, "ldur") == 0) {
 ld(decoded.rt, decoded.rn, decoded.variant);
 }
 else if (strcmp(decoded.method_name, "stur") == 0) {
 st(decoded.rt, decoded.rn, decoded.variant, decoded.imm);
 }
 else if (strcmp(decoded.method_name, "hlt") == 0) {
 hlt();
 }
 else if (strcmp(decoded.method_name, "adds_imm") == 0) {
 adds_imm(decoded.rn, decoded.rd, decoded.imm, decoded.shift);
 }
 else if (strcmp(decoded.method_name, "subs_er") == 0) {
 //printf("RM: %d, RN: %d, RD: %d\n", decoded.rm, decoded.rn, decoded.rd);
 subs_er(decoded.rm, decoded.rn, decoded.rd);
 }
 else if (strcmp(decoded.method_name, "hlt") == 0) {
 hlt();
 }
 else if (strcmp(decoded.method_name, "eor") == 0) {
 eor(decoded.rm, decoded.rn, decoded.rd);
 }
 else if (strcmp(decoded.method_name, "orr") == 0) {
 orr(decoded.rm, decoded.rn, decoded.rd);
 }
 else if (strcmp(decoded.method_name, "ands_sr") == 0) {
 ands_sr(decoded.rm, decoded.rn, decoded.rd);
 }
 else if (strcmp(decoded.method_name, "b") == 0) {
 b(decoded.imm);
 }
 else if (strcmp(decoded.method_name, "br") == 0) {
 br(decoded.rn);
 }
 else if (strcmp(decoded.method_name, "bcond") == 0) {
 bcond(decoded.imm, decoded.variant);
 }
 else if (strcmp(decoded.method_name, "lsl") == 0) {
 lsl(decoded.rn, decoded.rd, decoded.imm);
 }
 else if (strcmp(decoded.method_name, "lsr") == 0) {
 lsr(decoded.rn, decoded.rd, decoded.imm);
 }

 else if (strcmp(decoded.method_name, "cmp_er") == 0) {
 cmp_ext_r(decoded.rn, decoded.rm);
 }
 else if (strcmp(decoded.method_name, "cmp_imm") == 0) {
 cmp_imm(decoded.rn, decoded.imm, decoded.shift);
 }
 else if (strcmp(decoded.method_name, "ldur") == 0) {
 ld(decoded.rt, decoded.rn, decoded.variant);
 }

 NEXT_STATE.PC = CURRENT_STATE.PC += 4;
}