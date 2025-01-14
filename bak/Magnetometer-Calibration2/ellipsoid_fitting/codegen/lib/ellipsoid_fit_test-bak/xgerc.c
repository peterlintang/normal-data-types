/*
 * File: xgerc.c
 *
 * MATLAB Coder version            : 5.3
 * C/C++ source code generated on  : 02-Aug-2024 10:19:14
 */

/* Include Files */
#include "xgerc.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : int m
 *                int n
 *                const creal_T alpha1
 *                int ix0
 *                const creal_T y[3]
 *                creal_T A[9]
 *                int ia0
 * Return Type  : void
 */
void xgerc(int m, int n, const creal_T alpha1, int ix0, const creal_T y[3],
           creal_T A[9], int ia0)
{
  double b_A_re_tmp;
  double temp_im;
  double temp_re;
  double y_tmp;
  int A_re_tmp;
  int i;
  int ijA;
  int j;
  int jA;
  if ((!(alpha1.re == 0.0)) || (!(alpha1.im == 0.0))) {
    jA = ia0;
    for (j = 0; j < n; j++) {
      temp_im = y[j].re;
      y_tmp = y[j].im;
      if ((temp_im != 0.0) || (y_tmp != 0.0)) {
        temp_re = temp_im * alpha1.re + y_tmp * alpha1.im;
        temp_im = temp_im * alpha1.im - y_tmp * alpha1.re;
        i = m + jA;
        for (ijA = jA; ijA < i; ijA++) {
          A_re_tmp = ((ix0 + ijA) - jA) - 1;
          y_tmp = A[A_re_tmp].re;
          b_A_re_tmp = A[A_re_tmp].im;
          A[ijA - 1].re += y_tmp * temp_re - b_A_re_tmp * temp_im;
          A[ijA - 1].im += y_tmp * temp_im + b_A_re_tmp * temp_re;
        }
      }
      jA += 3;
    }
  }
}

/*
 * File trailer for xgerc.c
 *
 * [EOF]
 */
