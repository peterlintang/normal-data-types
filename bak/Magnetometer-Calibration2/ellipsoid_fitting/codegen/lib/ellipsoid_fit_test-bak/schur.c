/*
 * File: schur.c
 *
 * MATLAB Coder version            : 5.3
 * C/C++ source code generated on  : 02-Aug-2024 10:19:14
 */

/* Include Files */
#include "schur.h"
#include "ellipsoid_fit_test_data.h"
#include "ellipsoid_fit_test_rtwutil.h"
#include "recip.h"
#include "rt_nonfinite.h"
#include "xdhseqr.h"
#include "xdlapy3.h"
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
 * Arguments    : const creal_T A[9]
 *                creal_T V[9]
 *                creal_T T[9]
 * Return Type  : void
 */
void schur(const creal_T A[9], creal_T V[9], creal_T T[9])
{
  creal_T work[3];
  creal_T tau[2];
  creal_T c;
  double alpha1_im;
  double alpha1_re;
  double b_c_tmp;
  double b_tau_tmp;
  double beta1;
  double c_tmp;
  double tau_tmp;
  double temp_im;
  double temp_re;
  double xnorm;
  int alpha1_re_tmp;
  int b_i;
  int c_i;
  int exitg1;
  int i;
  int i1;
  int ia;
  int in;
  int iv0_tmp;
  int ix0;
  int jA;
  int knt;
  int lastc;
  int lastv;
  bool exitg2;
  bool p;
  p = true;
  for (jA = 0; jA < 9; jA++) {
    if ((!p) || (rtIsInf(A[jA].re) || rtIsInf(A[jA].im) ||
                 (rtIsNaN(A[jA].re) || rtIsNaN(A[jA].im)))) {
      p = false;
    }
  }
  if (!p) {
    for (i = 0; i < 9; i++) {
      V[i].re = rtNaN;
      V[i].im = 0.0;
    }
    b_i = 2;
    for (ix0 = 0; ix0 < 2; ix0++) {
      if (b_i <= 3) {
        memset(&V[(ix0 * 3 + b_i) + -1], 0, (-b_i + 4) * sizeof(creal_T));
      }
      b_i++;
    }
    for (i = 0; i < 9; i++) {
      T[i].re = rtNaN;
      T[i].im = 0.0;
    }
  } else {
    memcpy(&T[0], &A[0], 9U * sizeof(creal_T));
    work[0].re = 0.0;
    work[0].im = 0.0;
    work[1].re = 0.0;
    work[1].im = 0.0;
    work[2].re = 0.0;
    work[2].im = 0.0;
    for (c_i = 0; c_i < 2; c_i++) {
      b_i = c_i * 3 + 2;
      in = (c_i + 1) * 3;
      alpha1_re_tmp = (c_i + 3 * c_i) + 1;
      alpha1_re = T[alpha1_re_tmp].re;
      alpha1_im = T[alpha1_re_tmp].im;
      ix0 = b_i + 1;
      tau[c_i].re = 0.0;
      tau[c_i].im = 0.0;
      xnorm = 0.0;
      if (1 - c_i >= 1) {
        xnorm = rt_hypotd_snf(T[b_i].re, T[b_i].im);
      }
      if ((xnorm != 0.0) || (T[alpha1_re_tmp].im != 0.0)) {
        beta1 = xdlapy3(alpha1_re, alpha1_im, xnorm);
        if (T[alpha1_re_tmp].re >= 0.0) {
          beta1 = -beta1;
        }
        if (fabs(beta1) < 1.0020841800044864E-292) {
          knt = -1;
          i = (b_i - c_i) + 1;
          do {
            knt++;
            for (jA = ix0; jA <= i; jA++) {
              xnorm = T[jA - 1].im;
              temp_re = T[jA - 1].re;
              T[jA - 1].re = 9.9792015476736E+291 * temp_re - 0.0 * xnorm;
              T[jA - 1].im = 9.9792015476736E+291 * xnorm + 0.0 * temp_re;
            }
            beta1 *= 9.9792015476736E+291;
            alpha1_re *= 9.9792015476736E+291;
            alpha1_im *= 9.9792015476736E+291;
          } while (!(fabs(beta1) >= 1.0020841800044864E-292));
          xnorm = 0.0;
          if (1 - c_i >= 1) {
            xnorm = rt_hypotd_snf(T[b_i].re, T[b_i].im);
          }
          beta1 = xdlapy3(alpha1_re, alpha1_im, xnorm);
          if (alpha1_re >= 0.0) {
            beta1 = -beta1;
          }
          xnorm = beta1 - alpha1_re;
          if (0.0 - alpha1_im == 0.0) {
            tau[c_i].re = xnorm / beta1;
            tau[c_i].im = 0.0;
          } else if (xnorm == 0.0) {
            tau[c_i].re = 0.0;
            tau[c_i].im = (0.0 - alpha1_im) / beta1;
          } else {
            tau[c_i].re = xnorm / beta1;
            tau[c_i].im = (0.0 - alpha1_im) / beta1;
          }
          c.re = alpha1_re - beta1;
          c.im = alpha1_im;
          c = recip(c);
          for (jA = ix0; jA <= i; jA++) {
            xnorm = T[jA - 1].im;
            temp_re = T[jA - 1].re;
            T[jA - 1].re = c.re * temp_re - c.im * xnorm;
            T[jA - 1].im = c.re * xnorm + c.im * temp_re;
          }
          for (jA = 0; jA <= knt; jA++) {
            beta1 *= 1.0020841800044864E-292;
          }
          alpha1_re = beta1;
          alpha1_im = 0.0;
        } else {
          xnorm = beta1 - alpha1_re;
          if (0.0 - alpha1_im == 0.0) {
            tau[c_i].re = xnorm / beta1;
            tau[c_i].im = 0.0;
          } else if (xnorm == 0.0) {
            tau[c_i].re = 0.0;
            tau[c_i].im = (0.0 - alpha1_im) / beta1;
          } else {
            tau[c_i].re = xnorm / beta1;
            tau[c_i].im = (0.0 - alpha1_im) / beta1;
          }
          c.re = alpha1_re - beta1;
          c.im = alpha1_im;
          c = recip(c);
          i = (b_i - c_i) + 1;
          for (jA = ix0; jA <= i; jA++) {
            xnorm = T[jA - 1].im;
            temp_re = T[jA - 1].re;
            T[jA - 1].re = c.re * temp_re - c.im * xnorm;
            T[jA - 1].im = c.re * xnorm + c.im * temp_re;
          }
          alpha1_re = beta1;
          alpha1_im = 0.0;
        }
      }
      T[alpha1_re_tmp].re = 1.0;
      T[alpha1_re_tmp].im = 0.0;
      iv0_tmp = c_i + b_i;
      jA = in + 1;
      tau_tmp = tau[c_i].re;
      b_tau_tmp = tau[c_i].im;
      if ((tau_tmp != 0.0) || (b_tau_tmp != 0.0)) {
        lastv = 1 - c_i;
        b_i = iv0_tmp - c_i;
        while ((lastv + 1 > 0) && ((T[b_i].re == 0.0) && (T[b_i].im == 0.0))) {
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
              if ((T[ia - 1].re != 0.0) || (T[ia - 1].im != 0.0)) {
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
            memset(&work[0], 0, lastc * sizeof(creal_T));
          }
          i = (in + 3 * lastv) + 1;
          for (ix0 = jA; ix0 <= i; ix0 += 3) {
            b_i = (iv0_tmp + div_nde_s32_floor((ix0 - in) - 1, 3)) - 1;
            c_tmp = T[b_i].re;
            b_c_tmp = T[b_i].im;
            c.re = c_tmp - 0.0 * b_c_tmp;
            c.im = b_c_tmp + 0.0 * c_tmp;
            i1 = (ix0 + lastc) - 1;
            for (ia = ix0; ia <= i1; ia++) {
              xnorm = T[ia - 1].re;
              beta1 = T[ia - 1].im;
              knt = ia - ix0;
              work[knt].re += xnorm * c.re - beta1 * c.im;
              work[knt].im += xnorm * c.im + beta1 * c.re;
            }
          }
        }
        if ((!(-tau_tmp == 0.0)) || (!(-b_tau_tmp == 0.0))) {
          jA = in;
          for (ix0 = 0; ix0 <= lastv; ix0++) {
            knt = (iv0_tmp + ix0) - 1;
            xnorm = T[knt].re;
            beta1 = T[knt].im;
            if ((xnorm != 0.0) || (beta1 != 0.0)) {
              temp_re = xnorm * -tau_tmp + beta1 * -b_tau_tmp;
              temp_im = xnorm * -b_tau_tmp - beta1 * -tau_tmp;
              i = jA + 1;
              i1 = lastc + jA;
              for (knt = i; knt <= i1; knt++) {
                b_i = (knt - jA) - 1;
                xnorm = work[b_i].re;
                beta1 = work[b_i].im;
                T[knt - 1].re += xnorm * temp_re - beta1 * temp_im;
                T[knt - 1].im += xnorm * temp_im + beta1 * temp_re;
              }
            }
            jA += 3;
          }
        }
      }
      jA = (c_i + in) + 2;
      temp_im = -tau[c_i].im;
      if ((tau_tmp != 0.0) || (temp_im != 0.0)) {
        lastv = 2 - c_i;
        b_i = iv0_tmp - c_i;
        while ((lastv > 0) && ((T[b_i].re == 0.0) && (T[b_i].im == 0.0))) {
          lastv--;
          b_i--;
        }
        lastc = 2 - c_i;
        exitg2 = false;
        while ((!exitg2) && (lastc > 0)) {
          knt = jA + (lastc - 1) * 3;
          ia = knt;
          do {
            exitg1 = 0;
            if (ia <= (knt + lastv) - 1) {
              if ((T[ia - 1].re != 0.0) || (T[ia - 1].im != 0.0)) {
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
            memset(&work[0], 0, lastc * sizeof(creal_T));
          }
          i = jA + 3 * (lastc - 1);
          for (ix0 = jA; ix0 <= i; ix0 += 3) {
            c.re = 0.0;
            c.im = 0.0;
            i1 = (ix0 + lastv) - 1;
            for (ia = ix0; ia <= i1; ia++) {
              b_i = ((iv0_tmp + ia) - ix0) - 1;
              c_tmp = T[ia - 1].re;
              b_c_tmp = T[b_i].im;
              xnorm = T[ia - 1].im;
              temp_re = T[b_i].re;
              c.re += c_tmp * temp_re + xnorm * b_c_tmp;
              c.im += c_tmp * b_c_tmp - xnorm * temp_re;
            }
            i1 = div_nde_s32_floor(ix0 - jA, 3);
            work[i1].re += c.re - 0.0 * c.im;
            work[i1].im += c.im + 0.0 * c.re;
          }
        }
        c.re = -tau_tmp;
        c.im = -temp_im;
        xgerc(lastv, lastc, c, iv0_tmp, work, T, jA);
      }
      T[alpha1_re_tmp].re = alpha1_re;
      T[alpha1_re_tmp].im = alpha1_im;
    }
    memcpy(&V[0], &T[0], 9U * sizeof(creal_T));
    for (ix0 = 1; ix0 >= 0; ix0--) {
      ia = (ix0 + 1) * 3;
      for (c_i = 0; c_i <= ix0; c_i++) {
        i = ia + c_i;
        V[i].re = 0.0;
        V[i].im = 0.0;
      }
      i = ix0 + 3;
      for (c_i = i; c_i < 4; c_i++) {
        V[ia + 2] = V[ia - 1];
      }
    }
    V[1].re = 0.0;
    V[1].im = 0.0;
    V[2].re = 0.0;
    V[2].im = 0.0;
    V[0].re = 1.0;
    V[0].im = 0.0;
    work[0].re = 0.0;
    work[0].im = 0.0;
    work[1].re = 0.0;
    work[1].im = 0.0;
    work[2].re = 0.0;
    work[2].im = 0.0;
    for (c_i = 1; c_i >= 0; c_i--) {
      knt = (c_i + c_i * 3) + 8;
      if (c_i + 1 < 2) {
        V[knt - 4].re = 1.0;
        V[knt - 4].im = 0.0;
        beta1 = tau[c_i].re;
        temp_im = tau[c_i].im;
        if ((beta1 != 0.0) || (temp_im != 0.0)) {
          lastv = 2;
          b_i = knt;
          while ((lastv > 0) &&
                 ((V[b_i - 3].re == 0.0) && (V[b_i - 3].im == 0.0))) {
            lastv--;
            b_i--;
          }
          lastc = 1;
          ia = knt;
          do {
            exitg1 = 0;
            if (ia <= (knt + lastv) - 1) {
              if ((V[ia - 1].re != 0.0) || (V[ia - 1].im != 0.0)) {
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
            work[0].re = 0.0;
            work[0].im = 0.0;
            for (ix0 = knt; ix0 <= knt; ix0 += 3) {
              c.re = 0.0;
              c.im = 0.0;
              i = (ix0 + lastv) - 1;
              for (ia = ix0; ia <= i; ia++) {
                b_i = ((knt + ia) - ix0) - 4;
                c_tmp = V[ia - 1].re;
                b_c_tmp = V[b_i].im;
                xnorm = V[ia - 1].im;
                temp_re = V[b_i].re;
                c.re += c_tmp * temp_re + xnorm * b_c_tmp;
                c.im += c_tmp * b_c_tmp - xnorm * temp_re;
              }
              i = div_nde_s32_floor(ix0 - knt, 3);
              work[i].re += c.re - 0.0 * c.im;
              work[i].im += c.im + 0.0 * c.re;
            }
          }
          c.re = -beta1;
          c.im = -temp_im;
          xgerc(lastv, lastc, c, knt - 3, work, V, knt);
        }
        ix0 = knt - 2;
        for (jA = ix0; jA <= ix0; jA++) {
          xnorm = V[jA - 1].im;
          temp_re = V[jA - 1].re;
          V[jA - 1].re = -beta1 * temp_re - -temp_im * xnorm;
          V[jA - 1].im = -beta1 * xnorm + -temp_im * temp_re;
        }
      }
      V[knt - 4].re = 1.0 - tau[c_i].re;
      V[knt - 4].im = 0.0 - tau[c_i].im;
      if (0 <= c_i - 1) {
        V[knt - 5].re = 0.0;
        V[knt - 5].im = 0.0;
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
