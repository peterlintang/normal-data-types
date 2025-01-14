/*
 * File: xdhseqr.c
 *
 * MATLAB Coder version            : 5.3
 * C/C++ source code generated on  : 13-Aug-2024 17:16:40
 */

/* Include Files */
#include "xdhseqr.h"
#include "rt_nonfinite.h"
#include "xdlanv2.h"
#include "xzlarfg.h"
#include <math.h>

/* Function Definitions */
/*
 * Arguments    : double h[36]
 *                double z[36]
 * Return Type  : int
 */
int eml_dlahqr(double h[36], double z[36])
{
  double v[3];
  double aa;
  double ab;
  double ba;
  double bb;
  double rt1r;
  double rt2r;
  double s;
  double s_tmp;
  double tst;
  int L;
  int b_i;
  int b_k;
  int hoffset;
  int i;
  int info;
  int its;
  int j;
  int k;
  int m;
  int nr;
  int sum1_tmp;
  bool exitg1;
  bool exitg2;
  bool exitg3;
  bool goto150;
  info = 0;
  v[0] = 0.0;
  h[2] = 0.0;
  h[3] = 0.0;
  v[1] = 0.0;
  h[9] = 0.0;
  h[10] = 0.0;
  v[2] = 0.0;
  h[16] = 0.0;
  h[17] = 0.0;
  h[23] = 0.0;
  i = 5;
  exitg1 = false;
  while ((!exitg1) && (i + 1 >= 1)) {
    L = 1;
    goto150 = false;
    its = 0;
    exitg2 = false;
    while ((!exitg2) && (its < 301)) {
      k = i;
      exitg3 = false;
      while ((!exitg3) && (k + 1 > L)) {
        b_i = k + 6 * (k - 1);
        ba = fabs(h[b_i]);
        if (ba <= 6.0125050800269183E-292) {
          exitg3 = true;
        } else {
          hoffset = k + 6 * k;
          bb = fabs(h[hoffset]);
          tst = fabs(h[b_i - 1]) + bb;
          if (tst == 0.0) {
            if (k - 1 >= 1) {
              tst = fabs(h[(k + 6 * (k - 2)) - 1]);
            }
            if (k + 2 <= 6) {
              tst += fabs(h[hoffset + 1]);
            }
          }
          if (ba <= 2.2204460492503131E-16 * tst) {
            tst = fabs(h[hoffset - 1]);
            if (ba > tst) {
              ab = ba;
              ba = tst;
            } else {
              ab = tst;
            }
            tst = fabs(h[b_i - 1] - h[hoffset]);
            if (bb > tst) {
              aa = bb;
              bb = tst;
            } else {
              aa = tst;
            }
            s = aa + ab;
            if (ba * (ab / s) <=
                fmax(6.0125050800269183E-292,
                     2.2204460492503131E-16 * (bb * (aa / s)))) {
              exitg3 = true;
            } else {
              k--;
            }
          } else {
            k--;
          }
        }
      }
      L = k + 1;
      if (k + 1 > 1) {
        h[k + 6 * (k - 1)] = 0.0;
      }
      if (k + 1 >= i) {
        goto150 = true;
        exitg2 = true;
      } else {
        if (its == 10) {
          hoffset = k + 6 * k;
          s = fabs(h[hoffset + 1]) + fabs(h[(k + 6 * (k + 1)) + 2]);
          tst = 0.75 * s + h[hoffset];
          aa = -0.4375 * s;
          ab = s;
          bb = tst;
        } else if (its == 20) {
          s = fabs(h[i + 6 * (i - 1)]) + fabs(h[(i + 6 * (i - 2)) - 1]);
          tst = 0.75 * s + h[i + 6 * i];
          aa = -0.4375 * s;
          ab = s;
          bb = tst;
        } else {
          hoffset = i + 6 * (i - 1);
          tst = h[hoffset - 1];
          ab = h[hoffset];
          aa = h[(i + 6 * i) - 1];
          bb = h[i + 6 * i];
        }
        s = ((fabs(tst) + fabs(aa)) + fabs(ab)) + fabs(bb);
        if (s == 0.0) {
          rt1r = 0.0;
          ba = 0.0;
          rt2r = 0.0;
          bb = 0.0;
        } else {
          tst /= s;
          ab /= s;
          aa /= s;
          bb /= s;
          ba = (tst + bb) / 2.0;
          tst = (tst - ba) * (bb - ba) - aa * ab;
          ab = sqrt(fabs(tst));
          if (tst >= 0.0) {
            rt1r = ba * s;
            rt2r = rt1r;
            ba = ab * s;
            bb = -ba;
          } else {
            rt1r = ba + ab;
            rt2r = ba - ab;
            if (fabs(rt1r - bb) <= fabs(rt2r - bb)) {
              rt1r *= s;
              rt2r = rt1r;
            } else {
              rt2r *= s;
              rt1r = rt2r;
            }
            ba = 0.0;
            bb = 0.0;
          }
        }
        m = i - 1;
        exitg3 = false;
        while ((!exitg3) && (m >= k + 1)) {
          hoffset = m + 6 * (m - 1);
          tst = h[hoffset];
          s_tmp = h[hoffset - 1];
          ab = s_tmp - rt2r;
          s = (fabs(ab) + fabs(bb)) + fabs(tst);
          aa = tst / s;
          hoffset = m + 6 * m;
          v[0] =
              (aa * h[hoffset - 1] + (s_tmp - rt1r) * (ab / s)) - ba * (bb / s);
          tst = h[hoffset];
          v[1] = aa * (((s_tmp + tst) - rt1r) - rt2r);
          v[2] = aa * h[hoffset + 1];
          s = (fabs(v[0]) + fabs(v[1])) + fabs(v[2]);
          v[0] /= s;
          v[1] /= s;
          v[2] /= s;
          if (m == k + 1) {
            exitg3 = true;
          } else {
            b_i = m + 6 * (m - 2);
            if (fabs(h[b_i - 1]) * (fabs(v[1]) + fabs(v[2])) <=
                2.2204460492503131E-16 * fabs(v[0]) *
                    ((fabs(h[b_i - 2]) + fabs(s_tmp)) + fabs(tst))) {
              exitg3 = true;
            } else {
              m--;
            }
          }
        }
        for (b_k = m; b_k <= i; b_k++) {
          nr = (i - b_k) + 2;
          if (3 <= nr) {
            nr = 3;
          }
          if (b_k > m) {
            hoffset = (b_k + 6 * (b_k - 2)) - 1;
            for (j = 0; j < nr; j++) {
              v[j] = h[j + hoffset];
            }
          }
          tst = v[0];
          ba = xzlarfg(nr, &tst, v);
          v[0] = tst;
          if (b_k > m) {
            h[(b_k + 6 * (b_k - 2)) - 1] = tst;
            b_i = b_k + 6 * (b_k - 2);
            h[b_i] = 0.0;
            if (b_k < i) {
              h[b_i + 1] = 0.0;
            }
          } else if (m > k + 1) {
            h[(b_k + 6 * (b_k - 2)) - 1] *= 1.0 - ba;
          }
          s = v[1];
          ab = ba * v[1];
          if (nr == 3) {
            rt1r = v[2];
            tst = ba * v[2];
            for (j = b_k; j < 7; j++) {
              sum1_tmp = b_k + 6 * (j - 1);
              aa = (h[sum1_tmp - 1] + s * h[sum1_tmp]) + rt1r * h[sum1_tmp + 1];
              h[sum1_tmp - 1] -= aa * ba;
              h[sum1_tmp] -= aa * ab;
              h[sum1_tmp + 1] -= aa * tst;
            }
            if (b_k + 3 <= i + 1) {
              b_i = b_k + 2;
            } else {
              b_i = i;
            }
            for (j = 0; j <= b_i; j++) {
              sum1_tmp = j + 6 * (b_k - 1);
              hoffset = j + 6 * b_k;
              nr = j + 6 * (b_k + 1);
              aa = (h[sum1_tmp] + s * h[hoffset]) + rt1r * h[nr];
              h[sum1_tmp] -= aa * ba;
              h[hoffset] -= aa * ab;
              h[nr] -= aa * tst;
            }
            for (j = 0; j < 6; j++) {
              sum1_tmp = j + 6 * (b_k - 1);
              hoffset = j + 6 * b_k;
              nr = j + 6 * (b_k + 1);
              aa = (z[sum1_tmp] + s * z[hoffset]) + rt1r * z[nr];
              z[sum1_tmp] -= aa * ba;
              z[hoffset] -= aa * ab;
              z[nr] -= aa * tst;
            }
          } else if (nr == 2) {
            for (j = b_k; j < 7; j++) {
              sum1_tmp = b_k + 6 * (j - 1);
              tst = h[sum1_tmp - 1];
              aa = tst + s * h[sum1_tmp];
              h[sum1_tmp - 1] = tst - aa * ba;
              h[sum1_tmp] -= aa * ab;
            }
            for (j = 0; j <= i; j++) {
              sum1_tmp = j + 6 * (b_k - 1);
              hoffset = j + 6 * b_k;
              aa = h[sum1_tmp] + s * h[hoffset];
              h[sum1_tmp] -= aa * ba;
              h[hoffset] -= aa * ab;
            }
            for (j = 0; j < 6; j++) {
              sum1_tmp = j + 6 * (b_k - 1);
              tst = z[sum1_tmp];
              hoffset = j + 6 * b_k;
              aa = tst + s * z[hoffset];
              z[sum1_tmp] = tst - aa * ba;
              z[hoffset] -= aa * ab;
            }
          }
        }
        its++;
      }
    }
    if (!goto150) {
      info = i + 1;
      exitg1 = true;
    } else {
      if ((L != i + 1) && (L == i)) {
        b_i = i + 6 * i;
        s = h[b_i - 1];
        m = 6 * (i - 1);
        hoffset = i + m;
        rt1r = h[hoffset];
        tst = h[b_i];
        xdlanv2(&h[(i + 6 * (i - 1)) - 1], &s, &rt1r, &tst, &ab, &aa, &ba, &bb,
                &s_tmp, &rt2r);
        h[b_i - 1] = s;
        h[hoffset] = rt1r;
        h[b_i] = tst;
        if (6 > i + 1) {
          hoffset = 4 - i;
          nr = i + (i + 1) * 6;
          for (k = 0; k <= hoffset; k++) {
            sum1_tmp = nr + k * 6;
            tst = h[sum1_tmp];
            ab = h[sum1_tmp - 1];
            h[sum1_tmp] = s_tmp * tst - rt2r * ab;
            h[sum1_tmp - 1] = s_tmp * ab + rt2r * tst;
          }
        }
        hoffset = i * 6;
        if (i - 1 >= 1) {
          for (k = 0; k <= i - 2; k++) {
            nr = hoffset + k;
            sum1_tmp = m + k;
            tst = s_tmp * h[sum1_tmp] + rt2r * h[nr];
            h[nr] = s_tmp * h[nr] - rt2r * h[sum1_tmp];
            h[sum1_tmp] = tst;
          }
        }
        for (k = 0; k < 6; k++) {
          nr = hoffset + k;
          tst = z[nr];
          sum1_tmp = m + k;
          ab = z[sum1_tmp];
          z[nr] = s_tmp * tst - rt2r * ab;
          z[sum1_tmp] = s_tmp * ab + rt2r * tst;
        }
      }
      i = L - 2;
    }
  }
  return info;
}

/*
 * File trailer for xdhseqr.c
 *
 * [EOF]
 */
