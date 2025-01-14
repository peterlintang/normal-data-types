/*
 * File: xrot.c
 *
 * MATLAB Coder version            : 5.3
 * C/C++ source code generated on  : 02-Aug-2024 17:35:13
 */

/* Include Files */
#include "xrot.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : double x[36]
 *                int ix0
 *                int iy0
 *                double c
 *                double s
 * Return Type  : void
 */
void b_xrot(double x[36], int ix0, int iy0, double c, double s)
{
  double b_temp_tmp;
  double d_temp_tmp;
  int c_temp_tmp;
  int k;
  int temp_tmp;
  for (k = 0; k < 6; k++) {
    temp_tmp = (iy0 + k) - 1;
    b_temp_tmp = x[temp_tmp];
    c_temp_tmp = (ix0 + k) - 1;
    d_temp_tmp = x[c_temp_tmp];
    x[temp_tmp] = c * b_temp_tmp - s * d_temp_tmp;
    x[c_temp_tmp] = c * d_temp_tmp + s * b_temp_tmp;
  }
}

/*
 * Arguments    : int n
 *                double x[36]
 *                int ix0
 *                int iy0
 *                double c
 *                double s
 * Return Type  : void
 */
void xrot(int n, double x[36], int ix0, int iy0, double c, double s)
{
  double b_temp_tmp;
  double d_temp_tmp;
  int c_temp_tmp;
  int k;
  int temp_tmp;
  if (n >= 1) {
    for (k = 0; k < n; k++) {
      temp_tmp = (iy0 + k) - 1;
      b_temp_tmp = x[temp_tmp];
      c_temp_tmp = (ix0 + k) - 1;
      d_temp_tmp = x[c_temp_tmp];
      x[temp_tmp] = c * b_temp_tmp - s * d_temp_tmp;
      x[c_temp_tmp] = c * d_temp_tmp + s * b_temp_tmp;
    }
  }
}

/*
 * File trailer for xrot.c
 *
 * [EOF]
 */
