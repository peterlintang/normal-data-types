/*
 * File: ellipsoid_fit_test.h
 *
 * MATLAB Coder version            : 5.3
 * C/C++ source code generated on  : 13-Aug-2024 17:16:40
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
                               const emxArray_real_T *z_m, double v[10],
                               double offset[3], double gain[9],
                               double matrix[9]);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for ellipsoid_fit_test.h
 *
 * [EOF]
 */
