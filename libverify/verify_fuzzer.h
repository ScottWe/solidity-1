/**
 * The variables and functions that needed in verify_libfuzzer.cpp
 */

#pragma once

#include <stdint.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif

/*global variable Env record the environment*/
static jmp_buf Env;

/*global variable exception_type 
 * record the return value of setjmp function,
 * which can label different exception types*/
static int exception_type;

/*set up the exploration with Env environment,
 * return the result of setjmp function*/
int SetupExploration (void);

/*terminate the exploration with Env environment 
 * and different exception types, 
 * which can jump to setjmp with different results*/
void TerminateExploration (int exception_type);

/*
 * assign the global array with Data array*/
void ran(const uint8_t *Data, size_t Size);

/* whether the global array run out*/
void whether_RunOut(void);

/*
 * input the Data */
int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size);

#ifdef __cplusplus
}
#endif
