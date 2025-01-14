/*
 * File: rtGetNaN.c
 *
 * MATLAB Coder version            : 5.3
 * C/C++ source code generated on  : 02-Aug-2024 10:19:14
 */

/*
 * Abstract:
 *       MATLAB for code generation function to initialize non-finite, NaN
 */
/* Include Files */
#include "rtGetNaN.h"
#include "rt_nonfinite.h"

/*
 * Function: rtGetNaN
 * ======================================================================
 *  Abstract:
 * Initialize rtNaN needed by the generated code.
 *  NaN is initialized as non-signaling. Assumes IEEE.
 */
real_T rtGetNaN(void)
{
  return rtNaN;
}

/*
 * Function: rtGetNaNF
 * =====================================================================
 *  Abstract:
 *  Initialize rtNaNF needed by the generated code.
 *  NaN is initialized as non-signaling. Assumes IEEE
 */
real32_T rtGetNaNF(void)
{
  return rtNaNF;
}

/*
 * File trailer for rtGetNaN.c
 *
 * [EOF]
 */
