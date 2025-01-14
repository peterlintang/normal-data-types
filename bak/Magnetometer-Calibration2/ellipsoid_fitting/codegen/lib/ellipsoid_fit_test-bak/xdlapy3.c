/*
 * File: xdlapy3.c
 *
 * MATLAB Coder version            : 5.3
 * C/C++ source code generated on  : 02-Aug-2024 10:19:14
 */

/* Include Files */
#include "xdlapy3.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
/*
 * Arguments    : double x1
 *                double x2
 *                double x3
 * Return Type  : double
 */
double xdlapy3(double x1, double x2, double x3)
{
  double a;
  double b;
  double c;
  double y;
  a = fabs(x1);
  b = fabs(x2);
  c = fabs(x3);
  y = fmax(a, b);
  if (c > y) {
    y = c;
  }
  if ((y > 0.0) && (!rtIsInf(y))) {
    a /= y;
    b /= y;
    c /= y;
    y *= sqrt((a * a + c * c) + b * b);
  } else {
    y = (a + b) + c;
  }
  return y;
}

/*
 * File trailer for xdlapy3.c
 *
 * [EOF]
 */
