/*
 * File: _coder_ellipsoid_fit_api.h
 *
 * MATLAB Coder version            : 5.3
 * C/C++ source code generated on  : 02-Aug-2024 17:35:13
 */

#ifndef _CODER_ELLIPSOID_FIT_API_H
#define _CODER_ELLIPSOID_FIT_API_H

/* Include Files */
#include "emlrt.h"
#include "tmwtypes.h"
#include <string.h>

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void ellipsoid_fit(real_T x[1486], real_T y[1486], real_T z[1486],
                   creal_T u[10]);

void ellipsoid_fit_api(const mxArray *const prhs[3], const mxArray **plhs);

void ellipsoid_fit_atexit(void);

void ellipsoid_fit_initialize(void);

void ellipsoid_fit_terminate(void);

void ellipsoid_fit_xil_shutdown(void);

void ellipsoid_fit_xil_terminate(void);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for _coder_ellipsoid_fit_api.h
 *
 * [EOF]
 */
