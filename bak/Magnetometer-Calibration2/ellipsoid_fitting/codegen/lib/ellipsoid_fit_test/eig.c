/*
 * File: eig.c
 *
 * MATLAB Coder version            : 5.3
 * C/C++ source code generated on  : 13-Aug-2024 17:16:40
 */

/* Include Files */
#include "eig.h"
#include "ellipsoid_fit_test_data.h"
#include "rt_nonfinite.h"
#include "schur.h"
#include "xzggev.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : const double A[9]
 *                creal_T V[9]
 *                creal_T D[9]
 * Return Type  : void
 */
void b_eig(const double A[9], creal_T V[9], creal_T D[9])
{
  creal_T At[9];
  creal_T alpha1[3];
  creal_T beta1[3];
  double T[9];
  double U[9];
  double absxk;
  double colnorm;
  double scale;
  double t;
  int coltop;
  int exitg1;
  int j;
  int kend_tmp;
  int sgn;
  bool exitg2;
  bool p;
  p = true;
  for (sgn = 0; sgn < 9; sgn++) {
    if ((!p) || (rtIsInf(A[sgn]) || rtIsNaN(A[sgn]))) {
      p = false;
    }
  }
  if (!p) {
    for (kend_tmp = 0; kend_tmp < 9; kend_tmp++) {
      V[kend_tmp].re = rtNaN;
      V[kend_tmp].im = 0.0;
      D[kend_tmp].re = 0.0;
      D[kend_tmp].im = 0.0;
    }
    D[0].re = rtNaN;
    D[0].im = 0.0;
    D[4].re = rtNaN;
    D[4].im = 0.0;
    D[8].re = rtNaN;
    D[8].im = 0.0;
  } else {
    p = true;
    j = 0;
    exitg2 = false;
    while ((!exitg2) && (j < 3)) {
      sgn = 0;
      do {
        exitg1 = 0;
        if (sgn <= j) {
          if (!(A[sgn + 3 * j] == A[j + 3 * sgn])) {
            p = false;
            exitg1 = 1;
          } else {
            sgn++;
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
      schur(A, U, T);
      for (j = 0; j < 2; j++) {
        T[(j + 3 * j) + 1] = 0.0;
        for (sgn = 0; sgn <= j; sgn++) {
          T[sgn + 3 * (j + 1)] = 0.0;
        }
      }
      for (kend_tmp = 0; kend_tmp < 9; kend_tmp++) {
        V[kend_tmp].re = U[kend_tmp];
        V[kend_tmp].im = 0.0;
        D[kend_tmp].re = T[kend_tmp];
        D[kend_tmp].im = 0.0;
      }
    } else {
      p = true;
      j = 0;
      exitg2 = false;
      while ((!exitg2) && (j < 3)) {
        sgn = 0;
        do {
          exitg1 = 0;
          if (sgn <= j) {
            if (!(A[sgn + 3 * j] == -A[j + 3 * sgn])) {
              p = false;
              exitg1 = 1;
            } else {
              sgn++;
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
        schur(A, U, T);
        memset(&D[0], 0, 9U * sizeof(creal_T));
        sgn = 1;
        do {
          exitg1 = 0;
          if (sgn <= 3) {
            if (sgn != 3) {
              kend_tmp = sgn + 3 * (sgn - 1);
              scale = T[kend_tmp];
              if (scale != 0.0) {
                scale = fabs(scale);
                D[kend_tmp - 1].re = 0.0;
                D[kend_tmp - 1].im = scale;
                kend_tmp = sgn + 3 * sgn;
                D[kend_tmp].re = 0.0;
                D[kend_tmp].im = -scale;
                sgn += 2;
              } else {
                sgn++;
              }
            } else {
              sgn++;
            }
          } else {
            exitg1 = 1;
          }
        } while (exitg1 == 0);
        for (kend_tmp = 0; kend_tmp < 9; kend_tmp++) {
          V[kend_tmp].re = U[kend_tmp];
          V[kend_tmp].im = 0.0;
        }
        j = 1;
        do {
          exitg1 = 0;
          if (j <= 3) {
            if (j != 3) {
              kend_tmp = 3 * (j - 1);
              if (T[j + kend_tmp] != 0.0) {
                if (T[j + 3 * (j - 1)] < 0.0) {
                  sgn = 1;
                } else {
                  sgn = -1;
                }
                scale = V[kend_tmp].re;
                t = (double)sgn * V[3 * j].re;
                if (t == 0.0) {
                  V[kend_tmp].re = scale / 1.4142135623730951;
                  V[kend_tmp].im = 0.0;
                } else if (scale == 0.0) {
                  V[kend_tmp].re = 0.0;
                  V[kend_tmp].im = t / 1.4142135623730951;
                } else {
                  V[kend_tmp].re = scale / 1.4142135623730951;
                  V[kend_tmp].im = t / 1.4142135623730951;
                }
                V[3 * j].re = V[kend_tmp].re;
                V[3 * j].im = -V[kend_tmp].im;
                scale = V[kend_tmp + 1].re;
                coltop = 3 * j + 1;
                t = (double)sgn * V[coltop].re;
                if (t == 0.0) {
                  V[kend_tmp + 1].re = scale / 1.4142135623730951;
                  V[kend_tmp + 1].im = 0.0;
                } else if (scale == 0.0) {
                  V[kend_tmp + 1].re = 0.0;
                  V[kend_tmp + 1].im = t / 1.4142135623730951;
                } else {
                  V[kend_tmp + 1].re = scale / 1.4142135623730951;
                  V[kend_tmp + 1].im = t / 1.4142135623730951;
                }
                V[coltop].re = V[kend_tmp + 1].re;
                V[coltop].im = -V[kend_tmp + 1].im;
                scale = V[kend_tmp + 2].re;
                coltop = 3 * j + 2;
                t = (double)sgn * V[coltop].re;
                if (t == 0.0) {
                  V[kend_tmp + 2].re = scale / 1.4142135623730951;
                  V[kend_tmp + 2].im = 0.0;
                } else if (scale == 0.0) {
                  V[kend_tmp + 2].re = 0.0;
                  V[kend_tmp + 2].im = t / 1.4142135623730951;
                } else {
                  V[kend_tmp + 2].re = scale / 1.4142135623730951;
                  V[kend_tmp + 2].im = t / 1.4142135623730951;
                }
                V[coltop].re = V[kend_tmp + 2].re;
                V[coltop].im = -V[kend_tmp + 2].im;
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
        for (kend_tmp = 0; kend_tmp < 9; kend_tmp++) {
          At[kend_tmp].re = A[kend_tmp];
          At[kend_tmp].im = 0.0;
        }
        b_xzggev(At, &sgn, alpha1, beta1, V);
        for (coltop = 0; coltop <= 6; coltop += 3) {
          colnorm = 0.0;
          scale = 3.3121686421112381E-170;
          kend_tmp = coltop + 3;
          for (sgn = coltop + 1; sgn <= kend_tmp; sgn++) {
            absxk = fabs(V[sgn - 1].re);
            if (absxk > scale) {
              t = scale / absxk;
              colnorm = colnorm * t * t + 1.0;
              scale = absxk;
            } else {
              t = absxk / scale;
              colnorm += t * t;
            }
            absxk = fabs(V[sgn - 1].im);
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
          for (j = coltop + 1; j <= kend_tmp; j++) {
            scale = V[j - 1].re;
            t = V[j - 1].im;
            if (t == 0.0) {
              absxk = scale / colnorm;
              scale = 0.0;
            } else if (scale == 0.0) {
              absxk = 0.0;
              scale = t / colnorm;
            } else {
              absxk = scale / colnorm;
              scale = t / colnorm;
            }
            V[j - 1].re = absxk;
            V[j - 1].im = scale;
          }
        }
        memset(&D[0], 0, 9U * sizeof(creal_T));
        if (beta1[0].im == 0.0) {
          if (alpha1[0].im == 0.0) {
            D[0].re = alpha1[0].re / beta1[0].re;
            D[0].im = 0.0;
          } else if (alpha1[0].re == 0.0) {
            D[0].re = 0.0;
            D[0].im = alpha1[0].im / beta1[0].re;
          } else {
            D[0].re = alpha1[0].re / beta1[0].re;
            D[0].im = alpha1[0].im / beta1[0].re;
          }
        } else if (beta1[0].re == 0.0) {
          if (alpha1[0].re == 0.0) {
            D[0].re = alpha1[0].im / beta1[0].im;
            D[0].im = 0.0;
          } else if (alpha1[0].im == 0.0) {
            D[0].re = 0.0;
            D[0].im = -(alpha1[0].re / beta1[0].im);
          } else {
            D[0].re = alpha1[0].im / beta1[0].im;
            D[0].im = -(alpha1[0].re / beta1[0].im);
          }
        } else {
          t = fabs(beta1[0].re);
          scale = fabs(beta1[0].im);
          if (t > scale) {
            scale = beta1[0].im / beta1[0].re;
            absxk = beta1[0].re + scale * beta1[0].im;
            D[0].re = (alpha1[0].re + scale * alpha1[0].im) / absxk;
            D[0].im = (alpha1[0].im - scale * alpha1[0].re) / absxk;
          } else if (scale == t) {
            if (beta1[0].re > 0.0) {
              scale = 0.5;
            } else {
              scale = -0.5;
            }
            if (beta1[0].im > 0.0) {
              absxk = 0.5;
            } else {
              absxk = -0.5;
            }
            D[0].re = (alpha1[0].re * scale + alpha1[0].im * absxk) / t;
            D[0].im = (alpha1[0].im * scale - alpha1[0].re * absxk) / t;
          } else {
            scale = beta1[0].re / beta1[0].im;
            absxk = beta1[0].im + scale * beta1[0].re;
            D[0].re = (scale * alpha1[0].re + alpha1[0].im) / absxk;
            D[0].im = (scale * alpha1[0].im - alpha1[0].re) / absxk;
          }
        }
        if (beta1[1].im == 0.0) {
          if (alpha1[1].im == 0.0) {
            D[4].re = alpha1[1].re / beta1[1].re;
            D[4].im = 0.0;
          } else if (alpha1[1].re == 0.0) {
            D[4].re = 0.0;
            D[4].im = alpha1[1].im / beta1[1].re;
          } else {
            D[4].re = alpha1[1].re / beta1[1].re;
            D[4].im = alpha1[1].im / beta1[1].re;
          }
        } else if (beta1[1].re == 0.0) {
          if (alpha1[1].re == 0.0) {
            D[4].re = alpha1[1].im / beta1[1].im;
            D[4].im = 0.0;
          } else if (alpha1[1].im == 0.0) {
            D[4].re = 0.0;
            D[4].im = -(alpha1[1].re / beta1[1].im);
          } else {
            D[4].re = alpha1[1].im / beta1[1].im;
            D[4].im = -(alpha1[1].re / beta1[1].im);
          }
        } else {
          t = fabs(beta1[1].re);
          scale = fabs(beta1[1].im);
          if (t > scale) {
            scale = beta1[1].im / beta1[1].re;
            absxk = beta1[1].re + scale * beta1[1].im;
            D[4].re = (alpha1[1].re + scale * alpha1[1].im) / absxk;
            D[4].im = (alpha1[1].im - scale * alpha1[1].re) / absxk;
          } else if (scale == t) {
            if (beta1[1].re > 0.0) {
              scale = 0.5;
            } else {
              scale = -0.5;
            }
            if (beta1[1].im > 0.0) {
              absxk = 0.5;
            } else {
              absxk = -0.5;
            }
            D[4].re = (alpha1[1].re * scale + alpha1[1].im * absxk) / t;
            D[4].im = (alpha1[1].im * scale - alpha1[1].re * absxk) / t;
          } else {
            scale = beta1[1].re / beta1[1].im;
            absxk = beta1[1].im + scale * beta1[1].re;
            D[4].re = (scale * alpha1[1].re + alpha1[1].im) / absxk;
            D[4].im = (scale * alpha1[1].im - alpha1[1].re) / absxk;
          }
        }
        if (beta1[2].im == 0.0) {
          if (alpha1[2].im == 0.0) {
            D[8].re = alpha1[2].re / beta1[2].re;
            D[8].im = 0.0;
          } else if (alpha1[2].re == 0.0) {
            D[8].re = 0.0;
            D[8].im = alpha1[2].im / beta1[2].re;
          } else {
            D[8].re = alpha1[2].re / beta1[2].re;
            D[8].im = alpha1[2].im / beta1[2].re;
          }
        } else if (beta1[2].re == 0.0) {
          if (alpha1[2].re == 0.0) {
            D[8].re = alpha1[2].im / beta1[2].im;
            D[8].im = 0.0;
          } else if (alpha1[2].im == 0.0) {
            D[8].re = 0.0;
            D[8].im = -(alpha1[2].re / beta1[2].im);
          } else {
            D[8].re = alpha1[2].im / beta1[2].im;
            D[8].im = -(alpha1[2].re / beta1[2].im);
          }
        } else {
          t = fabs(beta1[2].re);
          scale = fabs(beta1[2].im);
          if (t > scale) {
            scale = beta1[2].im / beta1[2].re;
            absxk = beta1[2].re + scale * beta1[2].im;
            D[8].re = (alpha1[2].re + scale * alpha1[2].im) / absxk;
            D[8].im = (alpha1[2].im - scale * alpha1[2].re) / absxk;
          } else if (scale == t) {
            if (beta1[2].re > 0.0) {
              scale = 0.5;
            } else {
              scale = -0.5;
            }
            if (beta1[2].im > 0.0) {
              absxk = 0.5;
            } else {
              absxk = -0.5;
            }
            D[8].re = (alpha1[2].re * scale + alpha1[2].im * absxk) / t;
            D[8].im = (alpha1[2].im * scale - alpha1[2].re * absxk) / t;
          } else {
            scale = beta1[2].re / beta1[2].im;
            absxk = beta1[2].im + scale * beta1[2].re;
            D[8].re = (scale * alpha1[2].re + alpha1[2].im) / absxk;
            D[8].im = (scale * alpha1[2].im - alpha1[2].re) / absxk;
          }
        }
      }
    }
  }
}

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
      b_schur(T, U);
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
        b_schur(T, U);
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
