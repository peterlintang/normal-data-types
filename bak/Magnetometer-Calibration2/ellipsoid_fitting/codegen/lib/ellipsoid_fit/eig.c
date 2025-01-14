/*
 * File: eig.c
 *
 * MATLAB Coder version            : 5.3
 * C/C++ source code generated on  : 02-Aug-2024 17:35:13
 */

/* Include Files */
#include "eig.h"
#include "ellipsoid_fit_data.h"
#include "rt_nonfinite.h"
#include "schur.h"
#include "xzggev.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : const double A[36]
 *                creal_T V[36]
 *                creal_T D[36]
 * Return Type  : void
 */
void eig(const double A[36], creal_T V[36], creal_T D[36])
{
  creal_T At[36];
  creal_T alpha1[6];
  creal_T beta1[6];
  double T[36];
  double U[36];
  double absxk;
  double ai;
  double ar;
  double brm;
  double colnorm;
  double scale;
  double t;
  int exitg1;
  int i;
  int i1;
  int j;
  int k;
  int kend_tmp;
  int sgn;
  bool exitg2;
  bool p;
  p = true;
  for (k = 0; k < 36; k++) {
    if ((!p) || (rtIsInf(A[k]) || rtIsNaN(A[k]))) {
      p = false;
    }
  }
  if (!p) {
    for (i = 0; i < 36; i++) {
      V[i].re = rtNaN;
      V[i].im = 0.0;
      D[i].re = 0.0;
      D[i].im = 0.0;
    }
    for (k = 0; k < 6; k++) {
      i = k + 6 * k;
      D[i].re = rtNaN;
      D[i].im = 0.0;
    }
  } else {
    p = true;
    j = 0;
    exitg2 = false;
    while ((!exitg2) && (j < 6)) {
      k = 0;
      do {
        exitg1 = 0;
        if (k <= j) {
          if (!(A[k + 6 * j] == A[j + 6 * k])) {
            p = false;
            exitg1 = 1;
          } else {
            k++;
          }
        } else {
          j++;
          exitg1 = 2;
        }
      } while (exitg1 == 0);
      if (exitg1 == 1) {
        exitg2 = true;
      }
    }
    if (p) {
      memcpy(&T[0], &A[0], 36U * sizeof(double));
      schur(T, U);
      for (j = 0; j < 5; j++) {
        T[(j + 6 * j) + 1] = 0.0;
        for (k = 0; k <= j; k++) {
          T[k + 6 * (j + 1)] = 0.0;
        }
      }
      for (i = 0; i < 36; i++) {
        V[i].re = U[i];
        V[i].im = 0.0;
        D[i].re = T[i];
        D[i].im = 0.0;
      }
    } else {
      p = true;
      j = 0;
      exitg2 = false;
      while ((!exitg2) && (j < 6)) {
        k = 0;
        do {
          exitg1 = 0;
          if (k <= j) {
            if (!(A[k + 6 * j] == -A[j + 6 * k])) {
              p = false;
              exitg1 = 1;
            } else {
              k++;
            }
          } else {
            j++;
            exitg1 = 2;
          }
        } while (exitg1 == 0);
        if (exitg1 == 1) {
          exitg2 = true;
        }
      }
      if (p) {
        memcpy(&T[0], &A[0], 36U * sizeof(double));
        schur(T, U);
        memset(&D[0], 0, 36U * sizeof(creal_T));
        k = 1;
        do {
          exitg1 = 0;
          if (k <= 6) {
            if (k != 6) {
              i = k + 6 * (k - 1);
              scale = T[i];
              if (scale != 0.0) {
                scale = fabs(scale);
                D[i - 1].re = 0.0;
                D[i - 1].im = scale;
                i = k + 6 * k;
                D[i].re = 0.0;
                D[i].im = -scale;
                k += 2;
              } else {
                k++;
              }
            } else {
              k++;
            }
          } else {
            exitg1 = 1;
          }
        } while (exitg1 == 0);
        for (i = 0; i < 36; i++) {
          V[i].re = U[i];
          V[i].im = 0.0;
        }
        j = 1;
        do {
          exitg1 = 0;
          if (j <= 6) {
            if (j != 6) {
              i = 6 * (j - 1);
              if (T[j + i] != 0.0) {
                if (T[j + 6 * (j - 1)] < 0.0) {
                  sgn = 1;
                } else {
                  sgn = -1;
                }
                for (k = 0; k < 6; k++) {
                  kend_tmp = k + i;
                  scale = V[kend_tmp].re;
                  i1 = k + 6 * j;
                  ai = (double)sgn * V[i1].re;
                  if (ai == 0.0) {
                    V[kend_tmp].re = scale / 1.4142135623730951;
                    V[kend_tmp].im = 0.0;
                  } else if (scale == 0.0) {
                    V[kend_tmp].re = 0.0;
                    V[kend_tmp].im = ai / 1.4142135623730951;
                  } else {
                    V[kend_tmp].re = scale / 1.4142135623730951;
                    V[kend_tmp].im = ai / 1.4142135623730951;
                  }
                  V[i1].re = V[kend_tmp].re;
                  V[i1].im = -V[kend_tmp].im;
                }
                j += 2;
              } else {
                j++;
              }
            } else {
              j++;
            }
          } else {
            exitg1 = 1;
          }
        } while (exitg1 == 0);
      } else {
        for (i = 0; i < 36; i++) {
          At[i].re = A[i];
          At[i].im = 0.0;
        }
        xzggev(At, &k, alpha1, beta1, V);
        for (sgn = 0; sgn <= 30; sgn += 6) {
          colnorm = 0.0;
          scale = 3.3121686421112381E-170;
          kend_tmp = sgn + 6;
          for (k = sgn + 1; k <= kend_tmp; k++) {
            absxk = fabs(V[k - 1].re);
            if (absxk > scale) {
              t = scale / absxk;
              colnorm = colnorm * t * t + 1.0;
              scale = absxk;
            } else {
              t = absxk / scale;
              colnorm += t * t;
            }
            absxk = fabs(V[k - 1].im);
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
          for (j = sgn + 1; j <= kend_tmp; j++) {
            ar = V[j - 1].re;
            ai = V[j - 1].im;
            if (ai == 0.0) {
              absxk = ar / colnorm;
              scale = 0.0;
            } else if (ar == 0.0) {
              absxk = 0.0;
              scale = ai / colnorm;
            } else {
              absxk = ar / colnorm;
              scale = ai / colnorm;
            }
            V[j - 1].re = absxk;
            V[j - 1].im = scale;
          }
        }
        memset(&D[0], 0, 36U * sizeof(creal_T));
        for (k = 0; k < 6; k++) {
          ar = alpha1[k].re;
          ai = alpha1[k].im;
          t = beta1[k].re;
          colnorm = beta1[k].im;
          if (colnorm == 0.0) {
            if (ai == 0.0) {
              i = k + 6 * k;
              D[i].re = ar / t;
              D[i].im = 0.0;
            } else if (ar == 0.0) {
              i = k + 6 * k;
              D[i].re = 0.0;
              D[i].im = ai / t;
            } else {
              i = k + 6 * k;
              D[i].re = ar / t;
              D[i].im = ai / t;
            }
          } else if (t == 0.0) {
            if (ar == 0.0) {
              i = k + 6 * k;
              D[i].re = ai / colnorm;
              D[i].im = 0.0;
            } else if (ai == 0.0) {
              i = k + 6 * k;
              D[i].re = 0.0;
              D[i].im = -(ar / colnorm);
            } else {
              i = k + 6 * k;
              D[i].re = ai / colnorm;
              D[i].im = -(ar / colnorm);
            }
          } else {
            brm = fabs(t);
            scale = fabs(colnorm);
            if (brm > scale) {
              absxk = colnorm / t;
              scale = t + absxk * colnorm;
              i = k + 6 * k;
              D[i].re = (ar + absxk * ai) / scale;
              D[i].im = (ai - absxk * ar) / scale;
            } else if (scale == brm) {
              if (t > 0.0) {
                absxk = 0.5;
              } else {
                absxk = -0.5;
              }
              if (colnorm > 0.0) {
                scale = 0.5;
              } else {
                scale = -0.5;
              }
              i = k + 6 * k;
              D[i].re = (ar * absxk + ai * scale) / brm;
              D[i].im = (ai * absxk - ar * scale) / brm;
            } else {
              absxk = t / colnorm;
              scale = colnorm + absxk * t;
              i = k + 6 * k;
              D[i].re = (absxk * ar + ai) / scale;
              D[i].im = (absxk * ai - ar) / scale;
            }
          }
        }
      }
    }
  }
}

/*
 * File trailer for eig.c
 *
 * [EOF]
 */
