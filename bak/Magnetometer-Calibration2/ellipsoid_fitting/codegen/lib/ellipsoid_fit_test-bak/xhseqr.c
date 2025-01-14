/*
 * File: xhseqr.c
 *
 * MATLAB Coder version            : 5.3
 * C/C++ source code generated on  : 02-Aug-2024 10:19:14
 */

/* Include Files */
#include "xhseqr.h"
#include "ellipsoid_fit_test_data.h"
#include "ellipsoid_fit_test_rtwutil.h"
#include "recip.h"
#include "rt_nonfinite.h"
#include "sqrt.h"
#include "xdlapy3.h"
#include <math.h>

/* Function Definitions */
/*
 * Arguments    : creal_T h[9]
 *                creal_T z[9]
 * Return Type  : int
 */
int xhseqr(creal_T h[9], creal_T z[9])
{
  creal_T b_v[2];
  creal_T sc;
  creal_T v;
  double aa;
  double ba;
  double bb;
  double br;
  double htmp2;
  double im;
  double s;
  double t_im;
  double t_re;
  double tst;
  double tst_tmp_tmp;
  double u_im;
  double u_re;
  int L;
  int b_k;
  int h_tmp;
  int i;
  int info;
  int its;
  int k;
  int knt;
  int m;
  int sc_tmp;
  bool exitg1;
  bool exitg2;
  bool exitg3;
  bool goto140;
  bool goto70;
  info = 0;
  h[2].re = 0.0;
  h[2].im = 0.0;
  for (i = 0; i < 2; i++) {
    sc_tmp = (i + 3 * i) + 1;
    if (h[sc_tmp].im != 0.0) {
      aa = h[sc_tmp].re;
      tst = h[sc_tmp].im;
      br = fabs(aa) + fabs(h[sc_tmp].im);
      if (tst == 0.0) {
        sc.re = aa / br;
        sc.im = 0.0;
      } else if (aa == 0.0) {
        sc.re = 0.0;
        sc.im = tst / br;
      } else {
        sc.re = aa / br;
        sc.im = tst / br;
      }
      br = rt_hypotd_snf(sc.re, sc.im);
      if (-sc.im == 0.0) {
        bb = sc.re / br;
        im = 0.0;
      } else if (sc.re == 0.0) {
        bb = 0.0;
        im = -sc.im / br;
      } else {
        bb = sc.re / br;
        im = -sc.im / br;
      }
      h[sc_tmp].re = rt_hypotd_snf(aa, h[sc_tmp].im);
      h[sc_tmp].im = 0.0;
      h_tmp = (i + 1) * 3 + 3;
      knt = (i + h_tmp) - 1;
      sc_tmp = knt + 3 * (1 - i);
      for (k = knt; k <= sc_tmp; k += 3) {
        tst = h[k - 1].im;
        aa = h[k - 1].re;
        h[k - 1].re = bb * aa - im * tst;
        h[k - 1].im = bb * tst + im * aa;
      }
      knt = h_tmp - 2;
      for (k = knt; k <= h_tmp; k++) {
        tst = h[k - 1].im;
        aa = h[k - 1].re;
        h[k - 1].re = bb * aa - -im * tst;
        h[k - 1].im = bb * tst + -im * aa;
        tst = z[k - 1].im;
        aa = z[k - 1].re;
        z[k - 1].re = bb * aa - -im * tst;
        z[k - 1].im = bb * tst + -im * aa;
      }
    }
  }
  i = 2;
  exitg1 = false;
  while ((!exitg1) && (i + 1 >= 1)) {
    L = -1;
    goto140 = false;
    its = 0;
    exitg2 = false;
    while ((!exitg2) && (its < 301)) {
      k = i;
      exitg3 = false;
      while ((!exitg3) && (k + 1 > L + 2)) {
        sc_tmp = k + 3 * (k - 1);
        aa = fabs(h[sc_tmp].re);
        ba = aa + fabs(h[sc_tmp].im);
        if (ba <= 3.0062525400134592E-292) {
          exitg3 = true;
        } else {
          knt = k + 3 * k;
          tst_tmp_tmp = h[knt].re;
          t_im = h[knt].im;
          bb = fabs(tst_tmp_tmp) + fabs(t_im);
          br = h[sc_tmp - 1].re;
          t_re = h[sc_tmp - 1].im;
          tst = (fabs(br) + fabs(t_re)) + bb;
          if (tst == 0.0) {
            if (k - 1 >= 1) {
              tst = fabs(h[(k + 3 * (k - 2)) - 1].re);
            }
            if (k + 2 <= 3) {
              tst += fabs(h[knt + 1].re);
            }
          }
          if (aa <= 2.2204460492503131E-16 * tst) {
            htmp2 = fabs(h[knt - 1].re) + fabs(h[knt - 1].im);
            if (ba > htmp2) {
              tst = ba;
              ba = htmp2;
            } else {
              tst = htmp2;
            }
            htmp2 = fabs(br - tst_tmp_tmp) + fabs(t_re - t_im);
            if (bb > htmp2) {
              aa = bb;
              bb = htmp2;
            } else {
              aa = htmp2;
            }
            s = aa + tst;
            if (ba * (tst / s) <=
                fmax(3.0062525400134592E-292,
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
      L = k - 1;
      if (k + 1 > 1) {
        sc_tmp = k + 3 * (k - 1);
        h[sc_tmp].re = 0.0;
        h[sc_tmp].im = 0.0;
      }
      if (k + 1 >= i + 1) {
        goto140 = true;
        exitg2 = true;
      } else {
        if (its == 10) {
          knt = k + 3 * k;
          t_re = 0.75 * fabs(h[(k + 3 * k) + 1].re) + h[knt].re;
          t_im = h[knt].im;
        } else if (its == 20) {
          knt = i + 3 * i;
          t_re = 0.75 * fabs(h[i + 3 * (i - 1)].re) + h[knt].re;
          t_im = h[knt].im;
        } else {
          knt = i + 3 * i;
          t_re = h[knt].re;
          t_im = h[knt].im;
          v = h[knt - 1];
          b_sqrt(&v);
          sc_tmp = i + 3 * (i - 1);
          sc = h[sc_tmp];
          b_sqrt(&sc);
          u_re = v.re * sc.re - v.im * sc.im;
          u_im = v.re * sc.im + v.im * sc.re;
          s = fabs(u_re) + fabs(u_im);
          if (s != 0.0) {
            ba = 0.5 * (h[sc_tmp - 1].re - t_re);
            im = 0.5 * (h[sc_tmp - 1].im - t_im);
            tst_tmp_tmp = fabs(ba) + fabs(im);
            s = fmax(s, tst_tmp_tmp);
            if (im == 0.0) {
              t_re = ba / s;
              t_im = 0.0;
            } else if (ba == 0.0) {
              t_re = 0.0;
              t_im = im / s;
            } else {
              t_re = ba / s;
              t_im = im / s;
            }
            bb = t_re * t_re - t_im * t_im;
            aa = t_re * t_im;
            if (u_im == 0.0) {
              sc.re = u_re / s;
              sc.im = 0.0;
            } else if (u_re == 0.0) {
              sc.re = 0.0;
              sc.im = u_im / s;
            } else {
              sc.re = u_re / s;
              sc.im = u_im / s;
            }
            tst = sc.re * sc.re - sc.im * sc.im;
            htmp2 = sc.re * sc.im;
            v.re = bb + tst;
            v.im = (aa + aa) + (htmp2 + htmp2);
            b_sqrt(&v);
            sc.re = s * v.re;
            sc.im = s * v.im;
            if (tst_tmp_tmp > 0.0) {
              if (im == 0.0) {
                t_re = ba / tst_tmp_tmp;
                t_im = 0.0;
              } else if (ba == 0.0) {
                t_re = 0.0;
                t_im = im / tst_tmp_tmp;
              } else {
                t_re = ba / tst_tmp_tmp;
                t_im = im / tst_tmp_tmp;
              }
              if (t_re * sc.re + t_im * sc.im < 0.0) {
                sc.re = -sc.re;
                sc.im = -sc.im;
              }
            }
            br = ba + sc.re;
            htmp2 = im + sc.im;
            if (htmp2 == 0.0) {
              if (u_im == 0.0) {
                t_im = u_re / br;
                tst = 0.0;
              } else if (u_re == 0.0) {
                t_im = 0.0;
                tst = u_im / br;
              } else {
                t_im = u_re / br;
                tst = u_im / br;
              }
            } else if (br == 0.0) {
              if (u_re == 0.0) {
                t_im = u_im / htmp2;
                tst = 0.0;
              } else if (u_im == 0.0) {
                t_im = 0.0;
                tst = -(u_re / htmp2);
              } else {
                t_im = u_im / htmp2;
                tst = -(u_re / htmp2);
              }
            } else {
              tst_tmp_tmp = fabs(br);
              tst = fabs(htmp2);
              if (tst_tmp_tmp > tst) {
                s = htmp2 / br;
                tst = br + s * htmp2;
                t_im = (u_re + s * u_im) / tst;
                tst = (u_im - s * u_re) / tst;
              } else if (tst == tst_tmp_tmp) {
                if (br > 0.0) {
                  aa = 0.5;
                } else {
                  aa = -0.5;
                }
                if (htmp2 > 0.0) {
                  tst = 0.5;
                } else {
                  tst = -0.5;
                }
                t_im = (u_re * aa + u_im * tst) / tst_tmp_tmp;
                tst = (u_im * aa - u_re * tst) / tst_tmp_tmp;
              } else {
                s = br / htmp2;
                tst = htmp2 + s * br;
                t_im = (s * u_re + u_im) / tst;
                tst = (s * u_im - u_re) / tst;
              }
            }
            t_re = h[knt].re - (u_re * t_im - u_im * tst);
            t_im = h[knt].im - (u_re * tst + u_im * t_im);
          }
        }
        goto70 = false;
        m = i;
        exitg3 = false;
        while ((!exitg3) && (m > k + 1)) {
          sc.re = h[4].re - t_re;
          sc.im = h[4].im - t_im;
          tst = h[5].re;
          s = (fabs(sc.re) + fabs(sc.im)) + fabs(tst);
          if (sc.im == 0.0) {
            bb = sc.re / s;
            im = 0.0;
          } else if (sc.re == 0.0) {
            bb = 0.0;
            im = sc.im / s;
          } else {
            bb = sc.re / s;
            im = sc.im / s;
          }
          sc.re = bb;
          sc.im = im;
          tst /= s;
          b_v[0] = sc;
          b_v[1].re = tst;
          b_v[1].im = 0.0;
          if (fabs(h[1].re) * fabs(tst) <=
              2.2204460492503131E-16 *
                  ((fabs(bb) + fabs(im)) * ((fabs(h[4].re) + fabs(h[4].im)) +
                                            (fabs(h[8].re) + fabs(h[8].im))))) {
            goto70 = true;
            exitg3 = true;
          } else {
            m = 1;
          }
        }
        if (!goto70) {
          sc_tmp = k + 3 * k;
          sc.re = h[sc_tmp].re - t_re;
          sc.im = h[sc_tmp].im - t_im;
          tst = h[(k + 3 * k) + 1].re;
          s = (fabs(sc.re) + fabs(sc.im)) + fabs(tst);
          if (sc.im == 0.0) {
            b_v[0].re = sc.re / s;
            b_v[0].im = 0.0;
          } else if (sc.re == 0.0) {
            b_v[0].re = 0.0;
            b_v[0].im = sc.im / s;
          } else {
            b_v[0].re = sc.re / s;
            b_v[0].im = sc.im / s;
          }
          tst /= s;
          b_v[1].re = tst;
          b_v[1].im = 0.0;
        }
        for (b_k = m; b_k <= i; b_k++) {
          if (b_k > m) {
            b_v[0] = h[1];
            b_v[1] = h[2];
          }
          sc = b_v[0];
          ba = b_v[1].re;
          im = b_v[1].im;
          t_re = 0.0;
          t_im = 0.0;
          tst = rt_hypotd_snf(b_v[1].re, b_v[1].im);
          if ((tst != 0.0) || (b_v[0].im != 0.0)) {
            aa = xdlapy3(b_v[0].re, b_v[0].im, tst);
            if (b_v[0].re >= 0.0) {
              aa = -aa;
            }
            if (fabs(aa) < 1.0020841800044864E-292) {
              knt = -1;
              do {
                knt++;
                ba *= 9.9792015476736E+291;
                im *= 9.9792015476736E+291;
                aa *= 9.9792015476736E+291;
                sc.re *= 9.9792015476736E+291;
                sc.im *= 9.9792015476736E+291;
              } while (!(fabs(aa) >= 1.0020841800044864E-292));
              aa = xdlapy3(sc.re, sc.im, rt_hypotd_snf(ba, im));
              if (sc.re >= 0.0) {
                aa = -aa;
              }
              tst = aa - sc.re;
              if (0.0 - sc.im == 0.0) {
                t_re = tst / aa;
                t_im = 0.0;
              } else if (tst == 0.0) {
                t_re = 0.0;
                t_im = (0.0 - sc.im) / aa;
              } else {
                t_re = tst / aa;
                t_im = (0.0 - sc.im) / aa;
              }
              v.re = sc.re - aa;
              v.im = sc.im;
              sc = recip(v);
              bb = sc.re * ba - sc.im * im;
              im = sc.re * im + sc.im * ba;
              ba = bb;
              for (h_tmp = 0; h_tmp <= knt; h_tmp++) {
                aa *= 1.0020841800044864E-292;
              }
              sc.re = aa;
              sc.im = 0.0;
            } else {
              tst = aa - b_v[0].re;
              if (0.0 - b_v[0].im == 0.0) {
                t_re = tst / aa;
                t_im = 0.0;
              } else if (tst == 0.0) {
                t_re = 0.0;
                t_im = (0.0 - b_v[0].im) / aa;
              } else {
                t_re = tst / aa;
                t_im = (0.0 - b_v[0].im) / aa;
              }
              v.re = b_v[0].re - aa;
              v.im = b_v[0].im;
              v = recip(v);
              ba = v.re * b_v[1].re - v.im * b_v[1].im;
              im = v.re * b_v[1].im + v.im * b_v[1].re;
              sc.re = aa;
              sc.im = 0.0;
            }
          }
          b_v[0] = sc;
          b_v[1].re = ba;
          b_v[1].im = im;
          if (b_k > m) {
            h[1] = sc;
            h[2].re = 0.0;
            h[2].im = 0.0;
          }
          aa = t_re * ba - t_im * im;
          for (h_tmp = b_k; h_tmp < 4; h_tmp++) {
            knt = b_k + 3 * (h_tmp - 1);
            tst = h[knt - 1].im;
            htmp2 = h[knt - 1].re;
            sc.re = (t_re * htmp2 - -t_im * tst) + aa * h[knt].re;
            sc.im = (t_re * tst + -t_im * htmp2) + aa * h[knt].im;
            h[knt - 1].re = htmp2 - sc.re;
            h[knt - 1].im -= sc.im;
            h[knt].re -= sc.re * ba - sc.im * im;
            h[knt].im -= sc.re * im + sc.im * ba;
          }
          for (h_tmp = 0; h_tmp <= i; h_tmp++) {
            knt = h_tmp + 3 * (b_k - 1);
            tst = h[knt].im;
            htmp2 = h[knt].re;
            sc_tmp = h_tmp + 3 * b_k;
            sc.re = (t_re * htmp2 - t_im * tst) + aa * h[sc_tmp].re;
            sc.im = (t_re * tst + t_im * htmp2) + aa * h[sc_tmp].im;
            h[knt].re = htmp2 - sc.re;
            h[knt].im -= sc.im;
            h[sc_tmp].re -= sc.re * ba - sc.im * -im;
            h[sc_tmp].im -= sc.re * -im + sc.im * ba;
          }
          knt = 3 * (b_k - 1);
          tst = z[knt].im;
          htmp2 = z[knt].re;
          sc.re = (t_re * htmp2 - t_im * tst) + aa * z[3 * b_k].re;
          sc.im = (t_re * tst + t_im * htmp2) + aa * z[3 * b_k].im;
          z[knt].re = htmp2 - sc.re;
          z[knt].im -= sc.im;
          z[3 * b_k].re -= sc.re * ba - sc.im * -im;
          z[3 * b_k].im -= sc.re * -im + sc.im * ba;
          tst = z[knt + 1].im;
          htmp2 = z[knt + 1].re;
          sc_tmp = 3 * b_k + 1;
          sc.re = (t_re * htmp2 - t_im * tst) + aa * z[sc_tmp].re;
          sc.im = (t_re * tst + t_im * htmp2) + aa * z[sc_tmp].im;
          z[knt + 1].re = htmp2 - sc.re;
          z[knt + 1].im -= sc.im;
          z[sc_tmp].re -= sc.re * ba - sc.im * -im;
          z[sc_tmp].im -= sc.re * -im + sc.im * ba;
          tst = z[knt + 2].im;
          htmp2 = z[knt + 2].re;
          sc_tmp = 3 * b_k + 2;
          sc.re = (t_re * htmp2 - t_im * tst) + aa * z[sc_tmp].re;
          sc.im = (t_re * tst + t_im * htmp2) + aa * z[sc_tmp].im;
          z[knt + 2].re = htmp2 - sc.re;
          z[knt + 2].im -= sc.im;
          z[sc_tmp].re -= sc.re * ba - sc.im * -im;
          z[sc_tmp].im -= sc.re * -im + sc.im * ba;
          if ((b_k == m) && (m > k + 1)) {
            br = rt_hypotd_snf(1.0 - t_re, 0.0 - t_im);
            if (0.0 - t_im == 0.0) {
              bb = (1.0 - t_re) / br;
              im = 0.0;
            } else if (1.0 - t_re == 0.0) {
              bb = 0.0;
              im = (0.0 - t_im) / br;
            } else {
              bb = (1.0 - t_re) / br;
              im = (0.0 - t_im) / br;
            }
            tst = h[5].re * -im + h[5].im * bb;
            h[5].re = h[5].re * bb - h[5].im * -im;
            h[5].im = tst;
            for (h_tmp = 2; h_tmp <= i + 1; h_tmp++) {
              if (h_tmp != 3) {
                tst = bb * h[7].im + im * h[7].re;
                h[7].re = bb * h[7].re - im * h[7].im;
                h[7].im = tst;
                tst = bb * h[3].im + -im * h[3].re;
                h[3].re = bb * h[3].re - -im * h[3].im;
                h[3].im = tst;
                tst = bb * z[3].im + -im * z[3].re;
                z[3].re = bb * z[3].re - -im * z[3].im;
                z[3].im = tst;
                tst = bb * z[4].im + -im * z[4].re;
                z[4].re = bb * z[4].re - -im * z[4].im;
                z[4].im = tst;
                tst = bb * z[5].im + -im * z[5].re;
                z[5].re = bb * z[5].re - -im * z[5].im;
                z[5].im = tst;
              }
            }
          }
        }
        sc_tmp = i + 3 * (i - 1);
        sc = h[sc_tmp];
        if (h[sc_tmp].im != 0.0) {
          tst = rt_hypotd_snf(h[sc_tmp].re, h[sc_tmp].im);
          h[sc_tmp].re = tst;
          h[sc_tmp].im = 0.0;
          if (sc.im == 0.0) {
            bb = sc.re / tst;
            im = 0.0;
          } else if (sc.re == 0.0) {
            bb = 0.0;
            im = sc.im / tst;
          } else {
            bb = sc.re / tst;
            im = sc.im / tst;
          }
          if (3 > i + 1) {
            tst = bb * h[7].im + -im * h[7].re;
            h[7].re = bb * h[7].re - -im * h[7].im;
            h[7].im = tst;
          }
          knt = i * 3;
          h_tmp = i * 3 + 1;
          sc_tmp = knt + i;
          for (k = h_tmp; k <= sc_tmp; k++) {
            tst = h[k - 1].im;
            aa = h[k - 1].re;
            h[k - 1].re = bb * aa - im * tst;
            h[k - 1].im = bb * tst + im * aa;
          }
          sc_tmp = knt + 3;
          for (k = h_tmp; k <= sc_tmp; k++) {
            tst = z[k - 1].im;
            aa = z[k - 1].re;
            z[k - 1].re = bb * aa - im * tst;
            z[k - 1].im = bb * tst + im * aa;
          }
        }
        its++;
      }
    }
    if (!goto140) {
      info = i + 1;
      exitg1 = true;
    } else {
      i = L;
    }
  }
  return info;
}

/*
 * File trailer for xhseqr.c
 *
 * [EOF]
 */
