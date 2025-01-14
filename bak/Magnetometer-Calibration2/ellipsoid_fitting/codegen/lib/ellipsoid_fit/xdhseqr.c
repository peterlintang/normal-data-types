/*
 * File: xdhseqr.c
 *
 * MATLAB Coder version            : 5.3
 * C/C++ source code generated on  : 02-Aug-2024 17:35:13
 */

/* Include Files */
#include "xdhseqr.h"
#include "ellipsoid_fit_rtwutil.h"
#include "rt_nonfinite.h"
#include "xdlanv2.h"
#include "xnrm2.h"
#include "xrot.h"
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
  int b_k;
  int c_k;
  int hoffset;
  int i;
  int info;
  int its;
  int j;
  int k;
  int knt;
  int m;
  int nr;
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
        b_k = k + 6 * (k - 1);
        ba = fabs(h[b_k]);
        if (ba <= 6.0125050800269183E-292) {
          exitg3 = true;
        } else {
          hoffset = k + 6 * k;
          bb = fabs(h[hoffset]);
          tst = fabs(h[b_k - 1]) + bb;
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
            tst = fabs(h[b_k - 1] - h[hoffset]);
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
            b_k = m + 6 * (m - 2);
            if (fabs(h[b_k - 1]) * (fabs(v[1]) + fabs(v[2])) <=
                2.2204460492503131E-16 * fabs(v[0]) *
                    ((fabs(h[b_k - 2]) + fabs(s_tmp)) + fabs(tst))) {
              exitg3 = true;
            } else {
              m--;
            }
          }
        }
        for (c_k = m; c_k <= i; c_k++) {
          nr = (i - c_k) + 2;
          if (3 <= nr) {
            nr = 3;
          }
          if (c_k > m) {
            hoffset = (c_k + 6 * (c_k - 2)) - 1;
            for (j = 0; j < nr; j++) {
              v[j] = h[j + hoffset];
            }
          }
          ab = v[0];
          bb = 0.0;
          if (nr > 0) {
            tst = b_xnrm2(nr - 1, v);
            if (tst != 0.0) {
              aa = rt_hypotd_snf(v[0], tst);
              if (v[0] >= 0.0) {
                aa = -aa;
              }
              if (fabs(aa) < 1.0020841800044864E-292) {
                knt = -1;
                do {
                  knt++;
                  for (b_k = 2; b_k <= nr; b_k++) {
                    v[b_k - 1] *= 9.9792015476736E+291;
                  }
                  aa *= 9.9792015476736E+291;
                  ab *= 9.9792015476736E+291;
                } while (!(fabs(aa) >= 1.0020841800044864E-292));
                aa = rt_hypotd_snf(ab, b_xnrm2(nr - 1, v));
                if (ab >= 0.0) {
                  aa = -aa;
                }
                bb = (aa - ab) / aa;
                tst = 1.0 / (ab - aa);
                for (b_k = 2; b_k <= nr; b_k++) {
                  v[b_k - 1] *= tst;
                }
                for (b_k = 0; b_k <= knt; b_k++) {
                  aa *= 1.0020841800044864E-292;
                }
                ab = aa;
              } else {
                bb = (aa - v[0]) / aa;
                tst = 1.0 / (v[0] - aa);
                for (b_k = 2; b_k <= nr; b_k++) {
                  v[b_k - 1] *= tst;
                }
                ab = aa;
              }
            }
          }
          v[0] = ab;
          if (c_k > m) {
            h[(c_k + 6 * (c_k - 2)) - 1] = ab;
            b_k = c_k + 6 * (c_k - 2);
            h[b_k] = 0.0;
            if (c_k < i) {
              h[b_k + 1] = 0.0;
            }
          } else if (m > k + 1) {
            h[(c_k + 6 * (c_k - 2)) - 1] *= 1.0 - bb;
          }
          rt1r = v[1];
          tst = bb * v[1];
          if (nr == 3) {
            s = v[2];
            ba = bb * v[2];
            for (j = c_k; j < 7; j++) {
              hoffset = c_k + 6 * (j - 1);
              aa = (h[hoffset - 1] + rt1r * h[hoffset]) + s * h[hoffset + 1];
              h[hoffset - 1] -= aa * bb;
              h[hoffset] -= aa * tst;
              h[hoffset + 1] -= aa * ba;
            }
            if (c_k + 3 <= i + 1) {
              b_k = c_k + 2;
            } else {
              b_k = i;
            }
            for (j = 0; j <= b_k; j++) {
              hoffset = j + 6 * (c_k - 1);
              knt = j + 6 * c_k;
              nr = j + 6 * (c_k + 1);
              aa = (h[hoffset] + rt1r * h[knt]) + s * h[nr];
              h[hoffset] -= aa * bb;
              h[knt] -= aa * tst;
              h[nr] -= aa * ba;
            }
            for (j = 0; j < 6; j++) {
              hoffset = j + 6 * (c_k - 1);
              ab = z[hoffset];
              knt = j + 6 * c_k;
              nr = j + 6 * (c_k + 1);
              aa = (ab + rt1r * z[knt]) + s * z[nr];
              z[hoffset] = ab - aa * bb;
              z[knt] -= aa * tst;
              z[nr] -= aa * ba;
            }
          } else if (nr == 2) {
            for (j = c_k; j < 7; j++) {
              hoffset = c_k + 6 * (j - 1);
              ab = h[hoffset - 1];
              aa = ab + rt1r * h[hoffset];
              h[hoffset - 1] = ab - aa * bb;
              h[hoffset] -= aa * tst;
            }
            for (j = 0; j <= i; j++) {
              hoffset = j + 6 * (c_k - 1);
              knt = j + 6 * c_k;
              aa = h[hoffset] + rt1r * h[knt];
              h[hoffset] -= aa * bb;
              h[knt] -= aa * tst;
            }
            for (j = 0; j < 6; j++) {
              hoffset = j + 6 * (c_k - 1);
              ab = z[hoffset];
              knt = j + 6 * c_k;
              aa = ab + rt1r * z[knt];
              z[hoffset] = ab - aa * bb;
              z[knt] -= aa * tst;
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
        b_k = i + 6 * i;
        rt1r = h[b_k - 1];
        nr = 6 * (i - 1);
        hoffset = i + nr;
        s = h[hoffset];
        tst = h[b_k];
        xdlanv2(&h[(i + 6 * (i - 1)) - 1], &rt1r, &s, &tst, &ab, &aa, &ba, &bb,
                &s_tmp, &rt2r);
        h[b_k - 1] = rt1r;
        h[hoffset] = s;
        h[b_k] = tst;
        if (6 > i + 1) {
          hoffset = 4 - i;
          knt = i + (i + 1) * 6;
          for (k = 0; k <= hoffset; k++) {
            b_k = knt + k * 6;
            tst = h[b_k];
            ab = h[b_k - 1];
            h[b_k] = s_tmp * tst - rt2r * ab;
            h[b_k - 1] = s_tmp * ab + rt2r * tst;
          }
        }
        hoffset = i * 6 + 1;
        xrot(i - 1, h, nr + 1, hoffset, s_tmp, rt2r);
        b_xrot(z, nr + 1, hoffset, s_tmp, rt2r);
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
