/*
 * File: ellipsoid_fit_test.h
 *
 * MATLAB Coder version            : 5.3
 * C/C++ source code generated on  : 02-Aug-2024 10:19:14
 */

#ifndef ELLIPSOID_FIT_TEST_H
#define ELLIPSOID_FIT_TEST_H

/* Include Files */
#include "ellipsoid_fit_test_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
extern void ellipsoid_fit_test(const emxArray_real_T *x_m,
                               const emxArray_real_T *y_m,
                               const emxArray_real_T *z_m, creal_T v[10],
                               creal_T offset[3], creal_T gain[9],
                               creal_T matrix[9]);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for ellipsoid_fit_test.h
 *
 * [EOF]
 */
