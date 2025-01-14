/*
 * File: xzggev.c
 *
 * MATLAB Coder version            : 5.3
 * C/C++ source code generated on  : 02-Aug-2024 17:35:13
 */

/* Include Files */
#include "xzggev.h"
#include "ellipsoid_fit_rtwutil.h"
#include "rt_nonfinite.h"
#include "xzhgeqz.h"
#include "xzlartg.h"
#include "xztgevc.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : creal_T A[36]
 *                int *info
 *                creal_T alpha1[6]
 *                creal_T beta1[6]
 *                creal_T V[36]
 * Return Type  : void
 */
void xzggev(creal_T A[36], int *info, creal_T alpha1[6], creal_T beta1[6],
            creal_T V[36])
{
  creal_T atmp;
  double a;
  double absxk;
  double anrm;
  double anrmto;
  double cfrom1;
  double cto1;
  double ctoc;
  int rscale[6];
  int A_tmp;
  int exitg2;
  int exitg3;
  int i;
  int ihi;
  int ii;
  int ilo;
  int j;
  int jcol;
  int jcolp1;
  int jrow;
  int nzcount;
  bool exitg1;
  bool exitg4;
  bool guard1 = false;
  bool ilascl;
  bool notdone;
  *info = 0;
  anrm = 0.0;
  jcol = 0;
  exitg1 = false;
  while ((!exitg1) && (jcol < 36)) {
    absxk = rt_hypotd_snf(A[jcol].re, A[jcol].im);
    if (rtIsNaN(absxk)) {
      anrm = rtNaN;
      exitg1 = true;
    } else {
      if (absxk > anrm) {
        anrm = absxk;
      }
      jcol++;
    }
  }
  if (rtIsInf(anrm) || rtIsNaN(anrm)) {
    for (i = 0; i < 6; i++) {
      alpha1[i].re = rtNaN;
      alpha1[i].im = 0.0;
      beta1[i].re = rtNaN;
      beta1[i].im = 0.0;
    }
    for (jrow = 0; jrow < 36; jrow++) {
      V[jrow].re = rtNaN;
      V[jrow].im = 0.0;
    }
  } else {
    ilascl = false;
    anrmto = anrm;
    guard1 = false;
    if ((anrm > 0.0) && (anrm < 6.7178761075670888E-139)) {
      anrmto = 6.7178761075670888E-139;
      ilascl = true;
      guard1 = true;
    } else if (anrm > 1.4885657073574029E+138) {
      anrmto = 1.4885657073574029E+138;
      ilascl = true;
      guard1 = true;
    }
    if (guard1) {
      absxk = anrm;
      ctoc = anrmto;
      notdone = true;
      while (notdone) {
        cfrom1 = absxk * 2.0041683600089728E-292;
        cto1 = ctoc / 4.9896007738368E+291;
        if ((cfrom1 > ctoc) && (ctoc != 0.0)) {
          a = 2.0041683600089728E-292;
          absxk = cfrom1;
        } else if (cto1 > absxk) {
          a = 4.9896007738368E+291;
          ctoc = cto1;
        } else {
          a = ctoc / absxk;
          notdone = false;
        }
        for (jrow = 0; jrow < 36; jrow++) {
          A[jrow].re *= a;
          A[jrow].im *= a;
        }
      }
    }
    for (i = 0; i < 6; i++) {
      rscale[i] = 1;
    }
    ilo = 1;
    ihi = 6;
    do {
      exitg3 = 0;
      i = 0;
      j = 0;
      notdone = false;
      ii = ihi;
      exitg1 = false;
      while ((!exitg1) && (ii > 0)) {
        nzcount = 0;
        i = ii;
        j = ihi;
        jcol = 0;
        exitg4 = false;
        while ((!exitg4) && (jcol <= ihi - 1)) {
          A_tmp = (ii + 6 * jcol) - 1;
          if ((A[A_tmp].re != 0.0) || (A[A_tmp].im != 0.0) ||
              (ii == jcol + 1)) {
            if (nzcount == 0) {
              j = jcol + 1;
              nzcount = 1;
              jcol++;
            } else {
              nzcount = 2;
              exitg4 = true;
            }
          } else {
            jcol++;
          }
        }
        if (nzcount < 2) {
          notdone = true;
          exitg1 = true;
        } else {
          ii--;
        }
      }
      if (!notdone) {
        exitg3 = 2;
      } else {
        if (i != ihi) {
          for (jcol = 0; jcol < 6; jcol++) {
            jcolp1 = (i + 6 * jcol) - 1;
            atmp = A[jcolp1];
            jrow = (ihi + 6 * jcol) - 1;
            A[jcolp1] = A[jrow];
            A[jrow] = atmp;
          }
        }
        if (j != ihi) {
          for (jcol = 0; jcol < ihi; jcol++) {
            jcolp1 = jcol + 6 * (j - 1);
            atmp = A[jcolp1];
            jrow = jcol + 6 * (ihi - 1);
            A[jcolp1] = A[jrow];
            A[jrow] = atmp;
          }
        }
        rscale[ihi - 1] = j;
        ihi--;
        if (ihi == 1) {
          rscale[0] = 1;
          exitg3 = 1;
        }
      }
    } while (exitg3 == 0);
    if (exitg3 != 1) {
      do {
        exitg2 = 0;
        i = 0;
        j = 0;
        notdone = false;
        jcol = ilo;
        exitg1 = false;
        while ((!exitg1) && (jcol <= ihi)) {
          nzcount = 0;
          i = ihi;
          j = jcol;
          ii = ilo;
          exitg4 = false;
          while ((!exitg4) && (ii <= ihi)) {
            A_tmp = (ii + 6 * (jcol - 1)) - 1;
            if ((A[A_tmp].re != 0.0) || (A[A_tmp].im != 0.0) || (ii == jcol)) {
              if (nzcount == 0) {
                i = ii;
                nzcount = 1;
                ii++;
              } else {
                nzcount = 2;
                exitg4 = true;
              }
            } else {
              ii++;
            }
          }
          if (nzcount < 2) {
            notdone = true;
            exitg1 = true;
          } else {
            jcol++;
          }
        }
        if (!notdone) {
          exitg2 = 1;
        } else {
          if (i != ilo) {
            for (jcol = ilo; jcol < 7; jcol++) {
              jcolp1 = 6 * (jcol - 1);
              ii = (i + jcolp1) - 1;
              atmp = A[ii];
              jrow = (ilo + jcolp1) - 1;
              A[ii] = A[jrow];
              A[jrow] = atmp;
            }
          }
          if (j != ilo) {
            for (jcol = 0; jcol < ihi; jcol++) {
              jcolp1 = jcol + 6 * (j - 1);
              atmp = A[jcolp1];
              jrow = jcol + 6 * (ilo - 1);
              A[jcolp1] = A[jrow];
              A[jrow] = atmp;
            }
          }
          rscale[ilo - 1] = j;
          ilo++;
          if (ilo == ihi) {
            rscale[ilo - 1] = ilo;
            exitg2 = 1;
          }
        }
      } while (exitg2 == 0);
    }
    memset(&V[0], 0, 36U * sizeof(creal_T));
    for (jcol = 0; jcol < 6; jcol++) {
      jrow = jcol + 6 * jcol;
      V[jrow].re = 1.0;
      V[jrow].im = 0.0;
    }
    if (ihi >= ilo + 2) {
      for (jcol = ilo - 1; jcol + 1 < ihi - 1; jcol++) {
        jcolp1 = jcol + 2;
        for (jrow = ihi - 1; jrow + 1 > jcol + 2; jrow--) {
          A_tmp = jrow + 6 * jcol;
          xzlartg(A[A_tmp - 1], A[A_tmp], &ctoc, &atmp,
                  &A[(jrow + 6 * jcol) - 1]);
          A[A_tmp].re = 0.0;
          A[A_tmp].im = 0.0;
          for (j = jcolp1; j < 7; j++) {
            nzcount = jrow + 6 * (j - 1);
            absxk = A[nzcount].im;
            cfrom1 = A[nzcount].re;
            cto1 = A[nzcount - 1].re;
            a = A[nzcount - 1].im;
            A[nzcount].re = ctoc * cfrom1 - (atmp.re * cto1 + atmp.im * a);
            A[nzcount].im =
                ctoc * A[nzcount].im - (atmp.re * a - atmp.im * cto1);
            A[nzcount - 1].re =
                ctoc * cto1 + (atmp.re * cfrom1 - atmp.im * absxk);
            A[nzcount - 1].im = ctoc * a + (atmp.re * absxk + atmp.im * cfrom1);
          }
          atmp.re = -atmp.re;
          atmp.im = -atmp.im;
          for (i = 1; i <= ihi; i++) {
            nzcount = (i + 6 * (jrow - 1)) - 1;
            absxk = A[nzcount].im;
            cfrom1 = A[nzcount].re;
            ii = (i + 6 * jrow) - 1;
            cto1 = A[ii].re;
            a = A[ii].im;
            A[nzcount].re = ctoc * cfrom1 - (atmp.re * cto1 + atmp.im * a);
            A[nzcount].im =
                ctoc * A[nzcount].im - (atmp.re * a - atmp.im * cto1);
            A[ii].re = ctoc * cto1 + (atmp.re * cfrom1 - atmp.im * absxk);
            A[ii].im = ctoc * a + (atmp.re * absxk + atmp.im * cfrom1);
          }
          for (i = 0; i < 6; i++) {
            nzcount = i + 6 * (jrow - 1);
            cfrom1 = V[nzcount].im;
            absxk = V[nzcount].re;
            ii = i + 6 * jrow;
            cto1 = V[ii].re;
            a = V[ii].im;
            V[nzcount].re = ctoc * absxk - (atmp.re * cto1 + atmp.im * a);
            V[nzcount].im = ctoc * cfrom1 - (atmp.re * a - atmp.im * cto1);
            V[ii].re = ctoc * cto1 + (atmp.re * absxk - atmp.im * cfrom1);
            V[ii].im = ctoc * a + (atmp.re * cfrom1 + atmp.im * absxk);
          }
        }
      }
    }
    xzhgeqz(A, ilo, ihi, V, info, alpha1, beta1);
    if (*info == 0) {
      xztgevc(A, V);
      if (ilo > 1) {
        for (i = ilo - 2; i + 1 >= 1; i--) {
          jcol = rscale[i] - 1;
          if (rscale[i] != i + 1) {
            for (j = 0; j < 6; j++) {
              jcolp1 = i + 6 * j;
              atmp = V[jcolp1];
              nzcount = jcol + 6 * j;
              V[jcolp1] = V[nzcount];
              V[nzcount] = atmp;
            }
          }
        }
      }
      if (ihi < 6) {
        jrow = ihi + 1;
        for (i = jrow; i < 7; i++) {
          ii = rscale[i - 1];
          if (ii != i) {
            for (j = 0; j < 6; j++) {
              jcolp1 = (i + 6 * j) - 1;
              atmp = V[jcolp1];
              nzcount = (ii + 6 * j) - 1;
              V[jcolp1] = V[nzcount];
              V[nzcount] = atmp;
            }
          }
        }
      }
      for (nzcount = 0; nzcount < 6; nzcount++) {
        absxk = fabs(V[6 * nzcount].re) + fabs(V[6 * nzcount].im);
        for (jcol = 0; jcol < 5; jcol++) {
          ii = (jcol + 6 * nzcount) + 1;
          ctoc = fabs(V[ii].re) + fabs(V[ii].im);
          if (ctoc > absxk) {
            absxk = ctoc;
          }
        }
        if (absxk >= 6.7178761075670888E-139) {
          absxk = 1.0 / absxk;
          for (jcol = 0; jcol < 6; jcol++) {
            jrow = jcol + 6 * nzcount;
            V[jrow].re *= absxk;
            V[jrow].im *= absxk;
          }
        }
      }
      if (ilascl) {
        notdone = true;
        while (notdone) {
          cfrom1 = anrmto * 2.0041683600089728E-292;
          cto1 = anrm / 4.9896007738368E+291;
          if ((cfrom1 > anrm) && (anrm != 0.0)) {
            a = 2.0041683600089728E-292;
            anrmto = cfrom1;
          } else if (cto1 > anrmto) {
            a = 4.9896007738368E+291;
            anrm = cto1;
          } else {
            a = anrm / anrmto;
            notdone = false;
          }
          for (jrow = 0; jrow < 6; jrow++) {
            alpha1[jrow].re *= a;
            alpha1[jrow].im *= a;
          }
        }
      }
    }
  }
}

/*
 * File trailer for xzggev.c
 *
 * [EOF]
 */
