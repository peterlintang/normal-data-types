/*
 * File: ellipsoid_fit_test.c
 *
 * MATLAB Coder version            : 5.3
 * C/C++ source code generated on  : 13-Aug-2024 17:16:40
 */

/* Include Files */
#include "ellipsoid_fit_test.h"
#include "eig.h"
#include "ellipsoid_fit.h"
#include "ellipsoid_fit_test_emxutil.h"
#include "ellipsoid_fit_test_types.h"
#include "mldivide.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

/* Function Definitions */
/*
 * 指定输入和输出的数据类型
 *  使用 matlab.unittest.constraints 中的相关约束
 * inputs = {
 *     matlab.inputPort(double, "x_m", true),
 *     matlab.inputPort(double, "y_m", true)
 *     matlab.inputPort(double, "z_m", true)
 * };
 * outputs = {
 *     matlab.outputPort(double, "v", true)
 *     matlab.outputPort(double, "offset", true)
 *     matlab.outputPort(double, "gain", true)
 *     matlab.outputPort(double, "matrix", true)
 * };
 *  Import sensor readings
 * v=double(zeros(10,1));
 * offset = double(zeros(3,1));
 * gain = double(zeros(3,3));
 * matrix = double(zeros(3,3));
 *
 * Arguments    : const emxArray_real_T *x_m
 *                const emxArray_real_T *y_m
 *                const emxArray_real_T *z_m
 *                double v[10]
 *                double offset[3]
 *                double gain[9]
 *                double matrix[9]
 * Return Type  : void
 */
void ellipsoid_fit_test(const emxArray_real_T *x_m, const emxArray_real_T *y_m,
                        const emxArray_real_T *z_m, double v[10],
                        double offset[3], double gain[9], double matrix[9])
{
  emxArray_real_T *D;
  emxArray_real_T *b_y;
  emxArray_real_T *r;
  emxArray_real_T *r2;
  emxArray_real_T *r4;
  emxArray_real_T *y;
  creal_T eval[36];
  creal_T evec[36];
  creal_T b_eval[9];
  creal_T b_evec[9];
  double S[100];
  double x[36];
  double b_tmp[24];
  double A[16];
  double M_[9];
  double c_evec[9];
  double d_evec[9];
  double ex[6];
  double c_b_tmp[4];
  double B[3];
  double b_v[3];
  double pqr_[3];
  const double *x_m_data;
  const double *y_m_data;
  const double *z_m_data;
  double b_gain_tmp;
  double bkj;
  double c_gain_tmp;
  double d_gain_tmp;
  double gain_tmp;
  double s;
  double *D_data;
  double *b_y_data;
  double *r1;
  double *r3;
  double *r5;
  double *y_data;
  int aoffset;
  int b_b_tmp;
  int b_i;
  int coffset;
  int i;
  int i1;
  int j;
  int jA;
  int jBcol;
  int k;
  signed char ipiv[4];
  signed char i2;
  bool exitg1;
  bool p;
  z_m_data = z_m->data;
  y_m_data = y_m->data;
  x_m_data = x_m->data;
  emxInit_real_T(&y, 1);
  /* inputs = { */
  /*     matlab.inputPort(double, "x", true), */
  /*     matlab.inputPort(double, "y", true) */
  /*     matlab.inputPort(double, "z", true) */
  /* }; */
  /* outputs = { */
  /*     matlab.outputPort(double, "u", true) */
  /* }; */
  /*  Ellipsoid fitting algorithm */
  /*  */
  /*  Inputs: */
  /*    x = nx1 column vector of x coordinates of input data. */
  /*    y = nx1 column vector of y coordinates of input data. */
  /*    z = nx1 column vector of y coordinates of input data. */
  /*  */
  /*  Output: */
  /*    u = [a,b,c,f,g,h,p,q,r,d], a vector corresponding to coefficients of  */
  /*        the general quadric surface given by equation,   */
  /*        ax2 + by2 + cz2 + 2fyz + 2gxz + 2hxy + 2px + 2qy + 2rz + d = 0. */
  /*       */
  /*  Source: */
  /*    [1] Li - Least Square Ellipsoid Fitting (2004) */
  /*     */
  /*  2020/06/03 */
  /* u=double(zeros(10,1)); */
  /*  Design matrix */
  i = y->size[0];
  y->size[0] = y_m->size[0];
  emxEnsureCapacity_real_T(y, i);
  y_data = y->data;
  jA = y_m->size[0];
  for (i = 0; i < jA; i++) {
    y_data[i] = 2.0 * y_m_data[i];
  }
  emxInit_real_T(&b_y, 1);
  i = b_y->size[0];
  b_y->size[0] = x_m->size[0];
  emxEnsureCapacity_real_T(b_y, i);
  b_y_data = b_y->data;
  jA = x_m->size[0];
  for (i = 0; i < jA; i++) {
    b_y_data[i] = 2.0 * x_m_data[i];
  }
  emxInit_real_T(&r, 1);
  i = r->size[0];
  r->size[0] = x_m->size[0];
  emxEnsureCapacity_real_T(r, i);
  r1 = r->data;
  jA = x_m->size[0];
  for (i = 0; i < jA; i++) {
    bkj = x_m_data[i];
    r1[i] = bkj * bkj;
  }
  emxInit_real_T(&r2, 1);
  i = r2->size[0];
  r2->size[0] = y_m->size[0];
  emxEnsureCapacity_real_T(r2, i);
  r3 = r2->data;
  jA = y_m->size[0];
  for (i = 0; i < jA; i++) {
    bkj = y_m_data[i];
    r3[i] = bkj * bkj;
  }
  emxInit_real_T(&r4, 1);
  i = r4->size[0];
  r4->size[0] = z_m->size[0];
  emxEnsureCapacity_real_T(r4, i);
  r5 = r4->data;
  jA = z_m->size[0];
  for (i = 0; i < jA; i++) {
    bkj = z_m_data[i];
    r5[i] = bkj * bkj;
  }
  emxInit_real_T(&D, 2);
  if ((y->size[0] == z_m->size[0]) && (b_y->size[0] == z_m->size[0]) &&
      (b_y->size[0] == y_m->size[0])) {
    i = D->size[0] * D->size[1];
    D->size[0] = 10;
    D->size[1] = r->size[0];
    emxEnsureCapacity_real_T(D, i);
    D_data = D->data;
    jA = r->size[0];
    for (i = 0; i < jA; i++) {
      D_data[10 * i] = r1[i];
    }
    jA = r2->size[0];
    for (i = 0; i < jA; i++) {
      D_data[10 * i + 1] = r3[i];
    }
    jA = r4->size[0];
    for (i = 0; i < jA; i++) {
      D_data[10 * i + 2] = r5[i];
    }
    jA = y->size[0];
    for (i = 0; i < jA; i++) {
      D_data[10 * i + 3] = y_data[i] * z_m_data[i];
    }
    jA = b_y->size[0];
    for (i = 0; i < jA; i++) {
      D_data[10 * i + 4] = b_y_data[i] * z_m_data[i];
    }
    jA = b_y->size[0];
    for (i = 0; i < jA; i++) {
      D_data[10 * i + 5] = b_y_data[i] * y_m_data[i];
    }
    jA = b_y->size[0];
    for (i = 0; i < jA; i++) {
      D_data[10 * i + 6] = b_y_data[i];
    }
    jA = y->size[0];
    for (i = 0; i < jA; i++) {
      D_data[10 * i + 7] = y_data[i];
    }
    jA = z_m->size[0];
    for (i = 0; i < jA; i++) {
      D_data[10 * i + 8] = 2.0 * z_m_data[i];
    }
    jA = x_m->size[0];
    for (i = 0; i < jA; i++) {
      D_data[10 * i + 9] = 1.0;
    }
  } else {
    binary_expand_op(D, r, r2, r4, y, z_m, b_y, y_m, x_m);
    D_data = D->data;
  }
  emxFree_real_T(&r4);
  emxFree_real_T(&r2);
  emxFree_real_T(&r);
  emxFree_real_T(&y);
  emxFree_real_T(&b_y);
  printf("\n\nD =\n");
  fflush(stdout);
  /*  Constraint kJ > I^2 */
  /*  Ellipsoid if k = 4  */
  /*  Eqn(7) */
  printf("\n\nC1 =\n");
  fflush(stdout);
  /*  Eqn(11) */
  jA = D->size[1];
  for (j = 0; j < 10; j++) {
    coffset = j * 10;
    memset(&S[coffset], 0, 10U * sizeof(double));
    for (k = 0; k < jA; k++) {
      aoffset = k * 10;
      bkj = D_data[k * 10 + j];
      for (b_i = 0; b_i < 10; b_i++) {
        jBcol = coffset + b_i;
        S[jBcol] += D_data[aoffset + b_i] * bkj;
      }
    }
  }
  emxFree_real_T(&D);
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
    jA = 10 * (i + 6);
    coffset = i << 2;
    A[coffset] = S[jA + 6];
    A[coffset + 1] = S[jA + 7];
    A[coffset + 2] = S[jA + 8];
    A[coffset + 3] = S[jA + 9];
  }
  for (i = 0; i < 6; i++) {
    aoffset = i << 2;
    b_tmp[aoffset] = S[10 * i + 6];
    b_tmp[aoffset + 1] = S[10 * i + 7];
    b_tmp[aoffset + 2] = S[10 * i + 8];
    b_tmp[aoffset + 3] = S[10 * i + 9];
  }
  ipiv[0] = 1;
  ipiv[1] = 2;
  ipiv[2] = 3;
  ipiv[3] = 4;
  for (j = 0; j < 3; j++) {
    aoffset = 2 - j;
    b_b_tmp = j * 5;
    coffset = b_b_tmp + 2;
    jA = 4 - j;
    jBcol = 0;
    bkj = fabs(A[b_b_tmp]);
    for (k = 2; k <= jA; k++) {
      s = fabs(A[(b_b_tmp + k) - 1]);
      if (s > bkj) {
        jBcol = k - 1;
        bkj = s;
      }
    }
    if (A[b_b_tmp + jBcol] != 0.0) {
      if (jBcol != 0) {
        jA = j + jBcol;
        ipiv[j] = (signed char)(jA + 1);
        bkj = A[j];
        A[j] = A[jA];
        A[jA] = bkj;
        bkj = A[j + 4];
        A[j + 4] = A[jA + 4];
        A[jA + 4] = bkj;
        bkj = A[j + 8];
        A[j + 8] = A[jA + 8];
        A[jA + 8] = bkj;
        bkj = A[j + 12];
        A[j + 12] = A[jA + 12];
        A[jA + 12] = bkj;
      }
      i = (b_b_tmp - j) + 4;
      for (b_i = coffset; b_i <= i; b_i++) {
        A[b_i - 1] /= A[b_b_tmp];
      }
    }
    jA = b_b_tmp;
    for (jBcol = 0; jBcol <= aoffset; jBcol++) {
      bkj = A[(b_b_tmp + (jBcol << 2)) + 4];
      if (bkj != 0.0) {
        i = jA + 6;
        i1 = (jA - j) + 8;
        for (coffset = i; coffset <= i1; coffset++) {
          A[coffset - 1] += A[((b_b_tmp + coffset) - jA) - 5] * -bkj;
        }
      }
      jA += 4;
    }
    i2 = ipiv[j];
    if (i2 != j + 1) {
      for (jBcol = 0; jBcol < 6; jBcol++) {
        jA = jBcol << 2;
        coffset = j + jA;
        bkj = b_tmp[coffset];
        aoffset = (i2 + jA) - 1;
        b_tmp[coffset] = b_tmp[aoffset];
        b_tmp[aoffset] = bkj;
      }
    }
  }
  for (j = 0; j < 6; j++) {
    jBcol = j << 2;
    for (k = 0; k < 4; k++) {
      coffset = k << 2;
      i = k + jBcol;
      if (b_tmp[i] != 0.0) {
        i1 = k + 2;
        for (b_i = i1; b_i < 5; b_i++) {
          aoffset = (b_i + jBcol) - 1;
          b_tmp[aoffset] -= b_tmp[i] * A[(b_i + coffset) - 1];
        }
      }
    }
  }
  for (j = 0; j < 6; j++) {
    jBcol = j << 2;
    for (k = 3; k >= 0; k--) {
      coffset = k << 2;
      i = k + jBcol;
      gain_tmp = b_tmp[i];
      if (gain_tmp != 0.0) {
        b_tmp[i] = gain_tmp / A[k + coffset];
        for (b_i = 0; b_i < k; b_i++) {
          aoffset = b_i + jBcol;
          b_tmp[aoffset] -= b_tmp[i] * A[b_i + coffset];
        }
      }
    }
  }
  printf("\n\nM =\n");
  fflush(stdout);
  for (i = 0; i < 6; i++) {
    for (i1 = 0; i1 < 6; i1++) {
      jA = i1 << 2;
      x[i + 6 * i1] = S[i + 10 * i1] -
                      (((S[i + 60] * b_tmp[jA] + S[i + 70] * b_tmp[jA + 1]) +
                        S[i + 80] * b_tmp[jA + 2]) +
                       S[i + 90] * b_tmp[jA + 3]);
    }
  }
  mldivide(x);
  eig(x, evec, eval);
  printf("\n\n evec,eval =\n");
  fflush(stdout);
  /*  Locate the index of the 'only' positive eigenvalue. */
  /*  If M is singular, then u1 is eigenvector of largest eigen-value. */
  for (i = 0; i < 36; i++) {
    x[i] = eval[i].re;
  }
  for (j = 0; j < 6; j++) {
    ex[j] = x[6 * j];
    for (b_i = 0; b_i < 5; b_i++) {
      gain_tmp = x[(b_i + 6 * j) + 1];
      if (rtIsNaN(gain_tmp)) {
        p = false;
      } else if (rtIsNaN(ex[j])) {
        p = true;
      } else {
        p = (ex[j] < gain_tmp);
      }
      if (p) {
        ex[j] = gain_tmp;
      }
    }
  }
  if (!rtIsNaN(ex[0])) {
    jA = 1;
  } else {
    jA = 0;
    k = 2;
    exitg1 = false;
    while ((!exitg1) && (k < 7)) {
      if (!rtIsNaN(ex[k - 1])) {
        jA = k;
        exitg1 = true;
      } else {
        k++;
      }
    }
  }
  if (jA == 0) {
    jA = 1;
  } else {
    bkj = ex[jA - 1];
    i = jA + 1;
    for (k = i; k < 7; k++) {
      gain_tmp = ex[k - 1];
      if (bkj < gain_tmp) {
        bkj = gain_tmp;
        jA = k;
      }
    }
  }
  /*  Eigenvector for  positive eigenvalue. */
  printf("\n\n u1 =\n");
  fflush(stdout);
  printf("\n\n u2=\n");
  fflush(stdout);
  for (i = 0; i < 24; i++) {
    b_tmp[i] = -b_tmp[i];
  }
  for (i = 0; i < 4; i++) {
    gain_tmp = 0.0;
    for (i1 = 0; i1 < 6; i1++) {
      gain_tmp += b_tmp[i + (i1 << 2)] * evec[i1 + 6 * (jA - 1)].re;
    }
    c_b_tmp[i] = gain_tmp;
  }
  for (i = 0; i < 6; i++) {
    v[i] = evec[i + 6 * (jA - 1)].re;
  }
  v[6] = c_b_tmp[0];
  v[7] = c_b_tmp[1];
  v[8] = c_b_tmp[2];
  v[9] = c_b_tmp[3];
  printf("\n\n u=\n");
  fflush(stdout);
  matrix[0] = v[0];
  matrix[3] = v[5];
  matrix[6] = v[4];
  matrix[1] = v[5];
  matrix[4] = v[1];
  matrix[7] = v[3];
  matrix[2] = v[4];
  matrix[5] = v[3];
  matrix[8] = v[2];
  /*  Original ellipsoid matrix  */
  /* k = d; */
  b_eig(matrix, b_evec, b_eval);
  /*  Compute eigenvectors matrix */
  for (i = 0; i < 9; i++) {
    c_evec[i] = b_evec[i].re;
  }
  /*  DCM = eigenvectors matrix */
  /* eval = -eval; */
  /*  Diagonalize M */
  b_v[0] = c_b_tmp[0];
  b_v[1] = c_b_tmp[1];
  b_v[2] = c_b_tmp[2];
  for (i = 0; i < 3; i++) {
    gain_tmp = c_evec[3 * i];
    bkj = c_evec[3 * i + 1];
    s = c_evec[3 * i + 2];
    for (i1 = 0; i1 < 3; i1++) {
      d_evec[i + 3 * i1] =
          (gain_tmp * matrix[3 * i1] + bkj * matrix[3 * i1 + 1]) +
          s * matrix[3 * i1 + 2];
    }
    gain_tmp = d_evec[i];
    bkj = d_evec[i + 3];
    s = d_evec[i + 6];
    b_gain_tmp = 0.0;
    for (i1 = 0; i1 < 3; i1++) {
      M_[i + 3 * i1] = (gain_tmp * c_evec[3 * i1] + bkj * c_evec[3 * i1 + 1]) +
                       s * c_evec[3 * i1 + 2];
      b_gain_tmp += b_v[i1] * c_evec[i1 + 3 * i];
    }
    pqr_[i] = b_gain_tmp;
  }
  for (i = 0; i < 9; i++) {
    c_evec[i] = -matrix[i];
  }
  B[0] = c_b_tmp[0];
  B[1] = c_b_tmp[1];
  B[2] = c_b_tmp[2];
  jA = 0;
  coffset = 1;
  aoffset = 2;
  bkj = fabs(c_evec[0]);
  s = fabs(c_evec[1]);
  if (s > bkj) {
    bkj = s;
    jA = 1;
    coffset = 0;
  }
  if (fabs(c_evec[2]) > bkj) {
    jA = 2;
    coffset = 1;
    aoffset = 0;
  }
  c_evec[coffset] /= c_evec[jA];
  c_evec[aoffset] /= c_evec[jA];
  c_evec[coffset + 3] -= c_evec[coffset] * c_evec[jA + 3];
  c_evec[aoffset + 3] -= c_evec[aoffset] * c_evec[jA + 3];
  c_evec[coffset + 6] -= c_evec[coffset] * c_evec[jA + 6];
  c_evec[aoffset + 6] -= c_evec[aoffset] * c_evec[jA + 6];
  if (fabs(c_evec[aoffset + 3]) > fabs(c_evec[coffset + 3])) {
    jBcol = coffset;
    coffset = aoffset;
    aoffset = jBcol;
  }
  c_evec[aoffset + 3] /= c_evec[coffset + 3];
  c_evec[aoffset + 6] -= c_evec[aoffset + 3] * c_evec[coffset + 6];
  offset[1] = B[coffset] - B[jA] * c_evec[coffset];
  offset[2] =
      (B[aoffset] - B[jA] * c_evec[aoffset]) - offset[1] * c_evec[aoffset + 3];
  offset[2] /= c_evec[aoffset + 6];
  offset[0] = B[jA] - offset[2] * c_evec[jA + 6];
  offset[1] -= offset[2] * c_evec[coffset + 6];
  offset[1] /= c_evec[coffset + 3];
  offset[0] -= offset[1] * c_evec[jA + 3];
  offset[0] /= c_evec[jA];
  /*  Eqn(21) */
  /* gain = [1/ax_, 0, 0; 0, 1/bx_, 0; 0,0,1/cx_]; */
  bkj = pqr_[0] * pqr_[0];
  s = M_[0] * M_[4];
  b_gain_tmp = pqr_[1] * pqr_[1];
  gain_tmp = pqr_[2] * pqr_[2];
  c_gain_tmp = M_[0] * M_[8];
  gain[0] =
      1.0 /
      sqrt(((bkj / (M_[0] * M_[0]) + b_gain_tmp / s) + gain_tmp / c_gain_tmp) -
           c_b_tmp[3] / M_[0]);
  gain[3] = 0.0;
  gain[6] = 0.0;
  gain[1] = 0.0;
  d_gain_tmp = M_[4] * M_[8];
  gain[4] =
      1.0 /
      sqrt(((bkj / s + b_gain_tmp / (M_[4] * M_[4])) + gain_tmp / d_gain_tmp) -
           c_b_tmp[3] / M_[4]);
  gain[7] = 0.0;
  gain[2] = 0.0;
  gain[5] = 0.0;
  gain[8] = 1.0 / sqrt(((bkj / c_gain_tmp + b_gain_tmp / d_gain_tmp) +
                        gain_tmp / (M_[8] * M_[8])) -
                       c_b_tmp[3] / M_[8]);
  /* matrix = rotation; */
}

/*
 * File trailer for ellipsoid_fit_test.c
 *
 * [EOF]
 */
