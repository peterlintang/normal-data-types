/*
 * File: ellipsoid_fit.c
 *
 * MATLAB Coder version            : 5.3
 * C/C++ source code generated on  : 02-Aug-2024 17:35:13
 */

/* Include Files */
#include "ellipsoid_fit.h"
#include "eig.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

/* Function Definitions */
/*
 * Ellipsoid fitting algorithm
 *
 *  Inputs:
 *    x = nx1 column vector of x coordinates of input data.
 *    y = nx1 column vector of y coordinates of input data.
 *    z = nx1 column vector of y coordinates of input data.
 *
 *  Output:
 *    u = [a,b,c,f,g,h,p,q,r,d], a vector corresponding to coefficients of
 *        the general quadric surface given by equation,
 *        ax2 + by2 + cz2 + 2fyz + 2gxz + 2hxy + 2px + 2qy + 2rz + d = 0.
 *
 *  Source:
 *    [1] Li - Least Square Ellipsoid Fitting (2004)
 *
 *  2020/06/03
 *
 * Arguments    : const double x[1486]
 *                const double y[1486]
 *                const double z[1486]
 *                creal_T u[10]
 * Return Type  : void
 */
void ellipsoid_fit(const double x[1486], const double y[1486],
                   const double z[1486], creal_T u[10])
{
  static const signed char c_A[36] = {-1, 1, 1,  0, 0,  0, 1, -1, 1, 0,  0, 0,
                                      1,  1, -1, 0, 0,  0, 0, 0,  0, -4, 0, 0,
                                      0,  0, 0,  0, -4, 0, 0, 0,  0, 0,  0, -4};
  creal_T eval[36];
  creal_T evec[36];
  creal_T c_b_tmp[24];
  creal_T d_b_tmp[4];
  double D[14860];
  double S[100];
  double M[36];
  double b_A[36];
  double b_tmp[24];
  double A[16];
  double maxValues[6];
  double b_b_tmp_re_tmp;
  double b_tmp_re_tmp;
  double c_b_tmp_re_tmp;
  double re;
  double s;
  double smax;
  int A_tmp;
  int b_b_tmp;
  int b_i;
  int i;
  int i1;
  int j;
  int jA;
  int jBcol;
  int jp1j;
  int k;
  int mmj_tmp;
  signed char b_ipiv[6];
  signed char ipiv[4];
  signed char i2;
  bool exitg1;
  /*  Design matrix */
  for (k = 0; k < 1486; k++) {
    b_tmp_re_tmp = x[k];
    D[10 * k] = b_tmp_re_tmp * b_tmp_re_tmp;
    smax = y[k];
    D[10 * k + 1] = smax * smax;
    s = z[k];
    D[10 * k + 2] = s * s;
    D[10 * k + 3] = 2.0 * smax * s;
    D[10 * k + 4] = 2.0 * b_tmp_re_tmp * s;
    D[10 * k + 5] = 2.0 * b_tmp_re_tmp * smax;
    D[10 * k + 6] = 2.0 * b_tmp_re_tmp;
    D[10 * k + 7] = 2.0 * smax;
    D[10 * k + 8] = 2.0 * s;
    D[10 * k + 9] = 1.0;
  }
  printf("\n\nD =\n");
  fflush(stdout);
  /*  Constraint kJ > I^2 */
  /*  Ellipsoid if k = 4  */
  /*  Eqn(7) */
  printf("\n\nC1 =\n");
  fflush(stdout);
  /*  Eqn(11) */
  for (i = 0; i < 10; i++) {
    for (i1 = 0; i1 < 10; i1++) {
      b_tmp_re_tmp = 0.0;
      for (jA = 0; jA < 1486; jA++) {
        b_tmp_re_tmp += D[i + 10 * jA] * D[i1 + 10 * jA];
      }
      S[i + 10 * i1] = b_tmp_re_tmp;
    }
  }
  /*  6X6 */
  /*  6X4 */
  /*  4X6 */
  /*  4X4 */
  printf("\n\n S11 S12 S21 S22=\n");
  fflush(stdout);
  /*  Eqn(14) and Eqn(15) */
  /*  eval = Diagonal matrix of eigenvalues */
  /*  evec = Matix containing corresponding eigenvectors in its column */
  for (i = 0; i < 4; i++) {
    A_tmp = 10 * (i + 6);
    jA = i << 2;
    A[jA] = S[A_tmp + 6];
    A[jA + 1] = S[A_tmp + 7];
    A[jA + 2] = S[A_tmp + 8];
    A[jA + 3] = S[A_tmp + 9];
  }
  for (i = 0; i < 6; i++) {
    A_tmp = i << 2;
    b_tmp[A_tmp] = S[10 * i + 6];
    b_tmp[A_tmp + 1] = S[10 * i + 7];
    b_tmp[A_tmp + 2] = S[10 * i + 8];
    b_tmp[A_tmp + 3] = S[10 * i + 9];
  }
  ipiv[0] = 1;
  ipiv[1] = 2;
  ipiv[2] = 3;
  ipiv[3] = 4;
  for (j = 0; j < 3; j++) {
    mmj_tmp = 2 - j;
    b_b_tmp = j * 5;
    jp1j = b_b_tmp + 2;
    jA = 4 - j;
    jBcol = 0;
    smax = fabs(A[b_b_tmp]);
    for (k = 2; k <= jA; k++) {
      s = fabs(A[(b_b_tmp + k) - 1]);
      if (s > smax) {
        jBcol = k - 1;
        smax = s;
      }
    }
    if (A[b_b_tmp + jBcol] != 0.0) {
      if (jBcol != 0) {
        jA = j + jBcol;
        ipiv[j] = (signed char)(jA + 1);
        smax = A[j];
        A[j] = A[jA];
        A[jA] = smax;
        smax = A[j + 4];
        A[j + 4] = A[jA + 4];
        A[jA + 4] = smax;
        smax = A[j + 8];
        A[j + 8] = A[jA + 8];
        A[jA + 8] = smax;
        smax = A[j + 12];
        A[j + 12] = A[jA + 12];
        A[jA + 12] = smax;
      }
      i = (b_b_tmp - j) + 4;
      for (b_i = jp1j; b_i <= i; b_i++) {
        A[b_i - 1] /= A[b_b_tmp];
      }
    }
    jA = b_b_tmp;
    for (jp1j = 0; jp1j <= mmj_tmp; jp1j++) {
      smax = A[(b_b_tmp + (jp1j << 2)) + 4];
      if (smax != 0.0) {
        i = jA + 6;
        i1 = (jA - j) + 8;
        for (A_tmp = i; A_tmp <= i1; A_tmp++) {
          A[A_tmp - 1] += A[((b_b_tmp + A_tmp) - jA) - 5] * -smax;
        }
      }
      jA += 4;
    }
    i2 = ipiv[j];
    if (i2 != j + 1) {
      for (jp1j = 0; jp1j < 6; jp1j++) {
        b_i = jp1j << 2;
        jBcol = j + b_i;
        smax = b_tmp[jBcol];
        A_tmp = (i2 + b_i) - 1;
        b_tmp[jBcol] = b_tmp[A_tmp];
        b_tmp[A_tmp] = smax;
      }
    }
  }
  for (j = 0; j < 6; j++) {
    jBcol = j << 2;
    for (k = 0; k < 4; k++) {
      jA = k << 2;
      i = k + jBcol;
      if (b_tmp[i] != 0.0) {
        i1 = k + 2;
        for (b_i = i1; b_i < 5; b_i++) {
          A_tmp = (b_i + jBcol) - 1;
          b_tmp[A_tmp] -= b_tmp[i] * A[(b_i + jA) - 1];
        }
      }
    }
  }
  for (j = 0; j < 6; j++) {
    jBcol = j << 2;
    for (k = 3; k >= 0; k--) {
      jA = k << 2;
      i = k + jBcol;
      b_tmp_re_tmp = b_tmp[i];
      if (b_tmp_re_tmp != 0.0) {
        b_tmp[i] = b_tmp_re_tmp / A[k + jA];
        for (b_i = 0; b_i < k; b_i++) {
          A_tmp = b_i + jBcol;
          b_tmp[A_tmp] -= b_tmp[i] * A[b_i + jA];
        }
      }
    }
  }
  for (i = 0; i < 6; i++) {
    for (i1 = 0; i1 < 6; i1++) {
      jA = i1 << 2;
      M[i + 6 * i1] = S[i + 10 * i1] -
                      (((S[i + 60] * b_tmp[jA] + S[i + 70] * b_tmp[jA + 1]) +
                        S[i + 80] * b_tmp[jA + 2]) +
                       S[i + 90] * b_tmp[jA + 3]);
    }
  }
  for (i = 0; i < 36; i++) {
    b_A[i] = c_A[i];
  }
  for (i = 0; i < 6; i++) {
    b_ipiv[i] = (signed char)(i + 1);
  }
  for (j = 0; j < 5; j++) {
    mmj_tmp = 4 - j;
    b_b_tmp = j * 7;
    jp1j = b_b_tmp + 2;
    jA = 6 - j;
    jBcol = 0;
    smax = fabs(b_A[b_b_tmp]);
    for (k = 2; k <= jA; k++) {
      s = fabs(b_A[(b_b_tmp + k) - 1]);
      if (s > smax) {
        jBcol = k - 1;
        smax = s;
      }
    }
    if (b_A[b_b_tmp + jBcol] != 0.0) {
      if (jBcol != 0) {
        jA = j + jBcol;
        b_ipiv[j] = (signed char)(jA + 1);
        for (k = 0; k < 6; k++) {
          b_i = j + k * 6;
          smax = b_A[b_i];
          A_tmp = jA + k * 6;
          b_A[b_i] = b_A[A_tmp];
          b_A[A_tmp] = smax;
        }
      }
      i = (b_b_tmp - j) + 6;
      for (b_i = jp1j; b_i <= i; b_i++) {
        b_A[b_i - 1] /= b_A[b_b_tmp];
      }
    }
    jA = b_b_tmp;
    for (jp1j = 0; jp1j <= mmj_tmp; jp1j++) {
      smax = b_A[(b_b_tmp + jp1j * 6) + 6];
      if (smax != 0.0) {
        i = jA + 8;
        i1 = (jA - j) + 12;
        for (A_tmp = i; A_tmp <= i1; A_tmp++) {
          b_A[A_tmp - 1] += b_A[((b_b_tmp + A_tmp) - jA) - 7] * -smax;
        }
      }
      jA += 6;
    }
    i2 = b_ipiv[j];
    if (i2 != j + 1) {
      for (jp1j = 0; jp1j < 6; jp1j++) {
        b_i = j + 6 * jp1j;
        smax = M[b_i];
        A_tmp = (i2 + 6 * jp1j) - 1;
        M[b_i] = M[A_tmp];
        M[A_tmp] = smax;
      }
    }
  }
  for (j = 0; j < 6; j++) {
    jBcol = 6 * j;
    for (k = 0; k < 6; k++) {
      jA = 6 * k;
      i = k + jBcol;
      if (M[i] != 0.0) {
        i1 = k + 2;
        for (b_i = i1; b_i < 7; b_i++) {
          A_tmp = (b_i + jBcol) - 1;
          M[A_tmp] -= M[i] * b_A[(b_i + jA) - 1];
        }
      }
    }
  }
  for (j = 0; j < 6; j++) {
    jBcol = 6 * j;
    for (k = 5; k >= 0; k--) {
      jA = 6 * k;
      i = k + jBcol;
      b_tmp_re_tmp = M[i];
      if (b_tmp_re_tmp != 0.0) {
        M[i] = b_tmp_re_tmp / b_A[k + jA];
        for (b_i = 0; b_i < k; b_i++) {
          A_tmp = b_i + jBcol;
          M[A_tmp] -= M[i] * b_A[b_i + jA];
        }
      }
    }
  }
  printf("\n\nM =\n");
  fflush(stdout);
  eig(M, evec, eval);
  printf("\n\n evec,eval =\n");
  fflush(stdout);
  /*  Locate the index of the 'only' positive eigenvalue. */
  /*  If M is singular, then u1 is eigenvector of largest eigen-value. */
  /* [max_element_row,mmm]     = my_max(eval);  */
  /*  获取数组的行数和列数 */
  /*  初始化存储每列最大值的数组 */
  for (jA = 0; jA < 6; jA++) {
    smax = eval[6 * jA].re;
    /*  假设第一行的值为最大值 */
    for (jBcol = 0; jBcol < 5; jBcol++) {
      re = eval[(jBcol + 6 * jA) + 1].re;
      if (re > smax) {
        smax = re;
        /*  更新当前列的最大值 */
      }
    }
    maxValues[jA] = smax;
    /*  存储最大值 */
  }
  if (!rtIsNaN(maxValues[0])) {
    A_tmp = 1;
  } else {
    A_tmp = 0;
    k = 2;
    exitg1 = false;
    while ((!exitg1) && (k < 7)) {
      if (!rtIsNaN(maxValues[k - 1])) {
        A_tmp = k;
        exitg1 = true;
      } else {
        k++;
      }
    }
  }
  if (A_tmp == 0) {
    A_tmp = 1;
  } else {
    smax = maxValues[A_tmp - 1];
    i = A_tmp + 1;
    for (k = i; k < 7; k++) {
      b_tmp_re_tmp = maxValues[k - 1];
      if (smax < b_tmp_re_tmp) {
        smax = b_tmp_re_tmp;
        A_tmp = k;
      }
    }
  }
  printf("\n\n max_element_row  =\n");
  fflush(stdout);
  printf("\n\n max_column_index =\n");
  fflush(stdout);
  printf("\n\n mmm=\n");
  fflush(stdout);
  printf("\n\n mmmm=\n");
  fflush(stdout);
  /*  Eigenvector for  positive eigenvalue. */
  printf("\n\n u1 =\n");
  fflush(stdout);
  printf("\n\n u2=\n");
  fflush(stdout);
  for (i = 0; i < 24; i++) {
    c_b_tmp[i].re = -b_tmp[i];
    c_b_tmp[i].im = 0.0;
  }
  for (i = 0; i < 4; i++) {
    re = 0.0;
    smax = 0.0;
    for (i1 = 0; i1 < 6; i1++) {
      jA = i + (i1 << 2);
      jBcol = i1 + 6 * (A_tmp - 1);
      s = c_b_tmp[jA].re;
      b_tmp_re_tmp = evec[jBcol].im;
      b_b_tmp_re_tmp = c_b_tmp[jA].im;
      c_b_tmp_re_tmp = evec[jBcol].re;
      re += s * c_b_tmp_re_tmp - b_b_tmp_re_tmp * b_tmp_re_tmp;
      smax += s * b_tmp_re_tmp + b_b_tmp_re_tmp * c_b_tmp_re_tmp;
    }
    d_b_tmp[i].re = re;
    d_b_tmp[i].im = smax;
  }
  memcpy(&u[0], &evec[A_tmp * 6 + -6], 6U * sizeof(creal_T));
  memcpy(&u[6], &d_b_tmp[0], 4U * sizeof(creal_T));
  printf("\n\n u=\n");
  fflush(stdout);
}

/*
 * File trailer for ellipsoid_fit.c
 *
 * [EOF]
 */
