/*
 * File: ellipsoid_fit.h
 *
 * MATLAB Coder version            : 5.3
 * C/C++ source code generated on  : 13-Aug-2024 17:16:40
 */

#ifndef ELLIPSOID_FIT_H
#define ELLIPSOID_FIT_H

/* Include Files */
#include "ellipsoid_fit_test_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void binary_expand_op(emxArray_real_T *D, const emxArray_real_T *r3,
                      const emxArray_real_T *r4, const emxArray_real_T *r5,
                      const emxArray_real_T *y, const emxArray_real_T *z_m,
                      const emxArray_real_T *b_y, const emxArray_real_T *y_m,
                      const emxArray_real_T *x_m);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for ellipsoid_fit.h
 *
 * [EOF]
 */
