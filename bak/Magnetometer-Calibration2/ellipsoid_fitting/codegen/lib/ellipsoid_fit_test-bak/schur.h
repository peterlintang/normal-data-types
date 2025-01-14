/*
 * File: schur.h
 *
 * MATLAB Coder version            : 5.3
 * C/C++ source code generated on  : 02-Aug-2024 10:19:14
 */

#ifndef SCHUR_H
#define SCHUR_H

/* Include Files */
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void b_schur(double A[36], double V[36]);

void schur(const creal_T A[9], creal_T V[9], creal_T T[9]);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for schur.h
 *
 * [EOF]
 */
