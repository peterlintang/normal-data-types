/*
 * File: xzhgeqz.c
 *
 * MATLAB Coder version            : 5.3
 * C/C++ source code generated on  : 02-Aug-2024 10:19:14
 */

/* Include Files */
#include "xzhgeqz.h"
#include "ellipsoid_fit_test_data.h"
#include "rt_nonfinite.h"
#include "sqrt.h"
#include "xzlartg.h"
#include <math.h>

/* Function Definitions */
/*
 * Arguments    : creal_T A[9]
 *                int ilo
 *                int ihi
 *                creal_T Z[9]
 *                int *info
 *                creal_T alpha1[3]
 *                creal_T beta1[3]
 * Return Type  : void
 */
void b_xzhgeqz(creal_T A[9], int ilo, int ihi, creal_T Z[9], int *info,
               creal_T alpha1[3], creal_T beta1[3])
{
  creal_T b_ascale;
  creal_T ctemp;
  creal_T shift;
  double ad22_im;
  double ad22_re;
  double anorm;
  double ascale;
  double b_atol;
  double colscale;
  double colssq;
  double eshift_im;
  double eshift_re;
  double scale;
  double ssq;
  double t;
  double t1_im;
  double t1_im_tmp;
  double t1_re;
  int col;
  int exitg1;
  int i;
  int ifirst;
  int iiter;
  int ilast;
  int ilastm1;
  int istart;
  int j;
  int jiter;
  int jp1;
  int nm1;
  int row;
  bool b_guard1 = false;
  bool exitg2;
  bool failed;
  bool goto60;
  bool goto70;
  bool goto90;
  bool guard1 = false;
  bool guard2 = false;
  bool guard3 = false;
  *info = 0;
  alpha1[0].re = 0.0;
  alpha1[0].im = 0.0;
  beta1[0].re = 1.0;
  beta1[0].im = 0.0;
  alpha1[1].re = 0.0;
  alpha1[1].im = 0.0;
  beta1[1].re = 1.0;
  beta1[1].im = 0.0;
  alpha1[2].re = 0.0;
  alpha1[2].im = 0.0;
  beta1[2].re = 1.0;
  beta1[2].im = 0.0;
  eshift_re = 0.0;
  eshift_im = 0.0;
  ctemp.re = 0.0;
  ctemp.im = 0.0;
  anorm = 0.0;
  if (ilo <= ihi) {
    scale = 3.3121686421112381E-170;
    ssq = 0.0;
    nm1 = ihi - ilo;
    for (j = 0; j <= nm1; j++) {
      colscale = 3.3121686421112381E-170;
      colssq = 0.0;
      col = (ilo + j) - 1;
      jp1 = j + 1;
      if (jp1 > nm1) {
        jp1 = nm1;
      }
      i = ilo + jp1;
      for (row = ilo; row <= i; row++) {
        jp1 = (row + 3 * col) - 1;
        anorm = fabs(A[jp1].re);
        if (anorm > colscale) {
          t = colscale / anorm;
          colssq = colssq * t * t + 1.0;
          colscale = anorm;
        } else {
          t = anorm / colscale;
          colssq += t * t;
        }
        anorm = fabs(A[jp1].im);
        if (anorm > colscale) {
          t = colscale / anorm;
          colssq = colssq * t * t + 1.0;
          colscale = anorm;
        } else {
          t = anorm / colscale;
          colssq += t * t;
        }
      }
      if (scale >= colscale) {
        t = colscale / scale;
        ssq += t * t * colssq;
      } else {
        t = scale / colscale;
        ssq = colssq + t * t * ssq;
        scale = colscale;
      }
    }
    anorm = scale * sqrt(ssq);
  }
  t = 2.2204460492503131E-16 * anorm;
  b_atol = 2.2250738585072014E-308;
  if (t > 2.2250738585072014E-308) {
    b_atol = t;
  }
  t = 2.2250738585072014E-308;
  if (anorm > 2.2250738585072014E-308) {
    t = anorm;
  }
  ascale = 1.0 / t;
  failed = true;
  i = ihi + 1;
  for (j = i; j < 4; j++) {
    alpha1[j - 1] = A[(j + 3 * (j - 1)) - 1];
  }
  guard1 = false;
  guard2 = false;
  if (ihi >= ilo) {
    ifirst = ilo;
    istart = ilo;
    ilast = ihi - 1;
    ilastm1 = ihi - 2;
    iiter = 0;
    goto60 = false;
    goto70 = false;
    goto90 = false;
    jiter = 0;
    do {
      exitg1 = 0;
      if (jiter <= 30 * ((ihi - ilo) + 1) - 1) {
        b_guard1 = false;
        if (ilast + 1 == ilo) {
          goto60 = true;
          b_guard1 = true;
        } else {
          i = ilast + 3 * ilastm1;
          if (fabs(A[i].re) + fabs(A[i].im) <= b_atol) {
            A[i].re = 0.0;
            A[i].im = 0.0;
            goto60 = true;
            b_guard1 = true;
          } else {
            j = ilastm1;
            guard3 = false;
            exitg2 = false;
            while ((!exitg2) && (j + 1 >= ilo)) {
              if (j + 1 == ilo) {
                guard3 = true;
                exitg2 = true;
              } else {
                i = j + 3 * (j - 1);
                if (fabs(A[i].re) + fabs(A[i].im) <= b_atol) {
                  A[i].re = 0.0;
                  A[i].im = 0.0;
                  guard3 = true;
                  exitg2 = true;
                } else {
                  j--;
                  guard3 = false;
                }
              }
            }
            if (guard3) {
              ifirst = j + 1;
              goto70 = true;
            }
            if (goto70) {
              b_guard1 = true;
            } else {
              alpha1[0].re = rtNaN;
              alpha1[0].im = 0.0;
              beta1[0].re = rtNaN;
              beta1[0].im = 0.0;
              alpha1[1].re = rtNaN;
              alpha1[1].im = 0.0;
              beta1[1].re = rtNaN;
              beta1[1].im = 0.0;
              alpha1[2].re = rtNaN;
              alpha1[2].im = 0.0;
              beta1[2].re = rtNaN;
              beta1[2].im = 0.0;
              for (i = 0; i < 9; i++) {
                Z[i].re = rtNaN;
                Z[i].im = 0.0;
              }
              *info = 1;
              exitg1 = 1;
            }
          }
        }
        if (b_guard1) {
          if (goto60) {
            goto60 = false;
            alpha1[ilast] = A[ilast + 3 * ilast];
            ilast = ilastm1;
            ilastm1--;
            if (ilast + 1 < ilo) {
              failed = false;
              guard2 = true;
              exitg1 = 1;
            } else {
              iiter = 0;
              eshift_re = 0.0;
              eshift_im = 0.0;
              jiter++;
            }
          } else {
            if (goto70) {
              goto70 = false;
              iiter++;
              if (iiter - iiter / 10 * 10 != 0) {
                jp1 = ilastm1 + 3 * ilastm1;
                anorm = ascale * A[jp1].re;
                t = ascale * A[jp1].im;
                if (t == 0.0) {
                  shift.re = anorm / 0.57735026918962584;
                  shift.im = 0.0;
                } else if (anorm == 0.0) {
                  shift.re = 0.0;
                  shift.im = t / 0.57735026918962584;
                } else {
                  shift.re = anorm / 0.57735026918962584;
                  shift.im = t / 0.57735026918962584;
                }
                jp1 = ilast + 3 * ilast;
                anorm = ascale * A[jp1].re;
                t = ascale * A[jp1].im;
                if (t == 0.0) {
                  ad22_re = anorm / 0.57735026918962584;
                  ad22_im = 0.0;
                } else if (anorm == 0.0) {
                  ad22_re = 0.0;
                  ad22_im = t / 0.57735026918962584;
                } else {
                  ad22_re = anorm / 0.57735026918962584;
                  ad22_im = t / 0.57735026918962584;
                }
                t1_re = 0.5 * (shift.re + ad22_re);
                t1_im = 0.5 * (shift.im + ad22_im);
                t1_im_tmp = t1_re * t1_im;
                jp1 = ilastm1 + 3 * ilast;
                anorm = ascale * A[jp1].re;
                t = ascale * A[jp1].im;
                if (t == 0.0) {
                  colscale = anorm / 0.57735026918962584;
                  colssq = 0.0;
                } else if (anorm == 0.0) {
                  colscale = 0.0;
                  colssq = t / 0.57735026918962584;
                } else {
                  colscale = anorm / 0.57735026918962584;
                  colssq = t / 0.57735026918962584;
                }
                jp1 = ilast + 3 * ilastm1;
                anorm = ascale * A[jp1].re;
                t = ascale * A[jp1].im;
                if (t == 0.0) {
                  ssq = anorm / 0.57735026918962584;
                  anorm = 0.0;
                } else if (anorm == 0.0) {
                  ssq = 0.0;
                  anorm = t / 0.57735026918962584;
                } else {
                  ssq = anorm / 0.57735026918962584;
                  anorm = t / 0.57735026918962584;
                }
                t = shift.re * ad22_re - shift.im * ad22_im;
                scale = shift.re * ad22_im + shift.im * ad22_re;
                shift.re = ((t1_re * t1_re - t1_im * t1_im) +
                            (colscale * ssq - colssq * anorm)) -
                           t;
                shift.im = ((t1_im_tmp + t1_im_tmp) +
                            (colscale * anorm + colssq * ssq)) -
                           scale;
                b_sqrt(&shift);
                if ((t1_re - ad22_re) * shift.re +
                        (t1_im - ad22_im) * shift.im <=
                    0.0) {
                  shift.re += t1_re;
                  shift.im += t1_im;
                } else {
                  shift.re = t1_re - shift.re;
                  shift.im = t1_im - shift.im;
                }
              } else {
                jp1 = ilast + 3 * ilastm1;
                anorm = ascale * A[jp1].re;
                t = ascale * A[jp1].im;
                if (t == 0.0) {
                  colscale = anorm / 0.57735026918962584;
                  colssq = 0.0;
                } else if (anorm == 0.0) {
                  colscale = 0.0;
                  colssq = t / 0.57735026918962584;
                } else {
                  colscale = anorm / 0.57735026918962584;
                  colssq = t / 0.57735026918962584;
                }
                eshift_re += colscale;
                eshift_im += colssq;
                shift.re = eshift_re;
                shift.im = eshift_im;
              }
              j = ilastm1;
              jp1 = ilastm1 + 1;
              exitg2 = false;
              while ((!exitg2) && (j + 1 > ifirst)) {
                istart = j + 1;
                nm1 = j + 3 * j;
                ctemp.re = ascale * A[nm1].re - shift.re * 0.57735026918962584;
                ctemp.im = ascale * A[nm1].im - shift.im * 0.57735026918962584;
                anorm = fabs(ctemp.re) + fabs(ctemp.im);
                jp1 += 3 * j;
                t = ascale * (fabs(A[jp1].re) + fabs(A[jp1].im));
                scale = anorm;
                if (t > anorm) {
                  scale = t;
                }
                if ((scale < 1.0) && (scale != 0.0)) {
                  anorm /= scale;
                  t /= scale;
                }
                i = j + 3 * (j - 1);
                if ((fabs(A[i].re) + fabs(A[i].im)) * t <= anorm * b_atol) {
                  goto90 = true;
                  exitg2 = true;
                } else {
                  jp1 = j;
                  j--;
                }
              }
              if (!goto90) {
                istart = ifirst;
                nm1 = (ifirst + 3 * (ifirst - 1)) - 1;
                ctemp.re = ascale * A[nm1].re - shift.re * 0.57735026918962584;
                ctemp.im = ascale * A[nm1].im - shift.im * 0.57735026918962584;
              }
              goto90 = false;
              jp1 = istart + 3 * (istart - 1);
              b_ascale.re = ascale * A[jp1].re;
              b_ascale.im = ascale * A[jp1].im;
              b_xzlartg(ctemp, b_ascale, &anorm, &shift);
              j = istart;
              nm1 = istart - 2;
              while (j < ilast + 1) {
                if (j > istart) {
                  col = j + 3 * nm1;
                  xzlartg(A[3 * nm1 + 1], A[col], &anorm, &shift,
                          &A[3 * nm1 + 1]);
                  A[col].re = 0.0;
                  A[col].im = 0.0;
                }
                for (nm1 = j; nm1 < 4; nm1++) {
                  row = j + 3 * (nm1 - 1);
                  t = A[row].im;
                  scale = A[row].re;
                  ssq = A[row - 1].re;
                  colscale = A[row - 1].im;
                  A[row].re =
                      anorm * scale - (shift.re * ssq + shift.im * colscale);
                  A[row].im = anorm * A[row].im -
                              (shift.re * colscale - shift.im * ssq);
                  A[row - 1].re =
                      anorm * ssq + (shift.re * scale - shift.im * t);
                  A[row - 1].im =
                      anorm * colscale + (shift.re * t + shift.im * scale);
                }
                shift.re = -shift.re;
                shift.im = -shift.im;
                nm1 = j;
                if (ilast + 1 < j + 2) {
                  nm1 = ilast - 1;
                }
                for (jp1 = 1; jp1 <= nm1 + 2; jp1++) {
                  row = (jp1 + 3 * (j - 1)) - 1;
                  t = A[row].im;
                  scale = A[row].re;
                  col = (jp1 + 3 * j) - 1;
                  ssq = A[col].re;
                  colscale = A[col].im;
                  A[row].re =
                      anorm * scale - (shift.re * ssq + shift.im * colscale);
                  A[row].im = anorm * A[row].im -
                              (shift.re * colscale - shift.im * ssq);
                  A[col].re = anorm * ssq + (shift.re * scale - shift.im * t);
                  A[col].im =
                      anorm * colscale + (shift.re * t + shift.im * scale);
                }
                row = 3 * (j - 1);
                t = Z[row].im;
                scale = Z[row].re;
                ssq = Z[3 * j].re;
                colscale = Z[3 * j].im;
                Z[row].re =
                    anorm * scale - (shift.re * ssq + shift.im * colscale);
                Z[row].im =
                    anorm * Z[row].im - (shift.re * colscale - shift.im * ssq);
                Z[3 * j].re = anorm * ssq + (shift.re * scale - shift.im * t);
                Z[3 * j].im =
                    anorm * colscale + (shift.re * t + shift.im * scale);
                t = Z[row + 1].im;
                scale = Z[row + 1].re;
                col = 3 * j + 1;
                ssq = Z[col].re;
                colscale = Z[col].im;
                Z[row + 1].re =
                    anorm * scale - (shift.re * ssq + shift.im * colscale);
                Z[row + 1].im = anorm * Z[row + 1].im -
                                (shift.re * colscale - shift.im * ssq);
                Z[col].re = anorm * ssq + (shift.re * scale - shift.im * t);
                Z[col].im =
                    anorm * colscale + (shift.re * t + shift.im * scale);
                t = Z[row + 2].im;
                scale = Z[row + 2].re;
                col = 3 * j + 2;
                ssq = Z[col].re;
                colscale = Z[col].im;
                Z[row + 2].re =
                    anorm * scale - (shift.re * ssq + shift.im * colscale);
                Z[row + 2].im = anorm * Z[row + 2].im -
                                (shift.re * colscale - shift.im * ssq);
                Z[col].re = anorm * ssq + (shift.re * scale - shift.im * t);
                Z[col].im =
                    anorm * colscale + (shift.re * t + shift.im * scale);
                nm1 = j - 1;
                j++;
              }
            }
            jiter++;
          }
        }
      } else {
        guard2 = true;
        exitg1 = 1;
      }
    } while (exitg1 == 0);
  } else {
    guard1 = true;
  }
  if (guard2) {
    if (failed) {
      *info = ilast + 1;
      for (jp1 = 0; jp1 <= ilast; jp1++) {
        alpha1[jp1].re = rtNaN;
        alpha1[jp1].im = 0.0;
        beta1[jp1].re = rtNaN;
        beta1[jp1].im = 0.0;
      }
      for (i = 0; i < 9; i++) {
        Z[i].re = rtNaN;
        Z[i].im = 0.0;
      }
    } else {
      guard1 = true;
    }
  }
  if (guard1) {
    for (j = 0; j <= ilo - 2; j++) {
      alpha1[j] = A[j + 3 * j];
    }
  }
}

/*
 * Arguments    : creal_T A[36]
 *                int ilo
 *                int ihi
 *                creal_T Z[36]
 *                int *info
 *                creal_T alpha1[6]
 *                creal_T beta1[6]
 * Return Type  : void
 */
void xzhgeqz(creal_T A[36], int ilo, int ihi, creal_T Z[36], int *info,
             creal_T alpha1[6], creal_T beta1[6])
{
  creal_T b_ascale;
  creal_T ctemp;
  creal_T shift;
  double ad22_im;
  double ad22_re;
  double anorm;
  double ascale;
  double b_atol;
  double colscale;
  double colssq;
  double eshift_im;
  double eshift_re;
  double scale;
  double ssq;
  double t;
  double t1_im;
  double t1_im_tmp;
  double t1_re;
  int b_i;
  int col;
  int exitg1;
  int i;
  int ifirst;
  int iiter;
  int ilast;
  int ilastm1;
  int istart;
  int j;
  int jiter;
  int nm1;
  int row;
  bool b_guard1 = false;
  bool exitg2;
  bool failed;
  bool goto60;
  bool goto70;
  bool goto90;
  bool guard1 = false;
  bool guard2 = false;
  bool guard3 = false;
  *info = 0;
  for (i = 0; i < 6; i++) {
    alpha1[i].re = 0.0;
    alpha1[i].im = 0.0;
    beta1[i].re = 1.0;
    beta1[i].im = 0.0;
  }
  eshift_re = 0.0;
  eshift_im = 0.0;
  ctemp.re = 0.0;
  ctemp.im = 0.0;
  anorm = 0.0;
  if (ilo <= ihi) {
    scale = 3.3121686421112381E-170;
    ssq = 0.0;
    nm1 = ihi - ilo;
    for (j = 0; j <= nm1; j++) {
      colscale = 3.3121686421112381E-170;
      colssq = 0.0;
      col = (ilo + j) - 1;
      i = j + 1;
      if (i > nm1) {
        i = nm1;
      }
      b_i = ilo + i;
      for (row = ilo; row <= b_i; row++) {
        i = (row + 6 * col) - 1;
        anorm = fabs(A[i].re);
        if (anorm > colscale) {
          t = colscale / anorm;
          colssq = colssq * t * t + 1.0;
          colscale = anorm;
        } else {
          t = anorm / colscale;
          colssq += t * t;
        }
        anorm = fabs(A[i].im);
        if (anorm > colscale) {
          t = colscale / anorm;
          colssq = colssq * t * t + 1.0;
          colscale = anorm;
        } else {
          t = anorm / colscale;
          colssq += t * t;
        }
      }
      if (scale >= colscale) {
        t = colscale / scale;
        ssq += t * t * colssq;
      } else {
        t = scale / colscale;
        ssq = colssq + t * t * ssq;
        scale = colscale;
      }
    }
    anorm = scale * sqrt(ssq);
  }
  t = 2.2204460492503131E-16 * anorm;
  b_atol = 2.2250738585072014E-308;
  if (t > 2.2250738585072014E-308) {
    b_atol = t;
  }
  t = 2.2250738585072014E-308;
  if (anorm > 2.2250738585072014E-308) {
    t = anorm;
  }
  ascale = 1.0 / t;
  failed = true;
  b_i = ihi + 1;
  for (j = b_i; j < 7; j++) {
    alpha1[j - 1] = A[(j + 6 * (j - 1)) - 1];
  }
  guard1 = false;
  guard2 = false;
  if (ihi >= ilo) {
    ifirst = ilo;
    istart = ilo;
    ilast = ihi - 1;
    ilastm1 = ihi - 2;
    iiter = 0;
    goto60 = false;
    goto70 = false;
    goto90 = false;
    jiter = 0;
    do {
      exitg1 = 0;
      if (jiter <= 30 * ((ihi - ilo) + 1) - 1) {
        b_guard1 = false;
        if (ilast + 1 == ilo) {
          goto60 = true;
          b_guard1 = true;
        } else {
          b_i = ilast + 6 * ilastm1;
          if (fabs(A[b_i].re) + fabs(A[b_i].im) <= b_atol) {
            A[b_i].re = 0.0;
            A[b_i].im = 0.0;
            goto60 = true;
            b_guard1 = true;
          } else {
            j = ilastm1;
            guard3 = false;
            exitg2 = false;
            while ((!exitg2) && (j + 1 >= ilo)) {
              if (j + 1 == ilo) {
                guard3 = true;
                exitg2 = true;
              } else {
                b_i = j + 6 * (j - 1);
                if (fabs(A[b_i].re) + fabs(A[b_i].im) <= b_atol) {
                  A[b_i].re = 0.0;
                  A[b_i].im = 0.0;
                  guard3 = true;
                  exitg2 = true;
                } else {
                  j--;
                  guard3 = false;
                }
              }
            }
            if (guard3) {
              ifirst = j + 1;
              goto70 = true;
            }
            if (goto70) {
              b_guard1 = true;
            } else {
              for (i = 0; i < 6; i++) {
                alpha1[i].re = rtNaN;
                alpha1[i].im = 0.0;
                beta1[i].re = rtNaN;
                beta1[i].im = 0.0;
              }
              for (b_i = 0; b_i < 36; b_i++) {
                Z[b_i].re = rtNaN;
                Z[b_i].im = 0.0;
              }
              *info = 1;
              exitg1 = 1;
            }
          }
        }
        if (b_guard1) {
          if (goto60) {
            goto60 = false;
            alpha1[ilast] = A[ilast + 6 * ilast];
            ilast = ilastm1;
            ilastm1--;
            if (ilast + 1 < ilo) {
              failed = false;
              guard2 = true;
              exitg1 = 1;
            } else {
              iiter = 0;
              eshift_re = 0.0;
              eshift_im = 0.0;
              jiter++;
            }
          } else {
            if (goto70) {
              goto70 = false;
              iiter++;
              if (iiter - iiter / 10 * 10 != 0) {
                i = ilastm1 + 6 * ilastm1;
                anorm = ascale * A[i].re;
                t = ascale * A[i].im;
                if (t == 0.0) {
                  shift.re = anorm / 0.40824829046386307;
                  shift.im = 0.0;
                } else if (anorm == 0.0) {
                  shift.re = 0.0;
                  shift.im = t / 0.40824829046386307;
                } else {
                  shift.re = anorm / 0.40824829046386307;
                  shift.im = t / 0.40824829046386307;
                }
                i = ilast + 6 * ilast;
                anorm = ascale * A[i].re;
                t = ascale * A[i].im;
                if (t == 0.0) {
                  ad22_re = anorm / 0.40824829046386307;
                  ad22_im = 0.0;
                } else if (anorm == 0.0) {
                  ad22_re = 0.0;
                  ad22_im = t / 0.40824829046386307;
                } else {
                  ad22_re = anorm / 0.40824829046386307;
                  ad22_im = t / 0.40824829046386307;
                }
                t1_re = 0.5 * (shift.re + ad22_re);
                t1_im = 0.5 * (shift.im + ad22_im);
                t1_im_tmp = t1_re * t1_im;
                i = ilastm1 + 6 * ilast;
                anorm = ascale * A[i].re;
                t = ascale * A[i].im;
                if (t == 0.0) {
                  colscale = anorm / 0.40824829046386307;
                  colssq = 0.0;
                } else if (anorm == 0.0) {
                  colscale = 0.0;
                  colssq = t / 0.40824829046386307;
                } else {
                  colscale = anorm / 0.40824829046386307;
                  colssq = t / 0.40824829046386307;
                }
                i = ilast + 6 * ilastm1;
                anorm = ascale * A[i].re;
                t = ascale * A[i].im;
                if (t == 0.0) {
                  ssq = anorm / 0.40824829046386307;
                  anorm = 0.0;
                } else if (anorm == 0.0) {
                  ssq = 0.0;
                  anorm = t / 0.40824829046386307;
                } else {
                  ssq = anorm / 0.40824829046386307;
                  anorm = t / 0.40824829046386307;
                }
                t = shift.re * ad22_re - shift.im * ad22_im;
                scale = shift.re * ad22_im + shift.im * ad22_re;
                shift.re = ((t1_re * t1_re - t1_im * t1_im) +
                            (colscale * ssq - colssq * anorm)) -
                           t;
                shift.im = ((t1_im_tmp + t1_im_tmp) +
                            (colscale * anorm + colssq * ssq)) -
                           scale;
                b_sqrt(&shift);
                if ((t1_re - ad22_re) * shift.re +
                        (t1_im - ad22_im) * shift.im <=
                    0.0) {
                  shift.re += t1_re;
                  shift.im += t1_im;
                } else {
                  shift.re = t1_re - shift.re;
                  shift.im = t1_im - shift.im;
                }
              } else {
                i = ilast + 6 * ilastm1;
                anorm = ascale * A[i].re;
                t = ascale * A[i].im;
                if (t == 0.0) {
                  colscale = anorm / 0.40824829046386307;
                  colssq = 0.0;
                } else if (anorm == 0.0) {
                  colscale = 0.0;
                  colssq = t / 0.40824829046386307;
                } else {
                  colscale = anorm / 0.40824829046386307;
                  colssq = t / 0.40824829046386307;
                }
                eshift_re += colscale;
                eshift_im += colssq;
                shift.re = eshift_re;
                shift.im = eshift_im;
              }
              j = ilastm1;
              i = ilastm1 + 1;
              exitg2 = false;
              while ((!exitg2) && (j + 1 > ifirst)) {
                istart = j + 1;
                nm1 = j + 6 * j;
                ctemp.re = ascale * A[nm1].re - shift.re * 0.40824829046386307;
                ctemp.im = ascale * A[nm1].im - shift.im * 0.40824829046386307;
                anorm = fabs(ctemp.re) + fabs(ctemp.im);
                i += 6 * j;
                t = ascale * (fabs(A[i].re) + fabs(A[i].im));
                scale = anorm;
                if (t > anorm) {
                  scale = t;
                }
                if ((scale < 1.0) && (scale != 0.0)) {
                  anorm /= scale;
                  t /= scale;
                }
                b_i = j + 6 * (j - 1);
                if ((fabs(A[b_i].re) + fabs(A[b_i].im)) * t <= anorm * b_atol) {
                  goto90 = true;
                  exitg2 = true;
                } else {
                  i = j;
                  j--;
                }
              }
              if (!goto90) {
                istart = ifirst;
                nm1 = (ifirst + 6 * (ifirst - 1)) - 1;
                ctemp.re = ascale * A[nm1].re - shift.re * 0.40824829046386307;
                ctemp.im = ascale * A[nm1].im - shift.im * 0.40824829046386307;
              }
              goto90 = false;
              i = istart + 6 * (istart - 1);
              b_ascale.re = ascale * A[i].re;
              b_ascale.im = ascale * A[i].im;
              b_xzlartg(ctemp, b_ascale, &anorm, &shift);
              j = istart;
              nm1 = istart - 2;
              while (j < ilast + 1) {
                if (j > istart) {
                  col = j + 6 * nm1;
                  xzlartg(A[col - 1], A[col], &anorm, &shift,
                          &A[(j + 6 * nm1) - 1]);
                  A[col].re = 0.0;
                  A[col].im = 0.0;
                }
                for (nm1 = j; nm1 < 7; nm1++) {
                  row = j + 6 * (nm1 - 1);
                  t = A[row].im;
                  scale = A[row].re;
                  ssq = A[row - 1].re;
                  colscale = A[row - 1].im;
                  A[row].re =
                      anorm * scale - (shift.re * ssq + shift.im * colscale);
                  A[row].im = anorm * A[row].im -
                              (shift.re * colscale - shift.im * ssq);
                  A[row - 1].re =
                      anorm * ssq + (shift.re * scale - shift.im * t);
                  A[row - 1].im =
                      anorm * colscale + (shift.re * t + shift.im * scale);
                }
                shift.re = -shift.re;
                shift.im = -shift.im;
                nm1 = j;
                if (ilast + 1 < j + 2) {
                  nm1 = ilast - 1;
                }
                for (i = 1; i <= nm1 + 2; i++) {
                  row = (i + 6 * (j - 1)) - 1;
                  t = A[row].im;
                  scale = A[row].re;
                  col = (i + 6 * j) - 1;
                  ssq = A[col].re;
                  colscale = A[col].im;
                  A[row].re =
                      anorm * scale - (shift.re * ssq + shift.im * colscale);
                  A[row].im = anorm * A[row].im -
                              (shift.re * colscale - shift.im * ssq);
                  A[col].re = anorm * ssq + (shift.re * scale - shift.im * t);
                  A[col].im =
                      anorm * colscale + (shift.re * t + shift.im * scale);
                }
                for (i = 0; i < 6; i++) {
                  row = i + 6 * (j - 1);
                  t = Z[row].im;
                  scale = Z[row].re;
                  col = i + 6 * j;
                  ssq = Z[col].re;
                  colscale = Z[col].im;
                  Z[row].re =
                      anorm * scale - (shift.re * ssq + shift.im * colscale);
                  Z[row].im = anorm * Z[row].im -
                              (shift.re * colscale - shift.im * ssq);
                  Z[col].re = anorm * ssq + (shift.re * scale - shift.im * t);
                  Z[col].im =
                      anorm * colscale + (shift.re * t + shift.im * scale);
                }
                nm1 = j - 1;
                j++;
              }
            }
            jiter++;
          }
        }
      } else {
        guard2 = true;
        exitg1 = 1;
      }
    } while (exitg1 == 0);
  } else {
    guard1 = true;
  }
  if (guard2) {
    if (failed) {
      *info = ilast + 1;
      for (i = 0; i <= ilast; i++) {
        alpha1[i].re = rtNaN;
        alpha1[i].im = 0.0;
        beta1[i].re = rtNaN;
        beta1[i].im = 0.0;
      }
      for (b_i = 0; b_i < 36; b_i++) {
        Z[b_i].re = rtNaN;
        Z[b_i].im = 0.0;
      }
    } else {
      guard1 = true;
    }
  }
  if (guard1) {
    for (j = 0; j <= ilo - 2; j++) {
      alpha1[j] = A[j + 6 * j];
    }
  }
}

/*
 * File trailer for xzhgeqz.c
 *
 * [EOF]
 */
