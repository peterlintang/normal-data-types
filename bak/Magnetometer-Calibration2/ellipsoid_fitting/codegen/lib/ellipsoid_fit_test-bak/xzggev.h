/*
 * File: xzggev.h
 *
 * MATLAB Coder version            : 5.3
 * C/C++ source code generated on  : 02-Aug-2024 10:19:14
 */

#ifndef XZGGEV_H
#define XZGGEV_H

/* Include Files */
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void b_xzggev(creal_T A[9], int *info, creal_T alpha1[3], creal_T beta1[3],
              creal_T V[9]);

void xzggev(creal_T A[36], int *info, creal_T alpha1[6], creal_T beta1[6],
            creal_T V[36]);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for xzggev.h
 *
 * [EOF]
 */
