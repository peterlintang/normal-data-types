/*
 * File: ellipsoid_fit_test.c
 *
 * MATLAB Coder version            : 5.3
 * C/C++ source code generated on  : 02-Aug-2024 10:19:14
 */

/* Include Files */
#include "ellipsoid_fit_test.h"
#include "ellipsoid_fit.h"
#include "ellipsoid_fit_test_data.h"
#include "ellipsoid_fit_test_types.h"
#include "rt_nonfinite.h"
#include "schur.h"
#include "sqrt.h"
#include "xzggev.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : const emxArray_real_T *x_m
 *                const emxArray_real_T *y_m
 *                const emxArray_real_T *z_m
 *                creal_T v[10]
 *                creal_T offset[3]
 *                creal_T gain[9]
 *                creal_T matrix[9]
 * Return Type  : void
 */
void ellipsoid_fit_test(const emxArray_real_T *x_m, const emxArray_real_T *y_m,
                        const emxArray_real_T *z_m, creal_T v[10],
                        creal_T offset[3], creal_T gain[9], creal_T matrix[9])
{
  creal_T M[9];
  creal_T M_[9];
  creal_T V[9];
  creal_T b_V[9];
  creal_T beta1[3];
  creal_T pqr_[3];
  creal_T u[3];
  creal_T ax__tmp;
  creal_T dc;
  creal_T dc1;
  double V_im;
  double V_re;
  double V_re_tmp;
  double absxk;
  double ai;
  double ax__tmp_im;
  double ax__tmp_re;
  double ax__tmp_tmp;
  double b_V_re_tmp;
  double b_ax__tmp_im;
  double b_ax__tmp_re;
  double b_ax__tmp_tmp;
  double c_V_re_tmp;
  double c_ax__tmp_re;
  double colnorm;
  double d_V_re_tmp;
  double e_V_re_tmp;
  double im;
  double re;
  double scale;
  double t;
  int exitg1;
  int i;
  int r1;
  int r2;
  int r3;
  int rtemp;
  bool exitg2;
  bool p;
  ellipsoid_fit(x_m, y_m, z_m, v);
  M[0] = v[0];
  M[3] = v[5];
  M[6] = v[4];
  M[1] = v[5];
  M[4] = v[1];
  M[7] = v[3];
  M[2] = v[4];
  M[5] = v[3];
  M[8] = v[2];
  /*  Original ellipsoid matrix  */
  u[0].re = v[6].re;
  u[0].im = -v[6].im;
  u[1].re = v[7].re;
  u[1].im = -v[7].im;
  u[2].re = v[8].re;
  u[2].im = -v[8].im;
  p = true;
  for (r2 = 0; r2 < 9; r2++) {
    if ((!p) || (rtIsInf(M[r2].re) || rtIsInf(M[r2].im) ||
                 (rtIsNaN(M[r2].re) || rtIsNaN(M[r2].im)))) {
      p = false;
    }
  }
  if (!p) {
    for (rtemp = 0; rtemp < 9; rtemp++) {
      V[rtemp].re = rtNaN;
      V[rtemp].im = 0.0;
    }
  } else {
    p = true;
    rtemp = 0;
    exitg2 = false;
    while ((!exitg2) && (rtemp < 3)) {
      r1 = 0;
      do {
        exitg1 = 0;
        if (r1 <= rtemp) {
          r2 = rtemp + 3 * r1;
          r3 = r1 + 3 * rtemp;
          if ((!(M[r3].re == M[r2].re)) || (!(M[r3].im == -M[r2].im))) {
            p = false;
            exitg1 = 1;
          } else {
            r1++;
          }
        } else {
          rtemp++;
          exitg1 = 2;
        }
      } while (exitg1 == 0);
      if (exitg1 == 1) {
        exitg2 = true;
      }
    }
    if (p) {
      schur(M, V, M_);
    } else {
      p = true;
      rtemp = 0;
      exitg2 = false;
      while ((!exitg2) && (rtemp < 3)) {
        r1 = 0;
        do {
          exitg1 = 0;
          if (r1 <= rtemp) {
            r2 = rtemp + 3 * r1;
            r3 = r1 + 3 * rtemp;
            if ((!(M[r3].re == -M[r2].re)) || (!(M[r3].im == M[r2].im))) {
              p = false;
              exitg1 = 1;
            } else {
              r1++;
            }
          } else {
            rtemp++;
            exitg1 = 2;
          }
        } while (exitg1 == 0);
        if (exitg1 == 1) {
          exitg2 = true;
        }
      }
      if (p) {
        for (rtemp = 0; rtemp < 9; rtemp++) {
          ax__tmp_re = M[rtemp].im;
          V_re = M[rtemp].re;
          b_V[rtemp].re = 0.0 * V_re - ax__tmp_re;
          b_V[rtemp].im = 0.0 * ax__tmp_re + V_re;
        }
        schur(b_V, V, M_);
      } else {
        memcpy(&M_[0], &M[0], 9U * sizeof(creal_T));
        b_xzggev(M_, &r1, pqr_, beta1, V);
        for (r3 = 0; r3 <= 6; r3 += 3) {
          colnorm = 0.0;
          scale = 3.3121686421112381E-170;
          r1 = r3 + 3;
          for (r2 = r3 + 1; r2 <= r1; r2++) {
            absxk = fabs(V[r2 - 1].re);
            if (absxk > scale) {
              t = scale / absxk;
              colnorm = colnorm * t * t + 1.0;
              scale = absxk;
            } else {
              t = absxk / scale;
              colnorm += t * t;
            }
            absxk = fabs(V[r2 - 1].im);
            if (absxk > scale) {
              t = scale / absxk;
              colnorm = colnorm * t * t + 1.0;
              scale = absxk;
            } else {
              t = absxk / scale;
              colnorm += t * t;
            }
          }
          colnorm = scale * sqrt(colnorm);
          for (rtemp = r3 + 1; rtemp <= r1; rtemp++) {
            t = V[rtemp - 1].re;
            ai = V[rtemp - 1].im;
            if (ai == 0.0) {
              re = t / colnorm;
              im = 0.0;
            } else if (t == 0.0) {
              re = 0.0;
              im = ai / colnorm;
            } else {
              re = t / colnorm;
              im = ai / colnorm;
            }
            V[rtemp - 1].re = re;
            V[rtemp - 1].im = im;
          }
        }
      }
    }
  }
  /*  Compute eigenvectors matrix */
  /*  DCM = eigenvectors matrix */
  /*  Diagonalize M */
  beta1[0] = v[6];
  beta1[1] = v[7];
  beta1[2] = v[8];
  for (rtemp = 0; rtemp < 3; rtemp++) {
    scale = V[3 * rtemp].re;
    ax__tmp_re = V[3 * rtemp].im;
    r2 = 3 * rtemp + 1;
    r3 = 3 * rtemp + 2;
    for (i = 0; i < 3; i++) {
      V_re_tmp = M[3 * i].im;
      b_V_re_tmp = M[3 * i].re;
      V_re = V[r2].re;
      V_im = -V[r2].im;
      r1 = 3 * i + 1;
      c_V_re_tmp = M[r1].im;
      d_V_re_tmp = M[r1].re;
      re = (scale * b_V_re_tmp - -ax__tmp_re * V_re_tmp) +
           (V_re * d_V_re_tmp - V_im * c_V_re_tmp);
      im = (scale * V_re_tmp + -ax__tmp_re * b_V_re_tmp) +
           (V_re * c_V_re_tmp + V_im * d_V_re_tmp);
      V_re = V[r3].re;
      V_im = -V[r3].im;
      r1 = 3 * i + 2;
      V_re_tmp = M[r1].im;
      b_V_re_tmp = M[r1].re;
      re += V_re * b_V_re_tmp - V_im * V_re_tmp;
      im += V_re * V_re_tmp + V_im * b_V_re_tmp;
      r1 = rtemp + 3 * i;
      b_V[r1].re = re;
      b_V[r1].im = im;
    }
    V_re_tmp = b_V[rtemp].re;
    b_V_re_tmp = b_V[rtemp].im;
    c_V_re_tmp = b_V[rtemp + 3].re;
    d_V_re_tmp = b_V[rtemp + 3].im;
    V_im = b_V[rtemp + 6].re;
    im = b_V[rtemp + 6].im;
    ax__tmp_re = 0.0;
    V_re = 0.0;
    for (i = 0; i < 3; i++) {
      scale = V[3 * i].im;
      absxk = V[3 * i].re;
      r2 = 3 * i + 1;
      t = V[r2].im;
      colnorm = V[r2].re;
      r2 = 3 * i + 2;
      ai = V[r2].im;
      e_V_re_tmp = V[r2].re;
      r1 = rtemp + 3 * i;
      M_[r1].re = ((V_re_tmp * absxk - b_V_re_tmp * scale) +
                   (c_V_re_tmp * colnorm - d_V_re_tmp * t)) +
                  (V_im * e_V_re_tmp - im * ai);
      M_[r1].im = ((V_re_tmp * scale + b_V_re_tmp * absxk) +
                   (c_V_re_tmp * t + d_V_re_tmp * colnorm)) +
                  (V_im * ai + im * e_V_re_tmp);
      r1 = i + 3 * rtemp;
      scale = beta1[i].re;
      absxk = V[r1].im;
      t = beta1[i].im;
      colnorm = V[r1].re;
      ax__tmp_re += scale * colnorm - t * absxk;
      V_re += scale * absxk + t * colnorm;
    }
    pqr_[rtemp].im = V_re;
    pqr_[rtemp].re = ax__tmp_re;
  }
  ax__tmp_tmp = pqr_[0].re * pqr_[0].re - pqr_[0].im * pqr_[0].im;
  b_ax__tmp_tmp = pqr_[0].re * pqr_[0].im;
  b_ax__tmp_tmp += b_ax__tmp_tmp;
  b_ax__tmp_re = pqr_[1].re * pqr_[1].re - pqr_[1].im * pqr_[1].im;
  scale = pqr_[1].re * pqr_[1].im;
  ax__tmp_im = scale + scale;
  c_ax__tmp_re = pqr_[2].re * pqr_[2].re - pqr_[2].im * pqr_[2].im;
  scale = pqr_[2].re * pqr_[2].im;
  b_ax__tmp_im = scale + scale;
  for (rtemp = 0; rtemp < 9; rtemp++) {
    M[rtemp].re = -M[rtemp].re;
    M[rtemp].im = -M[rtemp].im;
  }
  r1 = 0;
  r2 = 1;
  r3 = 2;
  scale = fabs(M[0].re) + fabs(M[0].im);
  absxk = fabs(M[1].re) + fabs(M[1].im);
  if (absxk > scale) {
    scale = absxk;
    r1 = 1;
    r2 = 0;
  }
  if (fabs(M[2].re) + fabs(M[2].im) > scale) {
    r1 = 2;
    r2 = 1;
    r3 = 0;
  }
  t = M[r2].re;
  ai = M[r2].im;
  scale = M[r1].re;
  absxk = M[r1].im;
  if (absxk == 0.0) {
    if (ai == 0.0) {
      re = t / scale;
      im = 0.0;
    } else if (t == 0.0) {
      re = 0.0;
      im = ai / scale;
    } else {
      re = t / scale;
      im = ai / scale;
    }
  } else if (scale == 0.0) {
    if (t == 0.0) {
      re = ai / absxk;
      im = 0.0;
    } else if (ai == 0.0) {
      re = 0.0;
      im = -(t / absxk);
    } else {
      re = ai / absxk;
      im = -(t / absxk);
    }
  } else {
    im = fabs(scale);
    c_V_re_tmp = fabs(absxk);
    if (im > c_V_re_tmp) {
      c_V_re_tmp = absxk / scale;
      scale += c_V_re_tmp * absxk;
      re = (t + c_V_re_tmp * ai) / scale;
      im = (ai - c_V_re_tmp * t) / scale;
    } else if (c_V_re_tmp == im) {
      if (scale > 0.0) {
        c_V_re_tmp = 0.5;
      } else {
        c_V_re_tmp = -0.5;
      }
      if (absxk > 0.0) {
        scale = 0.5;
      } else {
        scale = -0.5;
      }
      re = (t * c_V_re_tmp + ai * scale) / im;
      im = (ai * c_V_re_tmp - t * scale) / im;
    } else {
      c_V_re_tmp = scale / absxk;
      scale = absxk + c_V_re_tmp * scale;
      re = (c_V_re_tmp * t + ai) / scale;
      im = (c_V_re_tmp * ai - t) / scale;
    }
  }
  M[r2].re = re;
  M[r2].im = im;
  t = M[r3].re;
  ai = M[r3].im;
  scale = M[r1].re;
  absxk = M[r1].im;
  if (absxk == 0.0) {
    if (ai == 0.0) {
      re = t / scale;
      im = 0.0;
    } else if (t == 0.0) {
      re = 0.0;
      im = ai / scale;
    } else {
      re = t / scale;
      im = ai / scale;
    }
  } else if (scale == 0.0) {
    if (t == 0.0) {
      re = ai / absxk;
      im = 0.0;
    } else if (ai == 0.0) {
      re = 0.0;
      im = -(t / absxk);
    } else {
      re = ai / absxk;
      im = -(t / absxk);
    }
  } else {
    im = fabs(scale);
    c_V_re_tmp = fabs(absxk);
    if (im > c_V_re_tmp) {
      c_V_re_tmp = absxk / scale;
      scale += c_V_re_tmp * absxk;
      re = (t + c_V_re_tmp * ai) / scale;
      im = (ai - c_V_re_tmp * t) / scale;
    } else if (c_V_re_tmp == im) {
      if (scale > 0.0) {
        c_V_re_tmp = 0.5;
      } else {
        c_V_re_tmp = -0.5;
      }
      if (absxk > 0.0) {
        scale = 0.5;
      } else {
        scale = -0.5;
      }
      re = (t * c_V_re_tmp + ai * scale) / im;
      im = (ai * c_V_re_tmp - t * scale) / im;
    } else {
      c_V_re_tmp = scale / absxk;
      scale = absxk + c_V_re_tmp * scale;
      re = (c_V_re_tmp * t + ai) / scale;
      im = (c_V_re_tmp * ai - t) / scale;
    }
  }
  M[r3].re = re;
  M[r3].im = im;
  scale = M[r2].re;
  absxk = M[r1 + 3].im;
  t = M[r2].im;
  colnorm = M[r1 + 3].re;
  M[r2 + 3].re -= scale * colnorm - t * absxk;
  M[r2 + 3].im -= scale * absxk + t * colnorm;
  scale = M[r3].re;
  absxk = M[r1 + 3].im;
  t = M[r3].im;
  colnorm = M[r1 + 3].re;
  M[r3 + 3].re -= scale * colnorm - t * absxk;
  M[r3 + 3].im -= scale * absxk + t * colnorm;
  scale = M[r2].re;
  absxk = M[r1 + 6].im;
  t = M[r2].im;
  colnorm = M[r1 + 6].re;
  M[r2 + 6].re -= scale * colnorm - t * absxk;
  M[r2 + 6].im -= scale * absxk + t * colnorm;
  scale = M[r3].re;
  absxk = M[r1 + 6].im;
  t = M[r3].im;
  colnorm = M[r1 + 6].re;
  M[r3 + 6].re -= scale * colnorm - t * absxk;
  M[r3 + 6].im -= scale * absxk + t * colnorm;
  if (fabs(M[r3 + 3].re) + fabs(M[r3 + 3].im) >
      fabs(M[r2 + 3].re) + fabs(M[r2 + 3].im)) {
    rtemp = r2;
    r2 = r3;
    r3 = rtemp;
  }
  t = M[r3 + 3].re;
  ai = M[r3 + 3].im;
  scale = M[r2 + 3].re;
  absxk = M[r2 + 3].im;
  if (absxk == 0.0) {
    if (ai == 0.0) {
      re = t / scale;
      im = 0.0;
    } else if (t == 0.0) {
      re = 0.0;
      im = ai / scale;
    } else {
      re = t / scale;
      im = ai / scale;
    }
  } else if (scale == 0.0) {
    if (t == 0.0) {
      re = ai / absxk;
      im = 0.0;
    } else if (ai == 0.0) {
      re = 0.0;
      im = -(t / absxk);
    } else {
      re = ai / absxk;
      im = -(t / absxk);
    }
  } else {
    im = fabs(scale);
    c_V_re_tmp = fabs(absxk);
    if (im > c_V_re_tmp) {
      c_V_re_tmp = absxk / scale;
      scale += c_V_re_tmp * absxk;
      re = (t + c_V_re_tmp * ai) / scale;
      im = (ai - c_V_re_tmp * t) / scale;
    } else if (c_V_re_tmp == im) {
      if (scale > 0.0) {
        c_V_re_tmp = 0.5;
      } else {
        c_V_re_tmp = -0.5;
      }
      if (absxk > 0.0) {
        scale = 0.5;
      } else {
        scale = -0.5;
      }
      re = (t * c_V_re_tmp + ai * scale) / im;
      im = (ai * c_V_re_tmp - t * scale) / im;
    } else {
      c_V_re_tmp = scale / absxk;
      scale = absxk + c_V_re_tmp * scale;
      re = (c_V_re_tmp * t + ai) / scale;
      im = (c_V_re_tmp * ai - t) / scale;
    }
  }
  M[r3 + 3].re = re;
  M[r3 + 3].im = im;
  scale = M[r3 + 3].re;
  absxk = M[r2 + 6].im;
  t = M[r3 + 3].im;
  colnorm = M[r2 + 6].re;
  M[r3 + 6].re -= scale * colnorm - t * absxk;
  M[r3 + 6].im -= scale * absxk + t * colnorm;
  e_V_re_tmp = u[r1].re;
  scale = M[r2].im;
  V_re = u[r1].im;
  absxk = M[r2].re;
  offset[1].re = u[r2].re - (e_V_re_tmp * absxk - V_re * scale);
  offset[1].im = u[r2].im - (e_V_re_tmp * scale + V_re * absxk);
  scale = M[r3].im;
  absxk = M[r3].re;
  t = M[r3 + 3].im;
  colnorm = M[r3 + 3].re;
  offset[2].re = (u[r3].re - (e_V_re_tmp * absxk - V_re * scale)) -
                 (offset[1].re * colnorm - offset[1].im * t);
  offset[2].im = (u[r3].im - (e_V_re_tmp * scale + V_re * absxk)) -
                 (offset[1].re * t + offset[1].im * colnorm);
  scale = M[r3 + 6].re;
  absxk = M[r3 + 6].im;
  if (absxk == 0.0) {
    if (offset[2].im == 0.0) {
      re = offset[2].re / scale;
      im = 0.0;
    } else if (offset[2].re == 0.0) {
      re = 0.0;
      im = offset[2].im / scale;
    } else {
      re = offset[2].re / scale;
      im = offset[2].im / scale;
    }
  } else if (scale == 0.0) {
    if (offset[2].re == 0.0) {
      re = offset[2].im / absxk;
      im = 0.0;
    } else if (offset[2].im == 0.0) {
      re = 0.0;
      im = -(offset[2].re / absxk);
    } else {
      re = offset[2].im / absxk;
      im = -(offset[2].re / absxk);
    }
  } else {
    im = fabs(scale);
    c_V_re_tmp = fabs(absxk);
    if (im > c_V_re_tmp) {
      c_V_re_tmp = absxk / scale;
      scale += c_V_re_tmp * absxk;
      re = (offset[2].re + c_V_re_tmp * offset[2].im) / scale;
      im = (offset[2].im - c_V_re_tmp * offset[2].re) / scale;
    } else if (c_V_re_tmp == im) {
      if (scale > 0.0) {
        c_V_re_tmp = 0.5;
      } else {
        c_V_re_tmp = -0.5;
      }
      if (absxk > 0.0) {
        scale = 0.5;
      } else {
        scale = -0.5;
      }
      re = (offset[2].re * c_V_re_tmp + offset[2].im * scale) / im;
      im = (offset[2].im * c_V_re_tmp - offset[2].re * scale) / im;
    } else {
      c_V_re_tmp = scale / absxk;
      scale = absxk + c_V_re_tmp * scale;
      re = (c_V_re_tmp * offset[2].re + offset[2].im) / scale;
      im = (c_V_re_tmp * offset[2].im - offset[2].re) / scale;
    }
  }
  offset[2].re = re;
  offset[2].im = im;
  t = M[r1 + 6].im;
  colnorm = M[r1 + 6].re;
  offset[0].re = e_V_re_tmp - (re * colnorm - im * t);
  offset[0].im = V_re - (re * t + im * colnorm);
  t = M[r2 + 6].im;
  colnorm = M[r2 + 6].re;
  offset[1].re -= re * colnorm - im * t;
  offset[1].im -= re * t + im * colnorm;
  scale = M[r2 + 3].re;
  absxk = M[r2 + 3].im;
  if (absxk == 0.0) {
    if (offset[1].im == 0.0) {
      re = offset[1].re / scale;
      im = 0.0;
    } else if (offset[1].re == 0.0) {
      re = 0.0;
      im = offset[1].im / scale;
    } else {
      re = offset[1].re / scale;
      im = offset[1].im / scale;
    }
  } else if (scale == 0.0) {
    if (offset[1].re == 0.0) {
      re = offset[1].im / absxk;
      im = 0.0;
    } else if (offset[1].im == 0.0) {
      re = 0.0;
      im = -(offset[1].re / absxk);
    } else {
      re = offset[1].im / absxk;
      im = -(offset[1].re / absxk);
    }
  } else {
    im = fabs(scale);
    c_V_re_tmp = fabs(absxk);
    if (im > c_V_re_tmp) {
      c_V_re_tmp = absxk / scale;
      scale += c_V_re_tmp * absxk;
      re = (offset[1].re + c_V_re_tmp * offset[1].im) / scale;
      im = (offset[1].im - c_V_re_tmp * offset[1].re) / scale;
    } else if (c_V_re_tmp == im) {
      if (scale > 0.0) {
        c_V_re_tmp = 0.5;
      } else {
        c_V_re_tmp = -0.5;
      }
      if (absxk > 0.0) {
        scale = 0.5;
      } else {
        scale = -0.5;
      }
      re = (offset[1].re * c_V_re_tmp + offset[1].im * scale) / im;
      im = (offset[1].im * c_V_re_tmp - offset[1].re * scale) / im;
    } else {
      c_V_re_tmp = scale / absxk;
      scale = absxk + c_V_re_tmp * scale;
      re = (c_V_re_tmp * offset[1].re + offset[1].im) / scale;
      im = (c_V_re_tmp * offset[1].im - offset[1].re) / scale;
    }
  }
  offset[1].re = re;
  offset[1].im = im;
  t = M[r1 + 3].im;
  colnorm = M[r1 + 3].re;
  offset[0].re -= re * colnorm - im * t;
  offset[0].im -= re * t + im * colnorm;
  scale = M[r1].re;
  absxk = M[r1].im;
  if (absxk == 0.0) {
    if (offset[0].im == 0.0) {
      re = offset[0].re / scale;
      im = 0.0;
    } else if (offset[0].re == 0.0) {
      re = 0.0;
      im = offset[0].im / scale;
    } else {
      re = offset[0].re / scale;
      im = offset[0].im / scale;
    }
  } else if (scale == 0.0) {
    if (offset[0].re == 0.0) {
      re = offset[0].im / absxk;
      im = 0.0;
    } else if (offset[0].im == 0.0) {
      re = 0.0;
      im = -(offset[0].re / absxk);
    } else {
      re = offset[0].im / absxk;
      im = -(offset[0].re / absxk);
    }
  } else {
    im = fabs(scale);
    c_V_re_tmp = fabs(absxk);
    if (im > c_V_re_tmp) {
      c_V_re_tmp = absxk / scale;
      scale += c_V_re_tmp * absxk;
      re = (offset[0].re + c_V_re_tmp * offset[0].im) / scale;
      im = (offset[0].im - c_V_re_tmp * offset[0].re) / scale;
    } else if (c_V_re_tmp == im) {
      if (scale > 0.0) {
        c_V_re_tmp = 0.5;
      } else {
        c_V_re_tmp = -0.5;
      }
      if (absxk > 0.0) {
        scale = 0.5;
      } else {
        scale = -0.5;
      }
      re = (offset[0].re * c_V_re_tmp + offset[0].im * scale) / im;
      im = (offset[0].im * c_V_re_tmp - offset[0].re * scale) / im;
    } else {
      c_V_re_tmp = scale / absxk;
      scale = absxk + c_V_re_tmp * scale;
      re = (c_V_re_tmp * offset[0].re + offset[0].im) / scale;
      im = (c_V_re_tmp * offset[0].im - offset[0].re) / scale;
    }
  }
  offset[0].re = re;
  offset[0].im = im;
  /*  Eqn(21) */
  scale = M_[0].re * M_[0].re - M_[0].im * M_[0].im;
  V_im = M_[0].re * M_[0].im;
  absxk = V_im + V_im;
  if (absxk == 0.0) {
    if (b_ax__tmp_tmp == 0.0) {
      ai = ax__tmp_tmp / scale;
      d_V_re_tmp = 0.0;
    } else if (ax__tmp_tmp == 0.0) {
      ai = 0.0;
      d_V_re_tmp = b_ax__tmp_tmp / scale;
    } else {
      ai = ax__tmp_tmp / scale;
      d_V_re_tmp = b_ax__tmp_tmp / scale;
    }
  } else if (scale == 0.0) {
    if (ax__tmp_tmp == 0.0) {
      ai = b_ax__tmp_tmp / absxk;
      d_V_re_tmp = 0.0;
    } else if (b_ax__tmp_tmp == 0.0) {
      ai = 0.0;
      d_V_re_tmp = -(ax__tmp_tmp / absxk);
    } else {
      ai = b_ax__tmp_tmp / absxk;
      d_V_re_tmp = -(ax__tmp_tmp / absxk);
    }
  } else {
    im = fabs(scale);
    c_V_re_tmp = fabs(absxk);
    if (im > c_V_re_tmp) {
      c_V_re_tmp = absxk / scale;
      scale += c_V_re_tmp * absxk;
      ai = (ax__tmp_tmp + c_V_re_tmp * b_ax__tmp_tmp) / scale;
      d_V_re_tmp = (b_ax__tmp_tmp - c_V_re_tmp * ax__tmp_tmp) / scale;
    } else if (c_V_re_tmp == im) {
      if (scale > 0.0) {
        c_V_re_tmp = 0.5;
      } else {
        c_V_re_tmp = -0.5;
      }
      if (absxk > 0.0) {
        scale = 0.5;
      } else {
        scale = -0.5;
      }
      ai = (ax__tmp_tmp * c_V_re_tmp + b_ax__tmp_tmp * scale) / im;
      d_V_re_tmp = (b_ax__tmp_tmp * c_V_re_tmp - ax__tmp_tmp * scale) / im;
    } else {
      c_V_re_tmp = scale / absxk;
      scale = absxk + c_V_re_tmp * scale;
      ai = (c_V_re_tmp * ax__tmp_tmp + b_ax__tmp_tmp) / scale;
      d_V_re_tmp = (c_V_re_tmp * b_ax__tmp_tmp - ax__tmp_tmp) / scale;
    }
  }
  t = M_[0].re * M_[4].re - M_[0].im * M_[4].im;
  V_im = M_[0].re * M_[4].im + M_[0].im * M_[4].re;
  if (V_im == 0.0) {
    if (ax__tmp_im == 0.0) {
      ax__tmp_re = b_ax__tmp_re / t;
      colnorm = 0.0;
    } else if (b_ax__tmp_re == 0.0) {
      ax__tmp_re = 0.0;
      colnorm = ax__tmp_im / t;
    } else {
      ax__tmp_re = b_ax__tmp_re / t;
      colnorm = ax__tmp_im / t;
    }
  } else if (t == 0.0) {
    if (b_ax__tmp_re == 0.0) {
      ax__tmp_re = ax__tmp_im / V_im;
      colnorm = 0.0;
    } else if (ax__tmp_im == 0.0) {
      ax__tmp_re = 0.0;
      colnorm = -(b_ax__tmp_re / V_im);
    } else {
      ax__tmp_re = ax__tmp_im / V_im;
      colnorm = -(b_ax__tmp_re / V_im);
    }
  } else {
    im = fabs(t);
    c_V_re_tmp = fabs(V_im);
    if (im > c_V_re_tmp) {
      c_V_re_tmp = V_im / t;
      scale = t + c_V_re_tmp * V_im;
      ax__tmp_re = (b_ax__tmp_re + c_V_re_tmp * ax__tmp_im) / scale;
      colnorm = (ax__tmp_im - c_V_re_tmp * b_ax__tmp_re) / scale;
    } else if (c_V_re_tmp == im) {
      if (t > 0.0) {
        c_V_re_tmp = 0.5;
      } else {
        c_V_re_tmp = -0.5;
      }
      if (V_im > 0.0) {
        scale = 0.5;
      } else {
        scale = -0.5;
      }
      ax__tmp_re = (b_ax__tmp_re * c_V_re_tmp + ax__tmp_im * scale) / im;
      colnorm = (ax__tmp_im * c_V_re_tmp - b_ax__tmp_re * scale) / im;
    } else {
      c_V_re_tmp = t / V_im;
      scale = V_im + c_V_re_tmp * t;
      ax__tmp_re = (c_V_re_tmp * b_ax__tmp_re + ax__tmp_im) / scale;
      colnorm = (c_V_re_tmp * ax__tmp_im - b_ax__tmp_re) / scale;
    }
  }
  V_re_tmp = M_[0].re * M_[8].re - M_[0].im * M_[8].im;
  b_V_re_tmp = M_[0].re * M_[8].im + M_[0].im * M_[8].re;
  if (b_V_re_tmp == 0.0) {
    if (b_ax__tmp_im == 0.0) {
      e_V_re_tmp = c_ax__tmp_re / V_re_tmp;
      V_re = 0.0;
    } else if (c_ax__tmp_re == 0.0) {
      e_V_re_tmp = 0.0;
      V_re = b_ax__tmp_im / V_re_tmp;
    } else {
      e_V_re_tmp = c_ax__tmp_re / V_re_tmp;
      V_re = b_ax__tmp_im / V_re_tmp;
    }
  } else if (V_re_tmp == 0.0) {
    if (c_ax__tmp_re == 0.0) {
      e_V_re_tmp = b_ax__tmp_im / b_V_re_tmp;
      V_re = 0.0;
    } else if (b_ax__tmp_im == 0.0) {
      e_V_re_tmp = 0.0;
      V_re = -(c_ax__tmp_re / b_V_re_tmp);
    } else {
      e_V_re_tmp = b_ax__tmp_im / b_V_re_tmp;
      V_re = -(c_ax__tmp_re / b_V_re_tmp);
    }
  } else {
    im = fabs(V_re_tmp);
    c_V_re_tmp = fabs(b_V_re_tmp);
    if (im > c_V_re_tmp) {
      c_V_re_tmp = b_V_re_tmp / V_re_tmp;
      scale = V_re_tmp + c_V_re_tmp * b_V_re_tmp;
      e_V_re_tmp = (c_ax__tmp_re + c_V_re_tmp * b_ax__tmp_im) / scale;
      V_re = (b_ax__tmp_im - c_V_re_tmp * c_ax__tmp_re) / scale;
    } else if (c_V_re_tmp == im) {
      if (V_re_tmp > 0.0) {
        c_V_re_tmp = 0.5;
      } else {
        c_V_re_tmp = -0.5;
      }
      if (b_V_re_tmp > 0.0) {
        scale = 0.5;
      } else {
        scale = -0.5;
      }
      e_V_re_tmp = (c_ax__tmp_re * c_V_re_tmp + b_ax__tmp_im * scale) / im;
      V_re = (b_ax__tmp_im * c_V_re_tmp - c_ax__tmp_re * scale) / im;
    } else {
      c_V_re_tmp = V_re_tmp / b_V_re_tmp;
      scale = b_V_re_tmp + c_V_re_tmp * V_re_tmp;
      e_V_re_tmp = (c_V_re_tmp * c_ax__tmp_re + b_ax__tmp_im) / scale;
      V_re = (c_V_re_tmp * b_ax__tmp_im - c_ax__tmp_re) / scale;
    }
  }
  if (M_[0].im == 0.0) {
    if (v[9].im == 0.0) {
      absxk = v[9].re / M_[0].re;
      scale = 0.0;
    } else if (v[9].re == 0.0) {
      absxk = 0.0;
      scale = v[9].im / M_[0].re;
    } else {
      absxk = v[9].re / M_[0].re;
      scale = v[9].im / M_[0].re;
    }
  } else if (M_[0].re == 0.0) {
    if (v[9].re == 0.0) {
      absxk = v[9].im / M_[0].im;
      scale = 0.0;
    } else if (v[9].im == 0.0) {
      absxk = 0.0;
      scale = -(v[9].re / M_[0].im);
    } else {
      absxk = v[9].im / M_[0].im;
      scale = -(v[9].re / M_[0].im);
    }
  } else {
    im = fabs(M_[0].re);
    c_V_re_tmp = fabs(M_[0].im);
    if (im > c_V_re_tmp) {
      c_V_re_tmp = M_[0].im / M_[0].re;
      scale = M_[0].re + c_V_re_tmp * M_[0].im;
      absxk = (v[9].re + c_V_re_tmp * v[9].im) / scale;
      scale = (v[9].im - c_V_re_tmp * v[9].re) / scale;
    } else if (c_V_re_tmp == im) {
      if (M_[0].re > 0.0) {
        c_V_re_tmp = 0.5;
      } else {
        c_V_re_tmp = -0.5;
      }
      if (M_[0].im > 0.0) {
        scale = 0.5;
      } else {
        scale = -0.5;
      }
      absxk = (v[9].re * c_V_re_tmp + v[9].im * scale) / im;
      scale = (v[9].im * c_V_re_tmp - v[9].re * scale) / im;
    } else {
      c_V_re_tmp = M_[0].re / M_[0].im;
      scale = M_[0].im + c_V_re_tmp * M_[0].re;
      absxk = (c_V_re_tmp * v[9].re + v[9].im) / scale;
      scale = (c_V_re_tmp * v[9].im - v[9].re) / scale;
    }
  }
  dc.re = ((ai + ax__tmp_re) + e_V_re_tmp) - absxk;
  dc.im = ((d_V_re_tmp + colnorm) + V_re) - scale;
  b_sqrt(&dc);
  if (V_im == 0.0) {
    if (b_ax__tmp_tmp == 0.0) {
      ai = ax__tmp_tmp / t;
      d_V_re_tmp = 0.0;
    } else if (ax__tmp_tmp == 0.0) {
      ai = 0.0;
      d_V_re_tmp = b_ax__tmp_tmp / t;
    } else {
      ai = ax__tmp_tmp / t;
      d_V_re_tmp = b_ax__tmp_tmp / t;
    }
  } else if (t == 0.0) {
    if (ax__tmp_tmp == 0.0) {
      ai = b_ax__tmp_tmp / V_im;
      d_V_re_tmp = 0.0;
    } else if (b_ax__tmp_tmp == 0.0) {
      ai = 0.0;
      d_V_re_tmp = -(ax__tmp_tmp / V_im);
    } else {
      ai = b_ax__tmp_tmp / V_im;
      d_V_re_tmp = -(ax__tmp_tmp / V_im);
    }
  } else {
    im = fabs(t);
    c_V_re_tmp = fabs(V_im);
    if (im > c_V_re_tmp) {
      c_V_re_tmp = V_im / t;
      scale = t + c_V_re_tmp * V_im;
      ai = (ax__tmp_tmp + c_V_re_tmp * b_ax__tmp_tmp) / scale;
      d_V_re_tmp = (b_ax__tmp_tmp - c_V_re_tmp * ax__tmp_tmp) / scale;
    } else if (c_V_re_tmp == im) {
      if (t > 0.0) {
        c_V_re_tmp = 0.5;
      } else {
        c_V_re_tmp = -0.5;
      }
      if (V_im > 0.0) {
        scale = 0.5;
      } else {
        scale = -0.5;
      }
      ai = (ax__tmp_tmp * c_V_re_tmp + b_ax__tmp_tmp * scale) / im;
      d_V_re_tmp = (b_ax__tmp_tmp * c_V_re_tmp - ax__tmp_tmp * scale) / im;
    } else {
      c_V_re_tmp = t / V_im;
      scale = V_im + c_V_re_tmp * t;
      ai = (c_V_re_tmp * ax__tmp_tmp + b_ax__tmp_tmp) / scale;
      d_V_re_tmp = (c_V_re_tmp * b_ax__tmp_tmp - ax__tmp_tmp) / scale;
    }
  }
  scale = M_[4].re * M_[4].re - M_[4].im * M_[4].im;
  V_im = M_[4].re * M_[4].im;
  absxk = V_im + V_im;
  if (absxk == 0.0) {
    if (ax__tmp_im == 0.0) {
      ax__tmp_re = b_ax__tmp_re / scale;
      colnorm = 0.0;
    } else if (b_ax__tmp_re == 0.0) {
      ax__tmp_re = 0.0;
      colnorm = ax__tmp_im / scale;
    } else {
      ax__tmp_re = b_ax__tmp_re / scale;
      colnorm = ax__tmp_im / scale;
    }
  } else if (scale == 0.0) {
    if (b_ax__tmp_re == 0.0) {
      ax__tmp_re = ax__tmp_im / absxk;
      colnorm = 0.0;
    } else if (ax__tmp_im == 0.0) {
      ax__tmp_re = 0.0;
      colnorm = -(b_ax__tmp_re / absxk);
    } else {
      ax__tmp_re = ax__tmp_im / absxk;
      colnorm = -(b_ax__tmp_re / absxk);
    }
  } else {
    im = fabs(scale);
    c_V_re_tmp = fabs(absxk);
    if (im > c_V_re_tmp) {
      c_V_re_tmp = absxk / scale;
      scale += c_V_re_tmp * absxk;
      ax__tmp_re = (b_ax__tmp_re + c_V_re_tmp * ax__tmp_im) / scale;
      colnorm = (ax__tmp_im - c_V_re_tmp * b_ax__tmp_re) / scale;
    } else if (c_V_re_tmp == im) {
      if (scale > 0.0) {
        c_V_re_tmp = 0.5;
      } else {
        c_V_re_tmp = -0.5;
      }
      if (absxk > 0.0) {
        scale = 0.5;
      } else {
        scale = -0.5;
      }
      ax__tmp_re = (b_ax__tmp_re * c_V_re_tmp + ax__tmp_im * scale) / im;
      colnorm = (ax__tmp_im * c_V_re_tmp - b_ax__tmp_re * scale) / im;
    } else {
      c_V_re_tmp = scale / absxk;
      scale = absxk + c_V_re_tmp * scale;
      ax__tmp_re = (c_V_re_tmp * b_ax__tmp_re + ax__tmp_im) / scale;
      colnorm = (c_V_re_tmp * ax__tmp_im - b_ax__tmp_re) / scale;
    }
  }
  t = M_[4].re * M_[8].re - M_[4].im * M_[8].im;
  V_im = M_[4].re * M_[8].im + M_[4].im * M_[8].re;
  if (V_im == 0.0) {
    if (b_ax__tmp_im == 0.0) {
      e_V_re_tmp = c_ax__tmp_re / t;
      V_re = 0.0;
    } else if (c_ax__tmp_re == 0.0) {
      e_V_re_tmp = 0.0;
      V_re = b_ax__tmp_im / t;
    } else {
      e_V_re_tmp = c_ax__tmp_re / t;
      V_re = b_ax__tmp_im / t;
    }
  } else if (t == 0.0) {
    if (c_ax__tmp_re == 0.0) {
      e_V_re_tmp = b_ax__tmp_im / V_im;
      V_re = 0.0;
    } else if (b_ax__tmp_im == 0.0) {
      e_V_re_tmp = 0.0;
      V_re = -(c_ax__tmp_re / V_im);
    } else {
      e_V_re_tmp = b_ax__tmp_im / V_im;
      V_re = -(c_ax__tmp_re / V_im);
    }
  } else {
    im = fabs(t);
    c_V_re_tmp = fabs(V_im);
    if (im > c_V_re_tmp) {
      c_V_re_tmp = V_im / t;
      scale = t + c_V_re_tmp * V_im;
      e_V_re_tmp = (c_ax__tmp_re + c_V_re_tmp * b_ax__tmp_im) / scale;
      V_re = (b_ax__tmp_im - c_V_re_tmp * c_ax__tmp_re) / scale;
    } else if (c_V_re_tmp == im) {
      if (t > 0.0) {
        c_V_re_tmp = 0.5;
      } else {
        c_V_re_tmp = -0.5;
      }
      if (V_im > 0.0) {
        scale = 0.5;
      } else {
        scale = -0.5;
      }
      e_V_re_tmp = (c_ax__tmp_re * c_V_re_tmp + b_ax__tmp_im * scale) / im;
      V_re = (b_ax__tmp_im * c_V_re_tmp - c_ax__tmp_re * scale) / im;
    } else {
      c_V_re_tmp = t / V_im;
      scale = V_im + c_V_re_tmp * t;
      e_V_re_tmp = (c_V_re_tmp * c_ax__tmp_re + b_ax__tmp_im) / scale;
      V_re = (c_V_re_tmp * b_ax__tmp_im - c_ax__tmp_re) / scale;
    }
  }
  if (M_[4].im == 0.0) {
    if (v[9].im == 0.0) {
      absxk = v[9].re / M_[4].re;
      scale = 0.0;
    } else if (v[9].re == 0.0) {
      absxk = 0.0;
      scale = v[9].im / M_[4].re;
    } else {
      absxk = v[9].re / M_[4].re;
      scale = v[9].im / M_[4].re;
    }
  } else if (M_[4].re == 0.0) {
    if (v[9].re == 0.0) {
      absxk = v[9].im / M_[4].im;
      scale = 0.0;
    } else if (v[9].im == 0.0) {
      absxk = 0.0;
      scale = -(v[9].re / M_[4].im);
    } else {
      absxk = v[9].im / M_[4].im;
      scale = -(v[9].re / M_[4].im);
    }
  } else {
    im = fabs(M_[4].re);
    c_V_re_tmp = fabs(M_[4].im);
    if (im > c_V_re_tmp) {
      c_V_re_tmp = M_[4].im / M_[4].re;
      scale = M_[4].re + c_V_re_tmp * M_[4].im;
      absxk = (v[9].re + c_V_re_tmp * v[9].im) / scale;
      scale = (v[9].im - c_V_re_tmp * v[9].re) / scale;
    } else if (c_V_re_tmp == im) {
      if (M_[4].re > 0.0) {
        c_V_re_tmp = 0.5;
      } else {
        c_V_re_tmp = -0.5;
      }
      if (M_[4].im > 0.0) {
        scale = 0.5;
      } else {
        scale = -0.5;
      }
      absxk = (v[9].re * c_V_re_tmp + v[9].im * scale) / im;
      scale = (v[9].im * c_V_re_tmp - v[9].re * scale) / im;
    } else {
      c_V_re_tmp = M_[4].re / M_[4].im;
      scale = M_[4].im + c_V_re_tmp * M_[4].re;
      absxk = (c_V_re_tmp * v[9].re + v[9].im) / scale;
      scale = (c_V_re_tmp * v[9].im - v[9].re) / scale;
    }
  }
  dc1.re = ((ai + ax__tmp_re) + e_V_re_tmp) - absxk;
  dc1.im = ((d_V_re_tmp + colnorm) + V_re) - scale;
  b_sqrt(&dc1);
  if (b_V_re_tmp == 0.0) {
    if (b_ax__tmp_tmp == 0.0) {
      ai = ax__tmp_tmp / V_re_tmp;
      d_V_re_tmp = 0.0;
    } else if (ax__tmp_tmp == 0.0) {
      ai = 0.0;
      d_V_re_tmp = b_ax__tmp_tmp / V_re_tmp;
    } else {
      ai = ax__tmp_tmp / V_re_tmp;
      d_V_re_tmp = b_ax__tmp_tmp / V_re_tmp;
    }
  } else if (V_re_tmp == 0.0) {
    if (ax__tmp_tmp == 0.0) {
      ai = b_ax__tmp_tmp / b_V_re_tmp;
      d_V_re_tmp = 0.0;
    } else if (b_ax__tmp_tmp == 0.0) {
      ai = 0.0;
      d_V_re_tmp = -(ax__tmp_tmp / b_V_re_tmp);
    } else {
      ai = b_ax__tmp_tmp / b_V_re_tmp;
      d_V_re_tmp = -(ax__tmp_tmp / b_V_re_tmp);
    }
  } else {
    im = fabs(V_re_tmp);
    c_V_re_tmp = fabs(b_V_re_tmp);
    if (im > c_V_re_tmp) {
      c_V_re_tmp = b_V_re_tmp / V_re_tmp;
      scale = V_re_tmp + c_V_re_tmp * b_V_re_tmp;
      ai = (ax__tmp_tmp + c_V_re_tmp * b_ax__tmp_tmp) / scale;
      d_V_re_tmp = (b_ax__tmp_tmp - c_V_re_tmp * ax__tmp_tmp) / scale;
    } else if (c_V_re_tmp == im) {
      if (V_re_tmp > 0.0) {
        c_V_re_tmp = 0.5;
      } else {
        c_V_re_tmp = -0.5;
      }
      if (b_V_re_tmp > 0.0) {
        scale = 0.5;
      } else {
        scale = -0.5;
      }
      ai = (ax__tmp_tmp * c_V_re_tmp + b_ax__tmp_tmp * scale) / im;
      d_V_re_tmp = (b_ax__tmp_tmp * c_V_re_tmp - ax__tmp_tmp * scale) / im;
    } else {
      c_V_re_tmp = V_re_tmp / b_V_re_tmp;
      scale = b_V_re_tmp + c_V_re_tmp * V_re_tmp;
      ai = (c_V_re_tmp * ax__tmp_tmp + b_ax__tmp_tmp) / scale;
      d_V_re_tmp = (c_V_re_tmp * b_ax__tmp_tmp - ax__tmp_tmp) / scale;
    }
  }
  if (V_im == 0.0) {
    if (ax__tmp_im == 0.0) {
      ax__tmp_re = b_ax__tmp_re / t;
      ax__tmp_im = 0.0;
    } else if (b_ax__tmp_re == 0.0) {
      ax__tmp_re = 0.0;
      ax__tmp_im /= t;
    } else {
      ax__tmp_re = b_ax__tmp_re / t;
      ax__tmp_im /= t;
    }
  } else if (t == 0.0) {
    if (b_ax__tmp_re == 0.0) {
      ax__tmp_re = ax__tmp_im / V_im;
      ax__tmp_im = 0.0;
    } else if (ax__tmp_im == 0.0) {
      ax__tmp_re = 0.0;
      ax__tmp_im = -(b_ax__tmp_re / V_im);
    } else {
      ax__tmp_re = ax__tmp_im / V_im;
      ax__tmp_im = -(b_ax__tmp_re / V_im);
    }
  } else {
    im = fabs(t);
    c_V_re_tmp = fabs(V_im);
    if (im > c_V_re_tmp) {
      c_V_re_tmp = V_im / t;
      scale = t + c_V_re_tmp * V_im;
      ax__tmp_re = (b_ax__tmp_re + c_V_re_tmp * ax__tmp_im) / scale;
      ax__tmp_im = (ax__tmp_im - c_V_re_tmp * b_ax__tmp_re) / scale;
    } else if (c_V_re_tmp == im) {
      if (t > 0.0) {
        c_V_re_tmp = 0.5;
      } else {
        c_V_re_tmp = -0.5;
      }
      if (V_im > 0.0) {
        scale = 0.5;
      } else {
        scale = -0.5;
      }
      ax__tmp_re = (b_ax__tmp_re * c_V_re_tmp + ax__tmp_im * scale) / im;
      ax__tmp_im = (ax__tmp_im * c_V_re_tmp - b_ax__tmp_re * scale) / im;
    } else {
      c_V_re_tmp = t / V_im;
      scale = V_im + c_V_re_tmp * t;
      ax__tmp_re = (c_V_re_tmp * b_ax__tmp_re + ax__tmp_im) / scale;
      ax__tmp_im = (c_V_re_tmp * ax__tmp_im - b_ax__tmp_re) / scale;
    }
  }
  scale = M_[8].re * M_[8].re - M_[8].im * M_[8].im;
  V_im = M_[8].re * M_[8].im;
  absxk = V_im + V_im;
  if (absxk == 0.0) {
    if (b_ax__tmp_im == 0.0) {
      b_ax__tmp_re = c_ax__tmp_re / scale;
      b_ax__tmp_im = 0.0;
    } else if (c_ax__tmp_re == 0.0) {
      b_ax__tmp_re = 0.0;
      b_ax__tmp_im /= scale;
    } else {
      b_ax__tmp_re = c_ax__tmp_re / scale;
      b_ax__tmp_im /= scale;
    }
  } else if (scale == 0.0) {
    if (c_ax__tmp_re == 0.0) {
      b_ax__tmp_re = b_ax__tmp_im / absxk;
      b_ax__tmp_im = 0.0;
    } else if (b_ax__tmp_im == 0.0) {
      b_ax__tmp_re = 0.0;
      b_ax__tmp_im = -(c_ax__tmp_re / absxk);
    } else {
      b_ax__tmp_re = b_ax__tmp_im / absxk;
      b_ax__tmp_im = -(c_ax__tmp_re / absxk);
    }
  } else {
    im = fabs(scale);
    c_V_re_tmp = fabs(absxk);
    if (im > c_V_re_tmp) {
      c_V_re_tmp = absxk / scale;
      scale += c_V_re_tmp * absxk;
      b_ax__tmp_re = (c_ax__tmp_re + c_V_re_tmp * b_ax__tmp_im) / scale;
      b_ax__tmp_im = (b_ax__tmp_im - c_V_re_tmp * c_ax__tmp_re) / scale;
    } else if (c_V_re_tmp == im) {
      if (scale > 0.0) {
        c_V_re_tmp = 0.5;
      } else {
        c_V_re_tmp = -0.5;
      }
      if (absxk > 0.0) {
        scale = 0.5;
      } else {
        scale = -0.5;
      }
      b_ax__tmp_re = (c_ax__tmp_re * c_V_re_tmp + b_ax__tmp_im * scale) / im;
      b_ax__tmp_im = (b_ax__tmp_im * c_V_re_tmp - c_ax__tmp_re * scale) / im;
    } else {
      c_V_re_tmp = scale / absxk;
      scale = absxk + c_V_re_tmp * scale;
      b_ax__tmp_re = (c_V_re_tmp * c_ax__tmp_re + b_ax__tmp_im) / scale;
      b_ax__tmp_im = (c_V_re_tmp * b_ax__tmp_im - c_ax__tmp_re) / scale;
    }
  }
  if (M_[8].im == 0.0) {
    if (v[9].im == 0.0) {
      absxk = v[9].re / M_[8].re;
      scale = 0.0;
    } else if (v[9].re == 0.0) {
      absxk = 0.0;
      scale = v[9].im / M_[8].re;
    } else {
      absxk = v[9].re / M_[8].re;
      scale = v[9].im / M_[8].re;
    }
  } else if (M_[8].re == 0.0) {
    if (v[9].re == 0.0) {
      absxk = v[9].im / M_[8].im;
      scale = 0.0;
    } else if (v[9].im == 0.0) {
      absxk = 0.0;
      scale = -(v[9].re / M_[8].im);
    } else {
      absxk = v[9].im / M_[8].im;
      scale = -(v[9].re / M_[8].im);
    }
  } else {
    im = fabs(M_[8].re);
    c_V_re_tmp = fabs(M_[8].im);
    if (im > c_V_re_tmp) {
      c_V_re_tmp = M_[8].im / M_[8].re;
      scale = M_[8].re + c_V_re_tmp * M_[8].im;
      absxk = (v[9].re + c_V_re_tmp * v[9].im) / scale;
      scale = (v[9].im - c_V_re_tmp * v[9].re) / scale;
    } else if (c_V_re_tmp == im) {
      if (M_[8].re > 0.0) {
        c_V_re_tmp = 0.5;
      } else {
        c_V_re_tmp = -0.5;
      }
      if (M_[8].im > 0.0) {
        scale = 0.5;
      } else {
        scale = -0.5;
      }
      absxk = (v[9].re * c_V_re_tmp + v[9].im * scale) / im;
      scale = (v[9].im * c_V_re_tmp - v[9].re * scale) / im;
    } else {
      c_V_re_tmp = M_[8].re / M_[8].im;
      scale = M_[8].im + c_V_re_tmp * M_[8].re;
      absxk = (c_V_re_tmp * v[9].re + v[9].im) / scale;
      scale = (c_V_re_tmp * v[9].im - v[9].re) / scale;
    }
  }
  ax__tmp.re = ((ai + ax__tmp_re) + b_ax__tmp_re) - absxk;
  ax__tmp.im = ((d_V_re_tmp + ax__tmp_im) + b_ax__tmp_im) - scale;
  b_sqrt(&ax__tmp);
  if (dc.im == 0.0) {
    gain[0].re = 1.0 / dc.re;
    gain[0].im = 0.0;
  } else if (dc.re == 0.0) {
    gain[0].re = 0.0;
    gain[0].im = -(1.0 / dc.im);
  } else {
    im = fabs(dc.re);
    c_V_re_tmp = fabs(dc.im);
    if (im > c_V_re_tmp) {
      c_V_re_tmp = dc.im / dc.re;
      scale = dc.re + c_V_re_tmp * dc.im;
      gain[0].re = (c_V_re_tmp * 0.0 + 1.0) / scale;
      gain[0].im = (0.0 - c_V_re_tmp) / scale;
    } else if (c_V_re_tmp == im) {
      if (dc.re > 0.0) {
        c_V_re_tmp = 0.5;
      } else {
        c_V_re_tmp = -0.5;
      }
      if (dc.im > 0.0) {
        scale = 0.5;
      } else {
        scale = -0.5;
      }
      gain[0].re = (c_V_re_tmp + 0.0 * scale) / im;
      gain[0].im = (0.0 * c_V_re_tmp - scale) / im;
    } else {
      c_V_re_tmp = dc.re / dc.im;
      scale = dc.im + c_V_re_tmp * dc.re;
      gain[0].re = c_V_re_tmp / scale;
      gain[0].im = (c_V_re_tmp * 0.0 - 1.0) / scale;
    }
  }
  gain[3].re = 0.0;
  gain[3].im = 0.0;
  gain[6].re = 0.0;
  gain[6].im = 0.0;
  gain[1].re = 0.0;
  gain[1].im = 0.0;
  if (dc1.im == 0.0) {
    gain[4].re = 1.0 / dc1.re;
    gain[4].im = 0.0;
  } else if (dc1.re == 0.0) {
    gain[4].re = 0.0;
    gain[4].im = -(1.0 / dc1.im);
  } else {
    im = fabs(dc1.re);
    c_V_re_tmp = fabs(dc1.im);
    if (im > c_V_re_tmp) {
      c_V_re_tmp = dc1.im / dc1.re;
      scale = dc1.re + c_V_re_tmp * dc1.im;
      gain[4].re = (c_V_re_tmp * 0.0 + 1.0) / scale;
      gain[4].im = (0.0 - c_V_re_tmp) / scale;
    } else if (c_V_re_tmp == im) {
      if (dc1.re > 0.0) {
        c_V_re_tmp = 0.5;
      } else {
        c_V_re_tmp = -0.5;
      }
      if (dc1.im > 0.0) {
        scale = 0.5;
      } else {
        scale = -0.5;
      }
      gain[4].re = (c_V_re_tmp + 0.0 * scale) / im;
      gain[4].im = (0.0 * c_V_re_tmp - scale) / im;
    } else {
      c_V_re_tmp = dc1.re / dc1.im;
      scale = dc1.im + c_V_re_tmp * dc1.re;
      gain[4].re = c_V_re_tmp / scale;
      gain[4].im = (c_V_re_tmp * 0.0 - 1.0) / scale;
    }
  }
  gain[7].re = 0.0;
  gain[7].im = 0.0;
  gain[2].re = 0.0;
  gain[2].im = 0.0;
  gain[5].re = 0.0;
  gain[5].im = 0.0;
  if (ax__tmp.im == 0.0) {
    gain[8].re = 1.0 / ax__tmp.re;
    gain[8].im = 0.0;
  } else if (ax__tmp.re == 0.0) {
    gain[8].re = 0.0;
    gain[8].im = -(1.0 / ax__tmp.im);
  } else {
    im = fabs(ax__tmp.re);
    c_V_re_tmp = fabs(ax__tmp.im);
    if (im > c_V_re_tmp) {
      c_V_re_tmp = ax__tmp.im / ax__tmp.re;
      scale = ax__tmp.re + c_V_re_tmp * ax__tmp.im;
      gain[8].re = (c_V_re_tmp * 0.0 + 1.0) / scale;
      gain[8].im = (0.0 - c_V_re_tmp) / scale;
    } else if (c_V_re_tmp == im) {
      if (ax__tmp.re > 0.0) {
        c_V_re_tmp = 0.5;
      } else {
        c_V_re_tmp = -0.5;
      }
      if (ax__tmp.im > 0.0) {
        scale = 0.5;
      } else {
        scale = -0.5;
      }
      gain[8].re = (c_V_re_tmp + 0.0 * scale) / im;
      gain[8].im = (0.0 * c_V_re_tmp - scale) / im;
    } else {
      c_V_re_tmp = ax__tmp.re / ax__tmp.im;
      scale = ax__tmp.im + c_V_re_tmp * ax__tmp.re;
      gain[8].re = c_V_re_tmp / scale;
      gain[8].im = (c_V_re_tmp * 0.0 - 1.0) / scale;
    }
  }
  for (rtemp = 0; rtemp < 3; rtemp++) {
    scale = gain[rtemp].re;
    absxk = gain[rtemp].im;
    t = gain[rtemp + 3].re;
    colnorm = gain[rtemp + 3].im;
    ai = gain[rtemp + 6].re;
    e_V_re_tmp = gain[rtemp + 6].im;
    for (i = 0; i < 3; i++) {
      V_re = V[i].re;
      V_im = -V[i].im;
      re = scale * V_re - absxk * V_im;
      im = scale * V_im + absxk * V_re;
      V_re = V[i + 3].re;
      V_im = -V[i + 3].im;
      re += t * V_re - colnorm * V_im;
      im += t * V_im + colnorm * V_re;
      V_re = V[i + 6].re;
      V_im = -V[i + 6].im;
      re += ai * V_re - e_V_re_tmp * V_im;
      im += ai * V_im + e_V_re_tmp * V_re;
      r1 = rtemp + 3 * i;
      matrix[r1].re = re;
      matrix[r1].im = im;
    }
  }
}

/*
 * File trailer for ellipsoid_fit_test.c
 *
 * [EOF]
 */
