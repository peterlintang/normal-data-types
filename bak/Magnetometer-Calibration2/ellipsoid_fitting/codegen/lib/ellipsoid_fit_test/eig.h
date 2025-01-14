/*
 * File: eig.h
 *
 * MATLAB Coder version            : 5.3
 * C/C++ source code generated on  : 13-Aug-2024 17:16:40
 */

#ifndef EIG_H
#define EIG_H

/* Include Files */
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void b_eig(const double A[9], creal_T V[9], creal_T D[9]);

void eig(const double A[36], creal_T V[36], creal_T D[36]);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for eig.h
 *
 * [EOF]
 */
