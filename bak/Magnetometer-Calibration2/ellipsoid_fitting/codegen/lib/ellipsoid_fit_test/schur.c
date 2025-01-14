/*
 * File: schur.c
 *
 * MATLAB Coder version            : 5.3
 * C/C++ source code generated on  : 13-Aug-2024 17:16:40
 */

/* Include Files */
#include "schur.h"
#include "ellipsoid_fit_test_rtwutil.h"
#include "rt_nonfinite.h"
#include "xdhseqr.h"
#include "xgerc.h"
#include "xhseqr.h"
#include "xnrm2.h"
#include "xzlarf.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : double A[36]
 *                double V[36]
 * Return Type  : void
 */
void b_schur(double A[36], double V[36])
{
  double work[6];
  double tau[5];
  double alpha1;
  double beta1;
  double beta1_tmp;
  double xnorm;
  int alpha1_tmp;
  int b_i;
  int exitg1;
  int i;
  int i1;
  int ia;
  int ic0;
  int im1n_tmp;
  int in;
  int iv0;
  int ix0;
  int k;
  int knt;
  int lastc;
  int lastv;
  bool exitg2;
  bool p;
  p = true;
  for (k = 0; k < 36; k++) {
    if (p) {
      xnorm = A[k];
      if (rtIsInf(xnorm) || rtIsNaN(xnorm)) {
        p = false;
      }
    } else {
      p = false;
    }
  }
  if (!p) {
    for (b_i = 0; b_i < 36; b_i++) {
      V[b_i] = rtNaN;
    }
    knt = 2;
    for (k = 0; k < 5; k++) {
      if (knt <= 6) {
        memset(&V[(k * 6 + knt) + -1], 0, (-knt + 7) * sizeof(double));
      }
      knt++;
    }
    for (b_i = 0; b_i < 36; b_i++) {
      A[b_i] = rtNaN;
    }
  } else {
    for (i = 0; i < 6; i++) {
      work[i] = 0.0;
    }
    for (i = 0; i < 5; i++) {
      im1n_tmp = i * 6 + 2;
      in = (i + 1) * 6;
      alpha1_tmp = (i + 6 * i) + 1;
      alpha1 = A[alpha1_tmp];
      if (i + 3 <= 6) {
        knt = i + 1;
      } else {
        knt = 4;
      }
      ix0 = knt + im1n_tmp;
      tau[i] = 0.0;
      xnorm = xnrm2(4 - i, A, ix0);
      if (xnorm != 0.0) {
        beta1_tmp = A[alpha1_tmp];
        beta1 = rt_hypotd_snf(beta1_tmp, xnorm);
        if (beta1_tmp >= 0.0) {
          beta1 = -beta1;
        }
        if (fabs(beta1) < 1.0020841800044864E-292) {
          knt = -1;
          b_i = (ix0 - i) + 3;
          do {
            knt++;
            for (k = ix0; k <= b_i; k++) {
              A[k - 1] *= 9.9792015476736E+291;
            }
            beta1 *= 9.9792015476736E+291;
            alpha1 *= 9.9792015476736E+291;
          } while (!(fabs(beta1) >= 1.0020841800044864E-292));
          beta1 = rt_hypotd_snf(alpha1, xnrm2(4 - i, A, ix0));
          if (alpha1 >= 0.0) {
            beta1 = -beta1;
          }
          tau[i] = (beta1 - alpha1) / beta1;
          xnorm = 1.0 / (alpha1 - beta1);
          b_i = (ix0 - i) + 3;
          for (k = ix0; k <= b_i; k++) {
            A[k - 1] *= xnorm;
          }
          for (k = 0; k <= knt; k++) {
            beta1 *= 1.0020841800044864E-292;
          }
          alpha1 = beta1;
        } else {
          xnorm = A[alpha1_tmp];
          tau[i] = (beta1 - xnorm) / beta1;
          xnorm = 1.0 / (xnorm - beta1);
          b_i = (ix0 - i) + 3;
          for (k = ix0; k <= b_i; k++) {
            A[k - 1] *= xnorm;
          }
          alpha1 = beta1;
        }
      }
      A[alpha1_tmp] = 1.0;
      iv0 = (i + im1n_tmp) - 1;
      ic0 = in + 1;
      if (tau[i] != 0.0) {
        lastv = 4 - i;
        knt = (iv0 - i) + 4;
        while ((lastv + 1 > 0) && (A[knt] == 0.0)) {
          lastv--;
          knt--;
        }
        lastc = 6;
        exitg2 = false;
        while ((!exitg2) && (lastc > 0)) {
          knt = in + lastc;
          ia = knt;
          do {
            exitg1 = 0;
            if (ia <= knt + lastv * 6) {
              if (A[ia - 1] != 0.0) {
                exitg1 = 1;
              } else {
                ia += 6;
              }
            } else {
              lastc--;
              exitg1 = 2;
            }
          } while (exitg1 == 0);
          if (exitg1 == 1) {
            exitg2 = true;
          }
        }
      } else {
        lastv = -1;
        lastc = 0;
      }
      if (lastv + 1 > 0) {
        if (lastc != 0) {
          if (0 <= lastc - 1) {
            memset(&work[0], 0, lastc * sizeof(double));
          }
          knt = iv0;
          b_i = (in + 6 * lastv) + 1;
          for (k = ic0; k <= b_i; k += 6) {
            i1 = (k + lastc) - 1;
            for (ia = k; ia <= i1; ia++) {
              ix0 = ia - k;
              work[ix0] += A[ia - 1] * A[knt];
            }
            knt++;
          }
        }
        if (!(-tau[i] == 0.0)) {
          knt = in;
          for (k = 0; k <= lastv; k++) {
            xnorm = A[iv0 + k];
            if (xnorm != 0.0) {
              xnorm *= -tau[i];
              b_i = knt + 1;
              i1 = lastc + knt;
              for (ix0 = b_i; ix0 <= i1; ix0++) {
                A[ix0 - 1] += work[(ix0 - knt) - 1] * xnorm;
              }
            }
            knt += 6;
          }
        }
      }
      xzlarf(5 - i, 5 - i, i + im1n_tmp, tau[i], A, (i + in) + 2, work);
      A[alpha1_tmp] = alpha1;
    }
    memcpy(&V[0], &A[0], 36U * sizeof(double));
    for (k = 4; k >= 0; k--) {
      ia = (k + 1) * 6;
      for (i = 0; i <= k; i++) {
        V[ia + i] = 0.0;
      }
      b_i = k + 3;
      for (i = b_i; i < 7; i++) {
        knt = ia + i;
        V[knt - 1] = V[knt - 7];
      }
    }
    for (i = 0; i < 6; i++) {
      V[i] = 0.0;
    }
    V[0] = 1.0;
    for (i = 0; i < 6; i++) {
      work[i] = 0.0;
    }
    for (i = 4; i >= 0; i--) {
      knt = (i + i * 6) + 7;
      if (i + 1 < 5) {
        V[knt] = 1.0;
        xzlarf(5 - i, 4 - i, knt + 1, tau[i], V, knt + 7, work);
        ix0 = knt + 2;
        b_i = (knt - i) + 5;
        for (k = ix0; k <= b_i; k++) {
          V[k - 1] *= -tau[i];
        }
      }
      V[knt] = 1.0 - tau[i];
      for (k = 0; k < i; k++) {
        V[(knt - k) - 1] = 0.0;
      }
    }
    eml_dlahqr(A, V);
    knt = 4;
    for (k = 0; k < 3; k++) {
      if (knt <= 6) {
        memset(&A[(k * 6 + knt) + -1], 0, (-knt + 7) * sizeof(double));
      }
      knt++;
    }
  }
}

/*
 * Arguments    : const double A[9]
 *                double V[9]
 *                double T[9]
 * Return Type  : void
 */
void schur(const double A[9], double V[9], double T[9])
{
  double work[3];
  double tau[2];
  double b_alpha1_tmp;
  double beta1;
  double xnorm;
  int alpha1_tmp;
  int b_i;
  int c_i;
  int exitg1;
  int i;
  int i1;
  int ia;
  int in;
  int iv0_tmp;
  int ix0;
  int k;
  int knt;
  int lastc;
  int lastv;
  bool exitg2;
  bool p;
  p = true;
  for (k = 0; k < 9; k++) {
    if ((!p) || (rtIsInf(A[k]) || rtIsNaN(A[k]))) {
      p = false;
    }
  }
  if (!p) {
    for (i = 0; i < 9; i++) {
      V[i] = rtNaN;
    }
    b_i = 2;
    for (k = 0; k < 2; k++) {
      if (b_i <= 3) {
        memset(&V[(k * 3 + b_i) + -1], 0, (-b_i + 4) * sizeof(double));
      }
      b_i++;
    }
    for (i = 0; i < 9; i++) {
      T[i] = rtNaN;
    }
  } else {
    memcpy(&T[0], &A[0], 9U * sizeof(double));
    work[0] = 0.0;
    work[1] = 0.0;
    work[2] = 0.0;
    for (c_i = 0; c_i < 2; c_i++) {
      b_i = c_i * 3 + 2;
      in = (c_i + 1) * 3;
      alpha1_tmp = (c_i + 3 * c_i) + 1;
      b_alpha1_tmp = T[alpha1_tmp];
      ix0 = b_i + 1;
      tau[c_i] = 0.0;
      xnorm = 0.0;
      if (1 - c_i >= 1) {
        xnorm = fabs(T[b_i]);
      }
      if (xnorm != 0.0) {
        beta1 = rt_hypotd_snf(b_alpha1_tmp, xnorm);
        if (b_alpha1_tmp >= 0.0) {
          beta1 = -beta1;
        }
        if (fabs(beta1) < 1.0020841800044864E-292) {
          knt = -1;
          i = (b_i - c_i) + 1;
          do {
            knt++;
            for (k = ix0; k <= i; k++) {
              T[k - 1] *= 9.9792015476736E+291;
            }
            beta1 *= 9.9792015476736E+291;
            b_alpha1_tmp *= 9.9792015476736E+291;
          } while (!(fabs(beta1) >= 1.0020841800044864E-292));
          xnorm = 0.0;
          if (1 - c_i >= 1) {
            xnorm = fabs(T[b_i]);
          }
          beta1 = rt_hypotd_snf(b_alpha1_tmp, xnorm);
          if (b_alpha1_tmp >= 0.0) {
            beta1 = -beta1;
          }
          tau[c_i] = (beta1 - b_alpha1_tmp) / beta1;
          xnorm = 1.0 / (b_alpha1_tmp - beta1);
          for (k = ix0; k <= i; k++) {
            T[k - 1] *= xnorm;
          }
          for (k = 0; k <= knt; k++) {
            beta1 *= 1.0020841800044864E-292;
          }
          b_alpha1_tmp = beta1;
        } else {
          tau[c_i] = (beta1 - b_alpha1_tmp) / beta1;
          xnorm = 1.0 / (b_alpha1_tmp - beta1);
          i = (b_i - c_i) + 1;
          for (k = ix0; k <= i; k++) {
            T[k - 1] *= xnorm;
          }
          b_alpha1_tmp = beta1;
        }
      }
      T[alpha1_tmp] = 1.0;
      iv0_tmp = c_i + b_i;
      ix0 = in + 1;
      if (tau[c_i] != 0.0) {
        lastv = 1 - c_i;
        b_i = iv0_tmp - c_i;
        while ((lastv + 1 > 0) && (T[b_i] == 0.0)) {
          lastv--;
          b_i--;
        }
        lastc = 3;
        exitg2 = false;
        while ((!exitg2) && (lastc > 0)) {
          knt = in + lastc;
          ia = knt;
          do {
            exitg1 = 0;
            if (ia <= knt + lastv * 3) {
              if (T[ia - 1] != 0.0) {
                exitg1 = 1;
              } else {
                ia += 3;
              }
            } else {
              lastc--;
              exitg1 = 2;
            }
          } while (exitg1 == 0);
          if (exitg1 == 1) {
            exitg2 = true;
          }
        }
      } else {
        lastv = -1;
        lastc = 0;
      }
      if (lastv + 1 > 0) {
        if (lastc != 0) {
          if (0 <= lastc - 1) {
            memset(&work[0], 0, lastc * sizeof(double));
          }
          knt = iv0_tmp - 1;
          i = (in + 3 * lastv) + 1;
          for (b_i = ix0; b_i <= i; b_i += 3) {
            i1 = (b_i + lastc) - 1;
            for (ia = b_i; ia <= i1; ia++) {
              k = ia - b_i;
              work[k] += T[ia - 1] * T[knt];
            }
            knt++;
          }
        }
        if (!(-tau[c_i] == 0.0)) {
          knt = in;
          for (k = 0; k <= lastv; k++) {
            xnorm = T[(iv0_tmp + k) - 1];
            if (xnorm != 0.0) {
              xnorm *= -tau[c_i];
              i = knt + 1;
              i1 = lastc + knt;
              for (b_i = i; b_i <= i1; b_i++) {
                T[b_i - 1] += work[(b_i - knt) - 1] * xnorm;
              }
            }
            knt += 3;
          }
        }
      }
      ix0 = (c_i + in) + 2;
      if (tau[c_i] != 0.0) {
        lastv = 2 - c_i;
        b_i = iv0_tmp - c_i;
        while ((lastv > 0) && (T[b_i] == 0.0)) {
          lastv--;
          b_i--;
        }
        lastc = 2 - c_i;
        exitg2 = false;
        while ((!exitg2) && (lastc > 0)) {
          knt = ix0 + (lastc - 1) * 3;
          ia = knt;
          do {
            exitg1 = 0;
            if (ia <= (knt + lastv) - 1) {
              if (T[ia - 1] != 0.0) {
                exitg1 = 1;
              } else {
                ia++;
              }
            } else {
              lastc--;
              exitg1 = 2;
            }
          } while (exitg1 == 0);
          if (exitg1 == 1) {
            exitg2 = true;
          }
        }
      } else {
        lastv = 0;
        lastc = 0;
      }
      if (lastv > 0) {
        if (lastc != 0) {
          if (0 <= lastc - 1) {
            memset(&work[0], 0, lastc * sizeof(double));
          }
          i = ix0 + 3 * (lastc - 1);
          for (b_i = ix0; b_i <= i; b_i += 3) {
            xnorm = 0.0;
            i1 = (b_i + lastv) - 1;
            for (ia = b_i; ia <= i1; ia++) {
              xnorm += T[ia - 1] * T[((iv0_tmp + ia) - b_i) - 1];
            }
            k = div_nde_s32_floor(b_i - ix0, 3);
            work[k] += xnorm;
          }
        }
        xgerc(lastv, lastc, -tau[c_i], iv0_tmp, work, T, ix0);
      }
      T[alpha1_tmp] = b_alpha1_tmp;
    }
    memcpy(&V[0], &T[0], 9U * sizeof(double));
    for (k = 1; k >= 0; k--) {
      ia = (k + 1) * 3;
      for (c_i = 0; c_i <= k; c_i++) {
        V[ia + c_i] = 0.0;
      }
      i = k + 3;
      for (c_i = i; c_i < 4; c_i++) {
        V[ia + 2] = V[ia - 1];
      }
    }
    V[1] = 0.0;
    V[2] = 0.0;
    V[0] = 1.0;
    work[0] = 0.0;
    work[1] = 0.0;
    work[2] = 0.0;
    for (c_i = 1; c_i >= 0; c_i--) {
      knt = (c_i + c_i * 3) + 8;
      if (c_i + 1 < 2) {
        V[knt - 4] = 1.0;
        if (tau[c_i] != 0.0) {
          lastv = 2;
          b_i = knt;
          while ((lastv > 0) && (V[b_i - 3] == 0.0)) {
            lastv--;
            b_i--;
          }
          lastc = 1;
          ia = knt;
          do {
            exitg1 = 0;
            if (ia <= (knt + lastv) - 1) {
              if (V[ia - 1] != 0.0) {
                exitg1 = 1;
              } else {
                ia++;
              }
            } else {
              lastc = 0;
              exitg1 = 1;
            }
          } while (exitg1 == 0);
        } else {
          lastv = 0;
          lastc = 0;
        }
        if (lastv > 0) {
          if (lastc != 0) {
            work[0] = 0.0;
            for (b_i = knt; b_i <= knt; b_i += 3) {
              xnorm = 0.0;
              i = (b_i + lastv) - 1;
              for (ia = b_i; ia <= i; ia++) {
                xnorm += V[ia - 1] * V[((knt + ia) - b_i) - 4];
              }
              k = div_nde_s32_floor(b_i - knt, 3);
              work[k] += xnorm;
            }
          }
          xgerc(lastv, lastc, -tau[c_i], knt - 3, work, V, knt);
        }
        ix0 = knt - 2;
        for (k = ix0; k <= ix0; k++) {
          V[k - 1] *= -tau[c_i];
        }
      }
      V[knt - 4] = 1.0 - tau[c_i];
      if (0 <= c_i - 1) {
        V[knt - 5] = 0.0;
      }
    }
    xhseqr(T, V);
  }
}

/*
 * File trailer for schur.c
 *
 * [EOF]
 */
