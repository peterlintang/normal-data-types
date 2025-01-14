/*
 * File: ellipsoid_fit_test_emxAPI.h
 *
 * MATLAB Coder version            : 5.3
 * C/C++ source code generated on  : 02-Aug-2024 10:19:14
 */

#ifndef ELLIPSOID_FIT_TEST_EMXAPI_H
#define ELLIPSOID_FIT_TEST_EMXAPI_H

/* Include Files */
#include "ellipsoid_fit_test_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
extern emxArray_real_T *emxCreateND_real_T(int numDimensions, const int *size);

extern emxArray_real_T *
emxCreateWrapperND_real_T(double *data, int numDimensions, const int *size);

extern emxArray_real_T *emxCreateWrapper_real_T(double *data, int rows,
                                                int cols);

extern emxArray_real_T *emxCreate_real_T(int rows, int cols);

extern void emxDestroyArray_real_T(emxArray_real_T *emxArray);

extern void emxInitArray_real_T(emxArray_real_T **pEmxArray, int numDimensions);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for ellipsoid_fit_test_emxAPI.h
 *
 * [EOF]
 */
