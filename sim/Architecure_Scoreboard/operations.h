#pragma once

void ld(int dst, int imm);

void st(int dst, int imm);

void add(int dst, int src0, int src1);

void sub(int dst, int src0, int src1);

void mult(int dst, int src0, int src1);

void div(int dst, int src0, int src1);

void halt();