/*
 * File: ellipsoid_fit.c
 *
 * MATLAB Coder version            : 5.3
 * C/C++ source code generated on  : 13-Aug-2024 17:16:40
 */

/* Include Files */
#include "ellipsoid_fit.h"
#include "ellipsoid_fit_test_emxutil.h"
#include "ellipsoid_fit_test_types.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : emxArray_real_T *D
 *                const emxArray_real_T *r3
 *                const emxArray_real_T *r4
 *                const emxArray_real_T *r5
 *                const emxArray_real_T *y
 *                const emxArray_real_T *z_m
 *                const emxArray_real_T *b_y
 *                const emxArray_real_T *y_m
 *                const emxArray_real_T *x_m
 * Return Type  : void
 */
void binary_expand_op(emxArray_real_T *D, const emxArray_real_T *r3,
                      const emxArray_real_T *r4, const emxArray_real_T *r5,
                      const emxArray_real_T *y, const emxArray_real_T *z_m,
                      const emxArray_real_T *b_y, const emxArray_real_T *y_m,
                      const emxArray_real_T *x_m)
{
  emxArray_real_T *c_y;
  emxArray_real_T *d_y;
  emxArray_real_T *e_y;
  const double *b_y_data;
  const double *r;
  const double *r1;
  const double *r2;
  const double *y_data;
  const double *y_m_data;
  const double *z_m_data;
  double *D_data;
  double *c_y_data;
  double *d_y_data;
  double *e_y_data;
  int i;
  int loop_ub;
  int stride_0_0;
  int stride_1_0;
  y_m_data = y_m->data;
  y_data = b_y->data;
  z_m_data = z_m->data;
  b_y_data = y->data;
  r = r5->data;
  r1 = r4->data;
  r2 = r3->data;
  emxInit_real_T(&c_y, 1);
  i = c_y->size[0];
  if (z_m->size[0] == 1) {
    c_y->size[0] = y->size[0];
  } else {
    c_y->size[0] = z_m->size[0];
  }
  emxEnsureCapacity_real_T(c_y, i);
  c_y_data = c_y->data;
  stride_0_0 = (y->size[0] != 1);
  stride_1_0 = (z_m->size[0] != 1);
  if (z_m->size[0] == 1) {
    loop_ub = y->size[0];
  } else {
    loop_ub = z_m->size[0];
  }
  for (i = 0; i < loop_ub; i++) {
    c_y_data[i] = b_y_data[i * stride_0_0] * z_m_data[i * stride_1_0];
  }
  emxInit_real_T(&d_y, 1);
  i = d_y->size[0];
  if (z_m->size[0] == 1) {
    d_y->size[0] = b_y->size[0];
  } else {
    d_y->size[0] = z_m->size[0];
  }
  emxEnsureCapacity_real_T(d_y, i);
  d_y_data = d_y->data;
  stride_0_0 = (b_y->size[0] != 1);
  stride_1_0 = (z_m->size[0] != 1);
  if (z_m->size[0] == 1) {
    loop_ub = b_y->size[0];
  } else {
    loop_ub = z_m->size[0];
  }
  for (i = 0; i < loop_ub; i++) {
    d_y_data[i] = y_data[i * stride_0_0] * z_m_data[i * stride_1_0];
  }
  emxInit_real_T(&e_y, 1);
  i = e_y->size[0];
  if (y_m->size[0] == 1) {
    e_y->size[0] = b_y->size[0];
  } else {
    e_y->size[0] = y_m->size[0];
  }
  emxEnsureCapacity_real_T(e_y, i);
  e_y_data = e_y->data;
  stride_0_0 = (b_y->size[0] != 1);
  stride_1_0 = (y_m->size[0] != 1);
  if (y_m->size[0] == 1) {
    loop_ub = b_y->size[0];
  } else {
    loop_ub = y_m->size[0];
  }
  for (i = 0; i < loop_ub; i++) {
    e_y_data[i] = y_data[i * stride_0_0] * y_m_data[i * stride_1_0];
  }
  i = D->size[0] * D->size[1];
  D->size[0] = 10;
  D->size[1] = r3->size[0];
  emxEnsureCapacity_real_T(D, i);
  D_data = D->data;
  loop_ub = r3->size[0];
  for (i = 0; i < loop_ub; i++) {
    D_data[10 * i] = r2[i];
  }
  loop_ub = r4->size[0];
  for (i = 0; i < loop_ub; i++) {
    D_data[10 * i + 1] = r1[i];
  }
  loop_ub = r5->size[0];
  for (i = 0; i < loop_ub; i++) {
    D_data[10 * i + 2] = r[i];
  }
  loop_ub = c_y->size[0];
  for (i = 0; i < loop_ub; i++) {
    D_data[10 * i + 3] = c_y_data[i];
  }
  emxFree_real_T(&c_y);
  loop_ub = d_y->size[0];
  for (i = 0; i < loop_ub; i++) {
    D_data[10 * i + 4] = d_y_data[i];
  }
  emxFree_real_T(&d_y);
  loop_ub = e_y->size[0];
  for (i = 0; i < loop_ub; i++) {
    D_data[10 * i + 5] = e_y_data[i];
  }
  emxFree_real_T(&e_y);
  loop_ub = b_y->size[0];
  for (i = 0; i < loop_ub; i++) {
    D_data[10 * i + 6] = y_data[i];
  }
  loop_ub = y->size[0];
  for (i = 0; i < loop_ub; i++) {
    D_data[10 * i + 7] = b_y_data[i];
  }
  loop_ub = z_m->size[0];
  for (i = 0; i < loop_ub; i++) {
    D_data[10 * i + 8] = 2.0 * z_m_data[i];
  }
  loop_ub = x_m->size[0];
  for (i = 0; i < loop_ub; i++) {
    D_data[10 * i + 9] = 1.0;
  }
}

/*
 * File trailer for ellipsoid_fit.c
 *
 * [EOF]
 */
