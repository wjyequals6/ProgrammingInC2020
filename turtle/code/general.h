#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>

#define strsame(A,B) (strcmp(A, B)==0)
typedef enum bool {false, true} bool;
/*
 Reference: Neill‘s version.
*/
void on_error(const char* s);
void* ncalloc(int n, size_t size);
void* nfopen(char* fname, char* mode);
bool is_number(char* s);
float get_number(char* s);
bool is_capital_letter(char* s);
