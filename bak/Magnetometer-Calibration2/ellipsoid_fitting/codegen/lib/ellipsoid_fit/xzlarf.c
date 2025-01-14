/*
 * File: xzlarf.c
 *
 * MATLAB Coder version            : 5.3
 * C/C++ source code generated on  : 02-Aug-2024 17:35:13
 */

/* Include Files */
#include "xzlarf.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Declarations */
static int div_nde_s32_floor(int numerator);

/* Function Definitions */
/*
 * Arguments    : int numerator
 * Return Type  : int
 */
static int div_nde_s32_floor(int numerator)
{
  int b_numerator;
  if ((numerator < 0) && (numerator % 6 != 0)) {
    b_numerator = -1;
  } else {
    b_numerator = 0;
  }
  return numerator / 6 + b_numerator;
}

/*
 * Arguments    : int m
 *                int n
 *                int iv0
 *                double tau
 *                double C[36]
 *                int ic0
 *                double work[6]
 * Return Type  : void
 */
void xzlarf(int m, int n, int iv0, double tau, double C[36], int ic0,
            double work[6])
{
  double c;
  int b_i;
  int exitg1;
  int i;
  int ia;
  int iac;
  int lastc;
  int lastv;
  bool exitg2;
  if (tau != 0.0) {
    lastv = m;
    i = iv0 + m;
    while ((lastv > 0) && (C[i - 2] == 0.0)) {
      lastv--;
      i--;
    }
    lastc = n - 1;
    exitg2 = false;
    while ((!exitg2) && (lastc + 1 > 0)) {
      i = ic0 + lastc * 6;
      ia = i;
      do {
        exitg1 = 0;
        if (ia <= (i + lastv) - 1) {
          if (C[ia - 1] != 0.0) {
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
    lastc = -1;
  }
  if (lastv > 0) {
    if (lastc + 1 != 0) {
      if (0 <= lastc) {
        memset(&work[0], 0, (lastc + 1) * sizeof(double));
      }
      b_i = ic0 + 6 * lastc;
      for (iac = ic0; iac <= b_i; iac += 6) {
        c = 0.0;
        i = (iac + lastv) - 1;
        for (ia = iac; ia <= i; ia++) {
          c += C[ia - 1] * C[((iv0 + ia) - iac) - 1];
        }
        i = div_nde_s32_floor(iac - ic0);
        work[i] += c;
      }
    }
    if (!(-tau == 0.0)) {
      i = ic0;
      for (iac = 0; iac <= lastc; iac++) {
        if (work[iac] != 0.0) {
          c = work[iac] * -tau;
          b_i = lastv + i;
          for (ia = i; ia < b_i; ia++) {
            C[ia - 1] += C[((iv0 + ia) - i) - 1] * c;
          }
        }
        i += 6;
      }
    }
  }
}

/*
 * File trailer for xzlarf.c
 *
 * [EOF]
 */
