/*
 * File: ellipsoid_fit.c
 *
 * MATLAB Coder version            : 5.3
 * C/C++ source code generated on  : 02-Aug-2024 10:19:14
 */

/* Include Files */
#include "ellipsoid_fit.h"
#include "eig.h"
#include "ellipsoid_fit_test_emxutil.h"
#include "ellipsoid_fit_test_types.h"
#include "mldivide.h"
#include "relop.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Declarations */
static void times(emxArray_real_T *varargin_6, const emxArray_real_T *y,
                  const emxArray_real_T *b_y);

/* Function Definitions */
/*
 * Arguments    : emxArray_real_T *varargin_6
 *                const emxArray_real_T *y
 *                const emxArray_real_T *b_y
 * Return Type  : void
 */
static void times(emxArray_real_T *varargin_6, const emxArray_real_T *y,
                  const emxArray_real_T *b_y)
{
  const double *b_y_data;
  const double *y_data;
  double *varargin_6_data;
  int aux_0_1;
  int aux_1_1;
  int b_loop_ub;
  int i;
  int i1;
  int loop_ub;
  int stride_0_0;
  int stride_0_1;
  int stride_1_0;
  int stride_1_1;
  y_data = b_y->data;
  b_y_data = y->data;
  i = varargin_6->size[0] * varargin_6->size[1];
  if (b_y->size[0] == 1) {
    varargin_6->size[0] = y->size[0];
  } else {
    varargin_6->size[0] = b_y->size[0];
  }
  if (b_y->size[1] == 1) {
    varargin_6->size[1] = y->size[1];
  } else {
    varargin_6->size[1] = b_y->size[1];
  }
  emxEnsureCapacity_real_T(varargin_6, i);
  varargin_6_data = varargin_6->data;
  stride_0_0 = (y->size[0] != 1);
  stride_0_1 = (y->size[1] != 1);
  stride_1_0 = (b_y->size[0] != 1);
  stride_1_1 = (b_y->size[1] != 1);
  aux_0_1 = 0;
  aux_1_1 = 0;
  if (b_y->size[1] == 1) {
    loop_ub = y->size[1];
  } else {
    loop_ub = b_y->size[1];
  }
  for (i = 0; i < loop_ub; i++) {
    if (b_y->size[0] == 1) {
      b_loop_ub = y->size[0];
    } else {
      b_loop_ub = b_y->size[0];
    }
    for (i1 = 0; i1 < b_loop_ub; i1++) {
      varargin_6_data[i1 + varargin_6->size[0] * i] =
          b_y_data[i1 * stride_0_0 + y->size[0] * aux_0_1] *
          y_data[i1 * stride_1_0 + b_y->size[0] * aux_1_1];
    }
    aux_1_1 += stride_1_1;
    aux_0_1 += stride_0_1;
  }
}

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
 * Arguments    : const emxArray_real_T *x
 *                const emxArray_real_T *y
 *                const emxArray_real_T *z
 *                creal_T u[10]
 * Return Type  : void
 */
void ellipsoid_fit(const emxArray_real_T *x, const emxArray_real_T *y,
                   const emxArray_real_T *z, creal_T u[10])
{
  emxArray_real_T *D;
  emxArray_real_T *S;
  emxArray_real_T *b_y;
  emxArray_real_T *c_y;
  emxArray_real_T *d_y;
  emxArray_real_T *varargin_1;
  emxArray_real_T *varargin_2;
  emxArray_real_T *varargin_3;
  emxArray_real_T *varargin_4;
  emxArray_real_T *varargin_5;
  emxArray_real_T *varargin_6;
  creal_T eval[36];
  creal_T evec[36];
  creal_T b_b_tmp[24];
  creal_T b_ex[6];
  creal_T c_b_tmp[4];
  creal_T ex;
  creal_T ex_tmp;
  double dv[36];
  double b_tmp[24];
  double A[16];
  const double *x_data;
  const double *y_data;
  const double *z_data;
  double b_b_tmp_re_tmp;
  double b_tmp_re_tmp;
  double bkj;
  double c_b_tmp_re_tmp;
  double d_b_tmp_re_tmp;
  double s;
  double *D_data;
  double *S_data;
  double *b_y_data;
  double *c_y_data;
  double *d_y_data;
  double *varargin_1_data;
  double *varargin_2_data;
  double *varargin_3_data;
  double *varargin_4_data;
  double *varargin_5_data;
  int b_input_sizes_idx_1;
  int c_input_sizes_idx_1;
  int d_input_sizes_idx_1;
  int e_input_sizes_idx_1;
  int f_input_sizes_idx_1;
  int g_input_sizes_idx_1;
  int h_input_sizes_idx_1;
  int i;
  int i1;
  int i2;
  int i3;
  int i4;
  int i_input_sizes_idx_1;
  int input_sizes_idx_1;
  int jA;
  int kAcol;
  int result;
  signed char ipiv[4];
  signed char sizes_idx_1;
  bool empty_non_axis_sizes;
  z_data = z->data;
  y_data = y->data;
  x_data = x->data;
  emxInit_real_T(&b_y, 2);
  /*  Design matrix */
  i = b_y->size[0] * b_y->size[1];
  b_y->size[0] = y->size[0];
  b_y->size[1] = y->size[1];
  emxEnsureCapacity_real_T(b_y, i);
  b_y_data = b_y->data;
  jA = y->size[0] * y->size[1];
  for (i = 0; i < jA; i++) {
    b_y_data[i] = 2.0 * y_data[i];
  }
  emxInit_real_T(&c_y, 2);
  i = c_y->size[0] * c_y->size[1];
  c_y->size[0] = x->size[0];
  c_y->size[1] = x->size[1];
  emxEnsureCapacity_real_T(c_y, i);
  c_y_data = c_y->data;
  jA = x->size[0] * x->size[1];
  for (i = 0; i < jA; i++) {
    c_y_data[i] = 2.0 * x_data[i];
  }
  emxInit_real_T(&d_y, 2);
  i = d_y->size[0] * d_y->size[1];
  d_y->size[0] = z->size[0];
  d_y->size[1] = z->size[1];
  emxEnsureCapacity_real_T(d_y, i);
  d_y_data = d_y->data;
  jA = z->size[0] * z->size[1];
  for (i = 0; i < jA; i++) {
    d_y_data[i] = 2.0 * z_data[i];
  }
  if ((x->size[0] == 0) || (x->size[1] == 0)) {
    kAcol = 0;
  } else {
    jA = x->size[0];
    kAcol = x->size[1];
    if (jA >= kAcol) {
      kAcol = jA;
    }
  }
  emxInit_real_T(&varargin_1, 2);
  i = varargin_1->size[0] * varargin_1->size[1];
  varargin_1->size[0] = x->size[0];
  varargin_1->size[1] = x->size[1];
  emxEnsureCapacity_real_T(varargin_1, i);
  varargin_1_data = varargin_1->data;
  jA = x->size[0] * x->size[1];
  for (i = 0; i < jA; i++) {
    bkj = x_data[i];
    varargin_1_data[i] = bkj * bkj;
  }
  emxInit_real_T(&varargin_2, 2);
  i = varargin_2->size[0] * varargin_2->size[1];
  varargin_2->size[0] = y->size[0];
  varargin_2->size[1] = y->size[1];
  emxEnsureCapacity_real_T(varargin_2, i);
  varargin_2_data = varargin_2->data;
  jA = y->size[0] * y->size[1];
  for (i = 0; i < jA; i++) {
    bkj = y_data[i];
    varargin_2_data[i] = bkj * bkj;
  }
  emxInit_real_T(&varargin_3, 2);
  i = varargin_3->size[0] * varargin_3->size[1];
  varargin_3->size[0] = z->size[0];
  varargin_3->size[1] = z->size[1];
  emxEnsureCapacity_real_T(varargin_3, i);
  varargin_3_data = varargin_3->data;
  jA = z->size[0] * z->size[1];
  for (i = 0; i < jA; i++) {
    bkj = z_data[i];
    varargin_3_data[i] = bkj * bkj;
  }
  emxInit_real_T(&varargin_4, 2);
  if ((b_y->size[0] == z->size[0]) && (b_y->size[1] == z->size[1])) {
    i = varargin_4->size[0] * varargin_4->size[1];
    varargin_4->size[0] = b_y->size[0];
    varargin_4->size[1] = b_y->size[1];
    emxEnsureCapacity_real_T(varargin_4, i);
    varargin_4_data = varargin_4->data;
    jA = b_y->size[0] * b_y->size[1];
    for (i = 0; i < jA; i++) {
      varargin_4_data[i] = b_y_data[i] * z_data[i];
    }
  } else {
    times(varargin_4, b_y, z);
    varargin_4_data = varargin_4->data;
  }
  emxInit_real_T(&varargin_5, 2);
  if ((c_y->size[0] == z->size[0]) && (c_y->size[1] == z->size[1])) {
    i = varargin_5->size[0] * varargin_5->size[1];
    varargin_5->size[0] = c_y->size[0];
    varargin_5->size[1] = c_y->size[1];
    emxEnsureCapacity_real_T(varargin_5, i);
    varargin_5_data = varargin_5->data;
    jA = c_y->size[0] * c_y->size[1];
    for (i = 0; i < jA; i++) {
      varargin_5_data[i] = c_y_data[i] * z_data[i];
    }
  } else {
    times(varargin_5, c_y, z);
    varargin_5_data = varargin_5->data;
  }
  emxInit_real_T(&varargin_6, 2);
  if ((c_y->size[0] == y->size[0]) && (c_y->size[1] == y->size[1])) {
    i = varargin_6->size[0] * varargin_6->size[1];
    varargin_6->size[0] = c_y->size[0];
    varargin_6->size[1] = c_y->size[1];
    emxEnsureCapacity_real_T(varargin_6, i);
    S_data = varargin_6->data;
    jA = c_y->size[0] * c_y->size[1];
    for (i = 0; i < jA; i++) {
      S_data[i] = c_y_data[i] * y_data[i];
    }
  } else {
    times(varargin_6, c_y, y);
    S_data = varargin_6->data;
  }
  if ((varargin_1->size[0] != 0) && (varargin_1->size[1] != 0)) {
    result = varargin_1->size[0];
  } else if ((varargin_2->size[0] != 0) && (varargin_2->size[1] != 0)) {
    result = varargin_2->size[0];
  } else if ((varargin_3->size[0] != 0) && (varargin_3->size[1] != 0)) {
    result = varargin_3->size[0];
  } else if ((varargin_4->size[0] != 0) && (varargin_4->size[1] != 0)) {
    result = varargin_4->size[0];
  } else if ((varargin_5->size[0] != 0) && (varargin_5->size[1] != 0)) {
    result = varargin_5->size[0];
  } else if ((varargin_6->size[0] != 0) && (varargin_6->size[1] != 0)) {
    result = varargin_6->size[0];
  } else if ((c_y->size[0] != 0) && (c_y->size[1] != 0)) {
    result = c_y->size[0];
  } else if ((b_y->size[0] != 0) && (b_y->size[1] != 0)) {
    result = b_y->size[0];
  } else if ((d_y->size[0] != 0) && (d_y->size[1] != 0)) {
    result = d_y->size[0];
  } else if (kAcol != 0) {
    result = kAcol;
  } else {
    result = varargin_1->size[0];
    if (varargin_2->size[0] > varargin_1->size[0]) {
      result = varargin_2->size[0];
    }
    if (varargin_3->size[0] > result) {
      result = varargin_3->size[0];
    }
    if (varargin_4->size[0] > result) {
      result = varargin_4->size[0];
    }
    if (varargin_5->size[0] > result) {
      result = varargin_5->size[0];
    }
    if (varargin_6->size[0] > result) {
      result = varargin_6->size[0];
    }
    if (c_y->size[0] > result) {
      result = c_y->size[0];
    }
    if (b_y->size[0] > result) {
      result = b_y->size[0];
    }
    if (d_y->size[0] > result) {
      result = d_y->size[0];
    }
  }
  empty_non_axis_sizes = (result == 0);
  if (empty_non_axis_sizes ||
      ((varargin_1->size[0] != 0) && (varargin_1->size[1] != 0))) {
    input_sizes_idx_1 = varargin_1->size[1];
  } else {
    input_sizes_idx_1 = 0;
  }
  if (empty_non_axis_sizes ||
      ((varargin_2->size[0] != 0) && (varargin_2->size[1] != 0))) {
    b_input_sizes_idx_1 = varargin_2->size[1];
  } else {
    b_input_sizes_idx_1 = 0;
  }
  if (empty_non_axis_sizes ||
      ((varargin_3->size[0] != 0) && (varargin_3->size[1] != 0))) {
    c_input_sizes_idx_1 = varargin_3->size[1];
  } else {
    c_input_sizes_idx_1 = 0;
  }
  if (empty_non_axis_sizes ||
      ((varargin_4->size[0] != 0) && (varargin_4->size[1] != 0))) {
    d_input_sizes_idx_1 = varargin_4->size[1];
  } else {
    d_input_sizes_idx_1 = 0;
  }
  if (empty_non_axis_sizes ||
      ((varargin_5->size[0] != 0) && (varargin_5->size[1] != 0))) {
    e_input_sizes_idx_1 = varargin_5->size[1];
  } else {
    e_input_sizes_idx_1 = 0;
  }
  if (empty_non_axis_sizes ||
      ((varargin_6->size[0] != 0) && (varargin_6->size[1] != 0))) {
    f_input_sizes_idx_1 = varargin_6->size[1];
  } else {
    f_input_sizes_idx_1 = 0;
  }
  if (empty_non_axis_sizes || ((c_y->size[0] != 0) && (c_y->size[1] != 0))) {
    g_input_sizes_idx_1 = c_y->size[1];
  } else {
    g_input_sizes_idx_1 = 0;
  }
  if (empty_non_axis_sizes || ((b_y->size[0] != 0) && (b_y->size[1] != 0))) {
    h_input_sizes_idx_1 = b_y->size[1];
  } else {
    h_input_sizes_idx_1 = 0;
  }
  if (empty_non_axis_sizes || ((d_y->size[0] != 0) && (d_y->size[1] != 0))) {
    i_input_sizes_idx_1 = d_y->size[1];
  } else {
    i_input_sizes_idx_1 = 0;
  }
  if (empty_non_axis_sizes || (kAcol != 0)) {
    sizes_idx_1 = 1;
  } else {
    sizes_idx_1 = 0;
  }
  emxInit_real_T(&D, 2);
  i = input_sizes_idx_1 + b_input_sizes_idx_1;
  i1 = i + c_input_sizes_idx_1;
  jA = i1 + d_input_sizes_idx_1;
  kAcol = jA + e_input_sizes_idx_1;
  i2 = D->size[0] * D->size[1];
  i3 = (((kAcol + f_input_sizes_idx_1) + g_input_sizes_idx_1) +
        h_input_sizes_idx_1) +
       i_input_sizes_idx_1;
  D->size[0] = i3 + sizes_idx_1;
  D->size[1] = result;
  emxEnsureCapacity_real_T(D, i2);
  D_data = D->data;
  for (i2 = 0; i2 < result; i2++) {
    for (i4 = 0; i4 < input_sizes_idx_1; i4++) {
      D_data[i4 + D->size[0] * i2] = varargin_1_data[i2 + result * i4];
    }
  }
  emxFree_real_T(&varargin_1);
  for (i2 = 0; i2 < result; i2++) {
    for (i4 = 0; i4 < b_input_sizes_idx_1; i4++) {
      D_data[(i4 + input_sizes_idx_1) + D->size[0] * i2] =
          varargin_2_data[i2 + result * i4];
    }
  }
  emxFree_real_T(&varargin_2);
  for (i2 = 0; i2 < result; i2++) {
    for (i4 = 0; i4 < c_input_sizes_idx_1; i4++) {
      D_data[((i4 + input_sizes_idx_1) + b_input_sizes_idx_1) +
             D->size[0] * i2] = varargin_3_data[i2 + result * i4];
    }
  }
  emxFree_real_T(&varargin_3);
  for (i2 = 0; i2 < result; i2++) {
    for (i4 = 0; i4 < d_input_sizes_idx_1; i4++) {
      D_data[(((i4 + input_sizes_idx_1) + b_input_sizes_idx_1) +
              c_input_sizes_idx_1) +
             D->size[0] * i2] = varargin_4_data[i2 + result * i4];
    }
  }
  emxFree_real_T(&varargin_4);
  for (i2 = 0; i2 < result; i2++) {
    for (i4 = 0; i4 < e_input_sizes_idx_1; i4++) {
      D_data[((((i4 + input_sizes_idx_1) + b_input_sizes_idx_1) +
               c_input_sizes_idx_1) +
              d_input_sizes_idx_1) +
             D->size[0] * i2] = varargin_5_data[i2 + result * i4];
    }
  }
  emxFree_real_T(&varargin_5);
  for (i2 = 0; i2 < result; i2++) {
    for (i4 = 0; i4 < f_input_sizes_idx_1; i4++) {
      D_data[((((i4 + i) + c_input_sizes_idx_1) + d_input_sizes_idx_1) +
              e_input_sizes_idx_1) +
             D->size[0] * i2] = S_data[i2 + result * i4];
    }
  }
  emxFree_real_T(&varargin_6);
  for (i = 0; i < result; i++) {
    for (i2 = 0; i2 < g_input_sizes_idx_1; i2++) {
      D_data[((((i2 + i1) + d_input_sizes_idx_1) + e_input_sizes_idx_1) +
              f_input_sizes_idx_1) +
             D->size[0] * i] = c_y_data[i + result * i2];
    }
  }
  emxFree_real_T(&c_y);
  for (i = 0; i < result; i++) {
    for (i1 = 0; i1 < h_input_sizes_idx_1; i1++) {
      D_data[((((i1 + jA) + e_input_sizes_idx_1) + f_input_sizes_idx_1) +
              g_input_sizes_idx_1) +
             D->size[0] * i] = b_y_data[i + result * i1];
    }
  }
  emxFree_real_T(&b_y);
  for (i = 0; i < result; i++) {
    for (i1 = 0; i1 < i_input_sizes_idx_1; i1++) {
      D_data[((((i1 + kAcol) + f_input_sizes_idx_1) + g_input_sizes_idx_1) +
              h_input_sizes_idx_1) +
             D->size[0] * i] = d_y_data[i + result * i1];
    }
  }
  emxFree_real_T(&d_y);
  for (i = 0; i < result; i++) {
    jA = sizes_idx_1;
    for (i1 = 0; i1 < jA; i1++) {
      D_data[i3 + D->size[0] * i] = 1.0;
    }
  }
  emxInit_real_T(&S, 2);
  /*  Constraint kJ > I^2 */
  /*  Ellipsoid if k = 4  */
  /*  Eqn(7) */
  /*  Eqn(11) */
  jA = D->size[0];
  c_input_sizes_idx_1 = D->size[1];
  kAcol = D->size[0];
  i = S->size[0] * S->size[1];
  S->size[0] = D->size[0];
  S->size[1] = D->size[0];
  emxEnsureCapacity_real_T(S, i);
  S_data = S->data;
  for (f_input_sizes_idx_1 = 0; f_input_sizes_idx_1 < kAcol;
       f_input_sizes_idx_1++) {
    input_sizes_idx_1 = f_input_sizes_idx_1 * jA;
    for (d_input_sizes_idx_1 = 0; d_input_sizes_idx_1 < jA;
         d_input_sizes_idx_1++) {
      S_data[input_sizes_idx_1 + d_input_sizes_idx_1] = 0.0;
    }
    for (e_input_sizes_idx_1 = 0; e_input_sizes_idx_1 < c_input_sizes_idx_1;
         e_input_sizes_idx_1++) {
      b_input_sizes_idx_1 = e_input_sizes_idx_1 * D->size[0];
      bkj = D_data[e_input_sizes_idx_1 * D->size[0] + f_input_sizes_idx_1];
      for (d_input_sizes_idx_1 = 0; d_input_sizes_idx_1 < jA;
           d_input_sizes_idx_1++) {
        i = input_sizes_idx_1 + d_input_sizes_idx_1;
        S_data[i] += D_data[b_input_sizes_idx_1 + d_input_sizes_idx_1] * bkj;
      }
    }
  }
  emxFree_real_T(&D);
  /*  6X6 */
  /*  6X4 */
  /*  4X6 */
  /*  4X4 */
  /*  Eqn(14) and Eqn(15) */
  /*  eval = Diagonal matrix of eigenvalues */
  /*  evec = Matix containing corresponding eigenvectors in its column */
  for (i = 0; i < 4; i++) {
    jA = i << 2;
    A[jA] = S_data[S->size[0] * (i + 6) + 6];
    A[jA + 1] = S_data[S->size[0] * (i + 6) + 7];
    A[jA + 2] = S_data[S->size[0] * (i + 6) + 8];
    A[jA + 3] = S_data[S->size[0] * (i + 6) + 9];
  }
  for (i = 0; i < 6; i++) {
    jA = i << 2;
    b_tmp[jA] = S_data[S->size[0] * i + 6];
    b_tmp[jA + 1] = S_data[S->size[0] * i + 7];
    b_tmp[jA + 2] = S_data[S->size[0] * i + 8];
    b_tmp[jA + 3] = S_data[S->size[0] * i + 9];
  }
  ipiv[0] = 1;
  ipiv[1] = 2;
  ipiv[2] = 3;
  ipiv[3] = 4;
  for (f_input_sizes_idx_1 = 0; f_input_sizes_idx_1 < 3;
       f_input_sizes_idx_1++) {
    input_sizes_idx_1 = 2 - f_input_sizes_idx_1;
    b_input_sizes_idx_1 = f_input_sizes_idx_1 * 5;
    c_input_sizes_idx_1 = b_input_sizes_idx_1 + 2;
    kAcol = 4 - f_input_sizes_idx_1;
    jA = 0;
    bkj = fabs(A[b_input_sizes_idx_1]);
    for (e_input_sizes_idx_1 = 2; e_input_sizes_idx_1 <= kAcol;
         e_input_sizes_idx_1++) {
      s = fabs(A[(b_input_sizes_idx_1 + e_input_sizes_idx_1) - 1]);
      if (s > bkj) {
        jA = e_input_sizes_idx_1 - 1;
        bkj = s;
      }
    }
    if (A[b_input_sizes_idx_1 + jA] != 0.0) {
      if (jA != 0) {
        jA += f_input_sizes_idx_1;
        ipiv[f_input_sizes_idx_1] = (signed char)(jA + 1);
        bkj = A[f_input_sizes_idx_1];
        A[f_input_sizes_idx_1] = A[jA];
        A[jA] = bkj;
        bkj = A[f_input_sizes_idx_1 + 4];
        A[f_input_sizes_idx_1 + 4] = A[jA + 4];
        A[jA + 4] = bkj;
        bkj = A[f_input_sizes_idx_1 + 8];
        A[f_input_sizes_idx_1 + 8] = A[jA + 8];
        A[jA + 8] = bkj;
        bkj = A[f_input_sizes_idx_1 + 12];
        A[f_input_sizes_idx_1 + 12] = A[jA + 12];
        A[jA + 12] = bkj;
      }
      i = (b_input_sizes_idx_1 - f_input_sizes_idx_1) + 4;
      for (d_input_sizes_idx_1 = c_input_sizes_idx_1; d_input_sizes_idx_1 <= i;
           d_input_sizes_idx_1++) {
        A[d_input_sizes_idx_1 - 1] /= A[b_input_sizes_idx_1];
      }
    }
    jA = b_input_sizes_idx_1;
    for (c_input_sizes_idx_1 = 0; c_input_sizes_idx_1 <= input_sizes_idx_1;
         c_input_sizes_idx_1++) {
      bkj = A[(b_input_sizes_idx_1 + (c_input_sizes_idx_1 << 2)) + 4];
      if (bkj != 0.0) {
        i = jA + 6;
        i1 = (jA - f_input_sizes_idx_1) + 8;
        for (kAcol = i; kAcol <= i1; kAcol++) {
          A[kAcol - 1] += A[((b_input_sizes_idx_1 + kAcol) - jA) - 5] * -bkj;
        }
      }
      jA += 4;
    }
    sizes_idx_1 = ipiv[f_input_sizes_idx_1];
    if (sizes_idx_1 != f_input_sizes_idx_1 + 1) {
      for (c_input_sizes_idx_1 = 0; c_input_sizes_idx_1 < 6;
           c_input_sizes_idx_1++) {
        jA = c_input_sizes_idx_1 << 2;
        kAcol = f_input_sizes_idx_1 + jA;
        bkj = b_tmp[kAcol];
        jA = (sizes_idx_1 + jA) - 1;
        b_tmp[kAcol] = b_tmp[jA];
        b_tmp[jA] = bkj;
      }
    }
  }
  for (f_input_sizes_idx_1 = 0; f_input_sizes_idx_1 < 6;
       f_input_sizes_idx_1++) {
    c_input_sizes_idx_1 = f_input_sizes_idx_1 << 2;
    for (e_input_sizes_idx_1 = 0; e_input_sizes_idx_1 < 4;
         e_input_sizes_idx_1++) {
      kAcol = e_input_sizes_idx_1 << 2;
      i = e_input_sizes_idx_1 + c_input_sizes_idx_1;
      if (b_tmp[i] != 0.0) {
        i1 = e_input_sizes_idx_1 + 2;
        for (d_input_sizes_idx_1 = i1; d_input_sizes_idx_1 < 5;
             d_input_sizes_idx_1++) {
          jA = (d_input_sizes_idx_1 + c_input_sizes_idx_1) - 1;
          b_tmp[jA] -= b_tmp[i] * A[(d_input_sizes_idx_1 + kAcol) - 1];
        }
      }
    }
  }
  for (f_input_sizes_idx_1 = 0; f_input_sizes_idx_1 < 6;
       f_input_sizes_idx_1++) {
    c_input_sizes_idx_1 = f_input_sizes_idx_1 << 2;
    for (e_input_sizes_idx_1 = 3; e_input_sizes_idx_1 >= 0;
         e_input_sizes_idx_1--) {
      kAcol = e_input_sizes_idx_1 << 2;
      i = e_input_sizes_idx_1 + c_input_sizes_idx_1;
      bkj = b_tmp[i];
      if (bkj != 0.0) {
        b_tmp[i] = bkj / A[e_input_sizes_idx_1 + kAcol];
        for (d_input_sizes_idx_1 = 0; d_input_sizes_idx_1 < e_input_sizes_idx_1;
             d_input_sizes_idx_1++) {
          jA = d_input_sizes_idx_1 + c_input_sizes_idx_1;
          b_tmp[jA] -= b_tmp[i] * A[d_input_sizes_idx_1 + kAcol];
        }
      }
    }
  }
  for (i = 0; i < 6; i++) {
    for (i1 = 0; i1 < 6; i1++) {
      jA = i1 << 2;
      dv[i + 6 * i1] = S_data[i + S->size[0] * i1] -
                       (((S_data[i + S->size[0] * 6] * b_tmp[jA] +
                          S_data[i + S->size[0] * 7] * b_tmp[jA + 1]) +
                         S_data[i + S->size[0] * 8] * b_tmp[jA + 2]) +
                        S_data[i + S->size[0] * 9] * b_tmp[jA + 3]);
    }
  }
  emxFree_real_T(&S);
  mldivide(dv);
  eig(dv, evec, eval);
  /*  Locate the index of the 'only' positive eigenvalue. */
  /*  If M is singular, then u1 is eigenvector of largest eigen-value. */
  for (f_input_sizes_idx_1 = 0; f_input_sizes_idx_1 < 6;
       f_input_sizes_idx_1++) {
    ex = eval[6 * f_input_sizes_idx_1];
    for (d_input_sizes_idx_1 = 0; d_input_sizes_idx_1 < 5;
         d_input_sizes_idx_1++) {
      ex_tmp = eval[(d_input_sizes_idx_1 + 6 * f_input_sizes_idx_1) + 1];
      empty_non_axis_sizes = relop(ex, ex_tmp);
      if (empty_non_axis_sizes) {
        ex = ex_tmp;
      }
    }
    b_ex[f_input_sizes_idx_1] = ex;
  }
  c_input_sizes_idx_1 = 0;
  ex = b_ex[0];
  for (e_input_sizes_idx_1 = 0; e_input_sizes_idx_1 < 5;
       e_input_sizes_idx_1++) {
    ex_tmp = b_ex[e_input_sizes_idx_1 + 1];
    empty_non_axis_sizes = relop(ex, ex_tmp);
    if (empty_non_axis_sizes) {
      ex = ex_tmp;
      c_input_sizes_idx_1 = e_input_sizes_idx_1 + 1;
    }
  }
  /*  Eigenvector for  positive eigenvalue. */
  for (i = 0; i < 24; i++) {
    b_b_tmp[i].re = -b_tmp[i];
    b_b_tmp[i].im = 0.0;
  }
  for (i = 0; i < 4; i++) {
    bkj = 0.0;
    s = 0.0;
    for (i1 = 0; i1 < 6; i1++) {
      jA = i + (i1 << 2);
      kAcol = i1 + 6 * c_input_sizes_idx_1;
      b_tmp_re_tmp = b_b_tmp[jA].re;
      b_b_tmp_re_tmp = evec[kAcol].im;
      c_b_tmp_re_tmp = b_b_tmp[jA].im;
      d_b_tmp_re_tmp = evec[kAcol].re;
      bkj += b_tmp_re_tmp * d_b_tmp_re_tmp - c_b_tmp_re_tmp * b_b_tmp_re_tmp;
      s += b_tmp_re_tmp * b_b_tmp_re_tmp + c_b_tmp_re_tmp * d_b_tmp_re_tmp;
    }
    c_b_tmp[i].re = bkj;
    c_b_tmp[i].im = s;
  }
  memcpy(&u[0], &evec[c_input_sizes_idx_1 * 6], 6U * sizeof(creal_T));
  memcpy(&u[6], &c_b_tmp[0], 4U * sizeof(creal_T));
}

/*
 * File trailer for ellipsoid_fit.c
 *
 * [EOF]
 */
