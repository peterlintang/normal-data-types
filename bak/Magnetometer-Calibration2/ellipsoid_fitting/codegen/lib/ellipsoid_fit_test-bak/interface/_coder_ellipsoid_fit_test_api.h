/*
 * File: _coder_ellipsoid_fit_test_api.h
 *
 * MATLAB Coder version            : 5.3
 * C/C++ source code generated on  : 02-Aug-2024 10:19:14
 */

#ifndef _CODER_ELLIPSOID_FIT_TEST_API_H
#define _CODER_ELLIPSOID_FIT_TEST_API_H

/* Include Files */
#include "emlrt.h"
#include "tmwtypes.h"
#include <string.h>

/* Type Definitions */
#ifndef struct_emxArray_real_T
#define struct_emxArray_real_T
struct emxArray_real_T {
  real_T *data;
  int32_T *size;
  int32_T allocatedSize;
  int32_T numDimensions;
  boolean_T canFreeData;
};
#endif /* struct_emxArray_real_T */
#ifndef typedef_emxArray_real_T
#define typedef_emxArray_real_T
typedef struct emxArray_real_T emxArray_real_T;
#endif /* typedef_emxArray_real_T */

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void ellipsoid_fit_test(emxArray_real_T *x_m, emxArray_real_T *y_m,
                        emxArray_real_T *z_m, creal_T v[10], creal_T offset[3],
                        creal_T gain[9], creal_T matrix[9]);

void ellipsoid_fit_test_api(const mxArray *const prhs[3], int32_T nlhs,
                            const mxArray *plhs[4]);

void ellipsoid_fit_test_atexit(void);

void ellipsoid_fit_test_initialize(void);

void ellipsoid_fit_test_terminate(void);

void ellipsoid_fit_test_xil_shutdown(void);

void ellipsoid_fit_test_xil_terminate(void);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for _coder_ellipsoid_fit_test_api.h
 *
 * [EOF]
 */
