/*
 * File: mldivide.c
 *
 * MATLAB Coder version            : 5.3
 * C/C++ source code generated on  : 02-Aug-2024 10:19:14
 */

/* Include Files */
#include "mldivide.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
/*
 * Arguments    : double B[36]
 * Return Type  : void
 */
void mldivide(double B[36])
{
  static const signed char b_A[36] = {-1, 1, 1,  0, 0,  0, 1, -1, 1, 0,  0, 0,
                                      1,  1, -1, 0, 0,  0, 0, 0,  0, -4, 0, 0,
                                      0,  0, 0,  0, -4, 0, 0, 0,  0, 0,  0, -4};
  double A[36];
  double s;
  double smax;
  int b_i;
  int b_tmp;
  int i;
  int i1;
  int j;
  int jA;
  int jBcol;
  int jp1j;
  int k;
  int mmj_tmp;
  int temp_tmp;
  signed char ipiv[6];
  signed char i2;
  for (i = 0; i < 36; i++) {
    A[i] = b_A[i];
  }
  for (i = 0; i < 6; i++) {
    ipiv[i] = (signed char)(i + 1);
  }
  for (j = 0; j < 5; j++) {
    mmj_tmp = 4 - j;
    b_tmp = j * 7;
    jp1j = b_tmp + 2;
    jA = 6 - j;
    jBcol = 0;
    smax = fabs(A[b_tmp]);
    for (k = 2; k <= jA; k++) {
      s = fabs(A[(b_tmp + k) - 1]);
      if (s > smax) {
        jBcol = k - 1;
        smax = s;
      }
    }
    if (A[b_tmp + jBcol] != 0.0) {
      if (jBcol != 0) {
        jA = j + jBcol;
        ipiv[j] = (signed char)(jA + 1);
        for (k = 0; k < 6; k++) {
          temp_tmp = j + k * 6;
          smax = A[temp_tmp];
          jBcol = jA + k * 6;
          A[temp_tmp] = A[jBcol];
          A[jBcol] = smax;
        }
      }
      i = (b_tmp - j) + 6;
      for (b_i = jp1j; b_i <= i; b_i++) {
        A[b_i - 1] /= A[b_tmp];
      }
    }
    jA = b_tmp;
    for (jp1j = 0; jp1j <= mmj_tmp; jp1j++) {
      smax = A[(b_tmp + jp1j * 6) + 6];
      if (smax != 0.0) {
        i = jA + 8;
        i1 = (jA - j) + 12;
        for (jBcol = i; jBcol <= i1; jBcol++) {
          A[jBcol - 1] += A[((b_tmp + jBcol) - jA) - 7] * -smax;
        }
      }
      jA += 6;
    }
    i2 = ipiv[j];
    if (i2 != j + 1) {
      for (jp1j = 0; jp1j < 6; jp1j++) {
        temp_tmp = j + 6 * jp1j;
        smax = B[temp_tmp];
        i = (i2 + 6 * jp1j) - 1;
        B[temp_tmp] = B[i];
        B[i] = smax;
      }
    }
  }
  for (j = 0; j < 6; j++) {
    jBcol = 6 * j;
    for (k = 0; k < 6; k++) {
      jA = 6 * k;
      i = k + jBcol;
      if (B[i] != 0.0) {
        i1 = k + 2;
        for (b_i = i1; b_i < 7; b_i++) {
          temp_tmp = (b_i + jBcol) - 1;
          B[temp_tmp] -= B[i] * A[(b_i + jA) - 1];
        }
      }
    }
  }
  for (j = 0; j < 6; j++) {
    jBcol = 6 * j;
    for (k = 5; k >= 0; k--) {
      jA = 6 * k;
      i = k + jBcol;
      smax = B[i];
      if (smax != 0.0) {
        B[i] = smax / A[k + jA];
        for (b_i = 0; b_i < k; b_i++) {
          i1 = b_i + jBcol;
          B[i1] -= B[i] * A[b_i + jA];
        }
      }
    }
  }
}

/*
 * File trailer for mldivide.c
 *
 * [EOF]
 */
