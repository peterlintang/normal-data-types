/*
 * File: _coder_ellipsoid_fit_test_mex.h
 *
 * MATLAB Coder version            : 5.3
 * C/C++ source code generated on  : 13-Aug-2024 17:16:40
 */

#ifndef _CODER_ELLIPSOID_FIT_TEST_MEX_H
#define _CODER_ELLIPSOID_FIT_TEST_MEX_H

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

void unsafe_ellipsoid_fit_test_mexFunction(int32_T nlhs, mxArray *plhs[4],
                                           int32_T nrhs,
                                           const mxArray *prhs[3]);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for _coder_ellipsoid_fit_test_mex.h
 *
 * [EOF]
 */
