/*
 * File: _coder_myfunc_mex.h
 *
 * MATLAB Coder version            : 5.3
 * C/C++ source code generated on  : 25-Jul-2024 16:00:28
 */

#ifndef _CODER_MYFUNC_MEX_H
#define _CODER_MYFUNC_MEX_H

/* Include Files */
#include "emlrt.h"
#include "mex.h"
#include "tmwtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
MEXFUNCTION_LINKAGE void mexFunction(int32_T nlhs, mxArray *plhs[],
                                     int32_T nrhs, const mxArray *prhs[]);

emlrtCTX mexFunctionCreateRootTLS(void);

void unsafe_myfunc_mexFunction(int32_T nlhs, mxArray *plhs[1], int32_T nrhs,
                               const mxArray *prhs[1]);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for _coder_myfunc_mex.h
 *
 * [EOF]
 */
