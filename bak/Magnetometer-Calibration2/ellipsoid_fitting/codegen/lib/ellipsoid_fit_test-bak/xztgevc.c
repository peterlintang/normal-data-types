/*
 * File: xztgevc.c
 *
 * MATLAB Coder version            : 5.3
 * C/C++ source code generated on  : 02-Aug-2024 10:19:14
 */

/* Include Files */
#include "xztgevc.h"
#include "ellipsoid_fit_test_data.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : const creal_T A[9]
 *                creal_T V[9]
 * Return Type  : void
 */
void b_xztgevc(const creal_T A[9], creal_T V[9])
{
  creal_T work1[3];
  creal_T work2[3];
  double rworka[3];
  double acoeff;
  double ai;
  double anorm;
  double ascale;
  double brm;
  double d_im;
  double d_re;
  double dmin;
  double salpha_im;
  double salpha_re;
  double scale;
  double temp;
  double xmx;
  double y;
  int d_re_tmp;
  int i;
  int j;
  int je;
  bool lscalea;
  bool lscaleb;
  rworka[0] = 0.0;
  rworka[1] = 0.0;
  rworka[2] = 0.0;
  anorm = fabs(A[0].re) + fabs(A[0].im);
  for (j = 0; j < 2; j++) {
    for (i = 0; i <= j; i++) {
      d_re_tmp = i + 3 * (j + 1);
      rworka[j + 1] += fabs(A[d_re_tmp].re) + fabs(A[d_re_tmp].im);
    }
    i = (j + 3 * (j + 1)) + 1;
    y = rworka[j + 1] + (fabs(A[i].re) + fabs(A[i].im));
    if (y > anorm) {
      anorm = y;
    }
  }
  d_re = anorm;
  if (2.2250738585072014E-308 > anorm) {
    d_re = 2.2250738585072014E-308;
  }
  ascale = 1.0 / d_re;
  for (je = 2; je >= 0; je--) {
    i = je + 3 * je;
    xmx = A[i].re;
    y = A[i].im;
    d_re = (fabs(xmx) + fabs(y)) * ascale;
    if (1.0 > d_re) {
      d_re = 1.0;
    }
    temp = 1.0 / d_re;
    salpha_re = ascale * (temp * xmx);
    salpha_im = ascale * (temp * y);
    acoeff = temp * ascale;
    if ((temp >= 2.2250738585072014E-308) &&
        (acoeff < 3.0062525400134592E-292)) {
      lscalea = true;
    } else {
      lscalea = false;
    }
    xmx = fabs(salpha_re) + fabs(salpha_im);
    if ((xmx >= 2.2250738585072014E-308) && (xmx < 3.0062525400134592E-292)) {
      lscaleb = true;
    } else {
      lscaleb = false;
    }
    scale = 1.0;
    if (lscalea) {
      d_re = anorm;
      if (3.3264005158911995E+291 < anorm) {
        d_re = 3.3264005158911995E+291;
      }
      scale = 3.0062525400134592E-292 / temp * d_re;
    }
    if (lscaleb) {
      y = 3.0062525400134592E-292 / xmx;
      if (y > scale) {
        scale = y;
      }
    }
    if (lscalea || lscaleb) {
      d_re = acoeff;
      if (1.0 > acoeff) {
        d_re = 1.0;
      }
      if (xmx > d_re) {
        d_re = xmx;
      }
      y = 1.0 / (2.2250738585072014E-308 * d_re);
      if (y < scale) {
        scale = y;
      }
      if (lscalea) {
        acoeff = ascale * (scale * temp);
      } else {
        acoeff *= scale;
      }
      salpha_re *= scale;
      salpha_im *= scale;
    }
    work1[0].re = 0.0;
    work1[0].im = 0.0;
    work1[1].re = 0.0;
    work1[1].im = 0.0;
    work1[2].re = 0.0;
    work1[2].im = 0.0;
    work1[je].re = 1.0;
    work1[je].im = 0.0;
    dmin = 2.2204460492503131E-16 * acoeff * anorm;
    y = 2.2204460492503131E-16 * (fabs(salpha_re) + fabs(salpha_im));
    if (y > dmin) {
      dmin = y;
    }
    if (2.2250738585072014E-308 > dmin) {
      dmin = 2.2250738585072014E-308;
    }
    for (d_re_tmp = 0; d_re_tmp < je; d_re_tmp++) {
      i = d_re_tmp + 3 * je;
      work1[d_re_tmp].re = acoeff * A[i].re;
      work1[d_re_tmp].im = acoeff * A[i].im;
    }
    work1[je].re = 1.0;
    work1[je].im = 0.0;
    for (j = je; j >= 1; j--) {
      d_re_tmp = (j + 3 * (j - 1)) - 1;
      d_re = acoeff * A[d_re_tmp].re - salpha_re;
      d_im = acoeff * A[d_re_tmp].im - salpha_im;
      if (fabs(d_re) + fabs(d_im) <= dmin) {
        d_re = dmin;
        d_im = 0.0;
      }
      brm = fabs(d_re);
      y = fabs(d_im);
      xmx = brm + y;
      if (xmx < 1.0) {
        scale = fabs(work1[j - 1].re) + fabs(work1[j - 1].im);
        if (scale >= 1.4980776123852632E+307 * xmx) {
          temp = 1.0 / scale;
          for (d_re_tmp = 0; d_re_tmp <= je; d_re_tmp++) {
            work1[d_re_tmp].re *= temp;
            work1[d_re_tmp].im *= temp;
          }
        }
      }
      temp = -work1[j - 1].re;
      ai = -work1[j - 1].im;
      if (d_im == 0.0) {
        if (ai == 0.0) {
          y = temp / d_re;
          xmx = 0.0;
        } else if (temp == 0.0) {
          y = 0.0;
          xmx = ai / d_re;
        } else {
          y = temp / d_re;
          xmx = ai / d_re;
        }
      } else if (d_re == 0.0) {
        if (temp == 0.0) {
          y = ai / d_im;
          xmx = 0.0;
        } else if (ai == 0.0) {
          y = 0.0;
          xmx = -(temp / d_im);
        } else {
          y = ai / d_im;
          xmx = -(temp / d_im);
        }
      } else if (brm > y) {
        scale = d_im / d_re;
        xmx = d_re + scale * d_im;
        y = (temp + scale * ai) / xmx;
        xmx = (ai - scale * temp) / xmx;
      } else if (y == brm) {
        if (d_re > 0.0) {
          scale = 0.5;
        } else {
          scale = -0.5;
        }
        if (d_im > 0.0) {
          xmx = 0.5;
        } else {
          xmx = -0.5;
        }
        y = (temp * scale + ai * xmx) / brm;
        xmx = (ai * scale - temp * xmx) / brm;
      } else {
        scale = d_re / d_im;
        xmx = d_im + scale * d_re;
        y = (scale * temp + ai) / xmx;
        xmx = (scale * ai - temp) / xmx;
      }
      work1[j - 1].re = y;
      work1[j - 1].im = xmx;
      if (j > 1) {
        xmx = fabs(work1[1].re) + fabs(work1[1].im);
        if (xmx > 1.0) {
          temp = 1.0 / xmx;
          if (acoeff * rworka[1] >= 1.4980776123852632E+307 * temp) {
            for (d_re_tmp = 0; d_re_tmp <= je; d_re_tmp++) {
              work1[d_re_tmp].re *= temp;
              work1[d_re_tmp].im *= temp;
            }
          }
        }
        xmx = acoeff * work1[1].re;
        d_re = acoeff * work1[1].im;
        work1[0].re += xmx * A[3].re - d_re * A[3].im;
        work1[0].im += xmx * A[3].im + d_re * A[3].re;
      }
    }
    work2[0].re = 0.0;
    work2[0].im = 0.0;
    work2[1].re = 0.0;
    work2[1].im = 0.0;
    work2[2].re = 0.0;
    work2[2].im = 0.0;
    for (d_re_tmp = 0; d_re_tmp <= je; d_re_tmp++) {
      xmx = V[3 * d_re_tmp].re;
      y = work1[d_re_tmp].im;
      d_re = V[3 * d_re_tmp].im;
      scale = work1[d_re_tmp].re;
      work2[0].re += xmx * scale - d_re * y;
      work2[0].im += xmx * y + d_re * scale;
      i = 3 * d_re_tmp + 1;
      xmx = V[i].re;
      d_re = V[i].im;
      work2[1].re += xmx * scale - d_re * y;
      work2[1].im += xmx * y + d_re * scale;
      i = 3 * d_re_tmp + 2;
      xmx = V[i].re;
      d_re = V[i].im;
      work2[2].re += xmx * scale - d_re * y;
      work2[2].im += xmx * y + d_re * scale;
    }
    xmx = fabs(work2[0].re) + fabs(work2[0].im);
    y = fabs(work2[1].re) + fabs(work2[1].im);
    if (y > xmx) {
      xmx = y;
    }
    y = fabs(work2[2].re) + fabs(work2[2].im);
    if (y > xmx) {
      xmx = y;
    }
    if (xmx > 2.2250738585072014E-308) {
      temp = 1.0 / xmx;
      V[3 * je].re = temp * work2[0].re;
      V[3 * je].im = temp * work2[0].im;
      i = 3 * je + 1;
      V[i].re = temp * work2[1].re;
      V[i].im = temp * work2[1].im;
      i = 3 * je + 2;
      V[i].re = temp * work2[2].re;
      V[i].im = temp * work2[2].im;
    } else {
      V[3 * je].re = 0.0;
      V[3 * je].im = 0.0;
      i = 3 * je + 1;
      V[i].re = 0.0;
      V[i].im = 0.0;
      i = 3 * je + 2;
      V[i].re = 0.0;
      V[i].im = 0.0;
    }
  }
}

/*
 * Arguments    : const creal_T A[36]
 *                creal_T V[36]
 * Return Type  : void
 */
void xztgevc(const creal_T A[36], creal_T V[36])
{
  creal_T work1[6];
  creal_T work2[6];
  double rworka[6];
  double acoeff;
  double anorm;
  double ascale;
  double brm;
  double d_im;
  double d_re;
  double dmin;
  double salpha_im;
  double salpha_re;
  double scale;
  double temp;
  double x;
  double xmx;
  double y;
  int d_re_tmp;
  int i;
  int j;
  int je;
  int jr;
  bool lscalea;
  bool lscaleb;
  for (i = 0; i < 6; i++) {
    rworka[i] = 0.0;
  }
  anorm = fabs(A[0].re) + fabs(A[0].im);
  for (j = 0; j < 5; j++) {
    for (i = 0; i <= j; i++) {
      d_re_tmp = i + 6 * (j + 1);
      rworka[j + 1] += fabs(A[d_re_tmp].re) + fabs(A[d_re_tmp].im);
    }
    i = (j + 6 * (j + 1)) + 1;
    y = rworka[j + 1] + (fabs(A[i].re) + fabs(A[i].im));
    if (y > anorm) {
      anorm = y;
    }
  }
  x = anorm;
  if (2.2250738585072014E-308 > anorm) {
    x = 2.2250738585072014E-308;
  }
  ascale = 1.0 / x;
  for (je = 5; je >= 0; je--) {
    i = je + 6 * je;
    xmx = A[i].re;
    y = A[i].im;
    x = (fabs(xmx) + fabs(y)) * ascale;
    if (1.0 > x) {
      x = 1.0;
    }
    temp = 1.0 / x;
    salpha_re = ascale * (temp * xmx);
    salpha_im = ascale * (temp * y);
    acoeff = temp * ascale;
    if ((temp >= 2.2250738585072014E-308) &&
        (acoeff < 6.0125050800269183E-292)) {
      lscalea = true;
    } else {
      lscalea = false;
    }
    xmx = fabs(salpha_re) + fabs(salpha_im);
    if ((xmx >= 2.2250738585072014E-308) && (xmx < 6.0125050800269183E-292)) {
      lscaleb = true;
    } else {
      lscaleb = false;
    }
    scale = 1.0;
    if (lscalea) {
      x = anorm;
      if (1.6632002579455998E+291 < anorm) {
        x = 1.6632002579455998E+291;
      }
      scale = 6.0125050800269183E-292 / temp * x;
    }
    if (lscaleb) {
      y = 6.0125050800269183E-292 / xmx;
      if (y > scale) {
        scale = y;
      }
    }
    if (lscalea || lscaleb) {
      x = acoeff;
      if (1.0 > acoeff) {
        x = 1.0;
      }
      if (xmx > x) {
        x = xmx;
      }
      y = 1.0 / (2.2250738585072014E-308 * x);
      if (y < scale) {
        scale = y;
      }
      if (lscalea) {
        acoeff = ascale * (scale * temp);
      } else {
        acoeff *= scale;
      }
      salpha_re *= scale;
      salpha_im *= scale;
    }
    memset(&work1[0], 0, 6U * sizeof(creal_T));
    work1[je].re = 1.0;
    work1[je].im = 0.0;
    dmin = 2.2204460492503131E-16 * acoeff * anorm;
    y = 2.2204460492503131E-16 * (fabs(salpha_re) + fabs(salpha_im));
    if (y > dmin) {
      dmin = y;
    }
    if (2.2250738585072014E-308 > dmin) {
      dmin = 2.2250738585072014E-308;
    }
    for (jr = 0; jr < je; jr++) {
      i = jr + 6 * je;
      work1[jr].re = acoeff * A[i].re;
      work1[jr].im = acoeff * A[i].im;
    }
    work1[je].re = 1.0;
    work1[je].im = 0.0;
    for (j = je; j >= 1; j--) {
      i = 6 * (j - 1);
      d_re_tmp = (j + i) - 1;
      d_re = acoeff * A[d_re_tmp].re - salpha_re;
      d_im = acoeff * A[d_re_tmp].im - salpha_im;
      if (fabs(d_re) + fabs(d_im) <= dmin) {
        d_re = dmin;
        d_im = 0.0;
      }
      brm = fabs(d_re);
      y = fabs(d_im);
      xmx = brm + y;
      if (xmx < 1.0) {
        scale = fabs(work1[j - 1].re) + fabs(work1[j - 1].im);
        if (scale >= 7.4903880619263159E+306 * xmx) {
          temp = 1.0 / scale;
          for (jr = 0; jr <= je; jr++) {
            work1[jr].re *= temp;
            work1[jr].im *= temp;
          }
        }
      }
      x = work1[j - 1].re;
      temp = work1[j - 1].im;
      if (d_im == 0.0) {
        if (-temp == 0.0) {
          y = -x / d_re;
          xmx = 0.0;
        } else if (-x == 0.0) {
          y = 0.0;
          xmx = -temp / d_re;
        } else {
          y = -x / d_re;
          xmx = -temp / d_re;
        }
      } else if (d_re == 0.0) {
        if (-x == 0.0) {
          y = -temp / d_im;
          xmx = 0.0;
        } else if (-temp == 0.0) {
          y = 0.0;
          xmx = -(-x / d_im);
        } else {
          y = -temp / d_im;
          xmx = -(-x / d_im);
        }
      } else if (brm > y) {
        scale = d_im / d_re;
        xmx = d_re + scale * d_im;
        y = (-x + scale * -temp) / xmx;
        xmx = (-temp - scale * -x) / xmx;
      } else if (y == brm) {
        if (d_re > 0.0) {
          scale = 0.5;
        } else {
          scale = -0.5;
        }
        if (d_im > 0.0) {
          xmx = 0.5;
        } else {
          xmx = -0.5;
        }
        y = (-x * scale + -temp * xmx) / brm;
        xmx = (-temp * scale - -x * xmx) / brm;
      } else {
        scale = d_re / d_im;
        xmx = d_im + scale * d_re;
        y = (scale * -x + -temp) / xmx;
        xmx = (scale * -temp - (-x)) / xmx;
      }
      work1[j - 1].re = y;
      work1[j - 1].im = xmx;
      if (j > 1) {
        xmx = fabs(y) + fabs(xmx);
        if (xmx > 1.0) {
          temp = 1.0 / xmx;
          if (acoeff * rworka[j - 1] >= 7.4903880619263159E+306 * temp) {
            for (jr = 0; jr <= je; jr++) {
              work1[jr].re *= temp;
              work1[jr].im *= temp;
            }
          }
        }
        d_re = acoeff * work1[j - 1].re;
        d_im = acoeff * work1[j - 1].im;
        for (jr = 0; jr <= j - 2; jr++) {
          d_re_tmp = jr + i;
          xmx = A[d_re_tmp].im;
          x = A[d_re_tmp].re;
          work1[jr].re += d_re * x - d_im * xmx;
          work1[jr].im += d_re * xmx + d_im * x;
        }
      }
    }
    memset(&work2[0], 0, 6U * sizeof(creal_T));
    for (d_re_tmp = 0; d_re_tmp <= je; d_re_tmp++) {
      xmx = work1[d_re_tmp].im;
      y = work1[d_re_tmp].re;
      for (jr = 0; jr < 6; jr++) {
        i = jr + 6 * d_re_tmp;
        scale = V[i].re;
        x = V[i].im;
        work2[jr].re += scale * y - x * xmx;
        work2[jr].im += scale * xmx + x * y;
      }
    }
    xmx = fabs(work2[0].re) + fabs(work2[0].im);
    for (jr = 0; jr < 5; jr++) {
      y = fabs(work2[jr + 1].re) + fabs(work2[jr + 1].im);
      if (y > xmx) {
        xmx = y;
      }
    }
    if (xmx > 2.2250738585072014E-308) {
      temp = 1.0 / xmx;
      for (jr = 0; jr < 6; jr++) {
        i = jr + 6 * je;
        V[i].re = temp * work2[jr].re;
        V[i].im = temp * work2[jr].im;
      }
    } else {
      memset(&V[je * 6], 0, 6U * sizeof(creal_T));
    }
  }
}

/*
 * File trailer for xztgevc.c
 *
 * [EOF]
 */
