/*
 * File: _coder_myfunc_api.h
 *
 * MATLAB Coder version            : 5.3
 * C/C++ source code generated on  : 25-Jul-2024 16:00:28
 */

#ifndef _CODER_MYFUNC_API_H
#define _CODER_MYFUNC_API_H

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
real_T myfunc(real_T x);

void myfunc_api(const mxArray *prhs, const mxArray **plhs);

void myfunc_atexit(void);

void myfunc_initialize(void);

void myfunc_terminate(void);

void myfunc_xil_shutdown(void);

void myfunc_xil_terminate(void);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for _coder_myfunc_api.h
 *
 * [EOF]
 */
