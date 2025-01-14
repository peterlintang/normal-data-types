/*
 * File: xnrm2.c
 *
 * MATLAB Coder version            : 5.3
 * C/C++ source code generated on  : 02-Aug-2024 10:19:14
 */

/* Include Files */
#include "xnrm2.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
/*
 * Arguments    : int n
 *                const double x[3]
 * Return Type  : double
 */
double b_xnrm2(int n, const double x[3])
{
  double absxk;
  double scale;
  double t;
  double y;
  y = 0.0;
  if (n >= 1) {
    if (n == 1) {
      y = fabs(x[1]);
    } else {
      scale = 3.3121686421112381E-170;
      absxk = fabs(x[1]);
      if (absxk > 3.3121686421112381E-170) {
        y = 1.0;
        scale = absxk;
      } else {
        t = absxk / 3.3121686421112381E-170;
        y = t * t;
      }
      absxk = fabs(x[2]);
      if (absxk > scale) {
        t = scale / absxk;
        y = y * t * t + 1.0;
        scale = absxk;
      } else {
        t = absxk / scale;
        y += t * t;
      }
      y = scale * sqrt(y);
    }
  }
  return y;
}

/*
 * Arguments    : int n
 *                const double x[36]
 *                int ix0
 * Return Type  : double
 */
double xnrm2(int n, const double x[36], int ix0)
{
  double absxk;
  double scale;
  double t;
  double y;
  int k;
  int kend;
  y = 0.0;
  if (n >= 1) {
    if (n == 1) {
      y = fabs(x[ix0 - 1]);
    } else {
      scale = 3.3121686421112381E-170;
      kend = (ix0 + n) - 1;
      for (k = ix0; k <= kend; k++) {
        absxk = fabs(x[k - 1]);
        if (absxk > scale) {
          t = scale / absxk;
          y = y * t * t + 1.0;
          scale = absxk;
        } else {
          t = absxk / scale;
          y += t * t;
        }
      }
      y = scale * sqrt(y);
    }
  }
  return y;
}

/*
 * File trailer for xnrm2.c
 *
 * [EOF]
 */
