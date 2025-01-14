/*
 * File: main.c
 *
 * MATLAB Coder version            : 5.3
 * C/C++ source code generated on  : 05-Aug-2024 11:25:10
 */

/*************************************************************************/
/* This automatically generated example C main file shows how to call    */
/* entry-point functions that MATLAB Coder generated. You must customize */
/* this file for your application. Do not modify this file directly.     */
/* Instead, make a copy of this file, modify it, and integrate it into   */
/* your development environment.                                         */
/*                                                                       */
/* This file initializes entry-point function arguments to a default     */
/* size and value before calling the entry-point functions. It does      */
/* not store or use any values returned from the entry-point functions.  */
/* If necessary, it does pre-allocate memory for returned values.        */
/* You can use this file as a starting point for a main function that    */
/* you can deploy in your application.                                   */
/*                                                                       */
/* After you copy the file, and before you deploy it, you must make the  */
/* following changes:                                                    */
/* * For variable-size function arguments, change the example sizes to   */
/* the sizes that your application requires.                             */
/* * Change the example values of function arguments to the values that  */
/* your application requires.                                            */
/* * If the entry-point functions return values, store these values or   */
/* otherwise use them as required by your application.                   */
/*                                                                       */
/*************************************************************************/

/* Include Files */
#include "main.h"
#include "ellipsoid_fit.h"
#include "ellipsoid_fit_terminate.h"
#include "ellipsoid_fit_test.h"
#include "rt_nonfinite.h"
#include <stdio.h>

/* Function Declarations */
static void argInit_1486x1_real_T(char *file, double result[1486]);

static double argInit_real_T(FILE *fp);

static void main_ellipsoid_fit(void);

static void main_ellipsoid_fit_test(void);

/* Function Definitions */
/*
 * Arguments    : double result[1486]
 * Return Type  : void
 */
static void argInit_1486x1_real_T(char *file, double result[1486])
{
  int idx0;
  /* Loop over the array to initialize each element. */
  FILE *fp = NULL;
  fp = fopen(file, "r");
  /* Loop over the array to initialize each element. */
  for (idx0 = 0; idx0 < 1486; idx0++) {
    /* Set the value of the array element.
Change this value to the value that the application requires. */
    result[idx0] = argInit_real_T(fp);
  }
  fclose(fp);
}

/*
 * Arguments    : void
 * Return Type  : double
 */
static double argInit_real_T(FILE *fp)
{
	double value = 0.0;
	char *line = NULL;
	size_t len = 0;
	ssize_t nread = 0;


	nread = getline(&line, &len, fp);
	value = strtod(line, NULL);//argInit_real_T();
	free(line);
	line = NULL;
	len = 0;
	nread = 0;

  return value;
  return 0.0;
}

/*
 * Arguments    : void
 * Return Type  : void
 */
static void main_ellipsoid_fit(void)
{
  creal_T u[10];
  double x_tmp[1486];
  /* Initialize function 'ellipsoid_fit' input arguments. */
  /* Initialize function input argument 'x'. */
//  argInit_1486x1_real_T(x_tmp);
  /* Initialize function input argument 'y'. */
  /* Initialize function input argument 'z'. */
  /* Call the entry-point 'ellipsoid_fit'. */
  ellipsoid_fit(x_tmp, x_tmp, x_tmp, u);
}

/*
 * Arguments    : void
 * Return Type  : void
 */
static void main_ellipsoid_fit_test(void)
{
  creal_T v[10];
  creal_T gain[9];
  creal_T matrix[9];
  creal_T offset[3];
  double x_m_tmp[1486];
  double y_m_tmp[1486];
  double z_m_tmp[1486];
  /* Initialize function 'ellipsoid_fit_test' input arguments. */
  /* Initialize function input argument 'x_m'. */
  argInit_1486x1_real_T("11.txt", x_m_tmp);
  /* Initialize function input argument 'y_m'. */
  argInit_1486x1_real_T("22.txt", y_m_tmp);
  /* Initialize function input argument 'z_m'. */
  argInit_1486x1_real_T("33.txt", z_m_tmp);
  /* Call the entry-point 'ellipsoid_fit_test'. */
  ellipsoid_fit_test(x_m_tmp, y_m_tmp, z_m_tmp, v, offset, gain, matrix);
  /*
  fprintf(stdout, "v:\n");
  for (int i = 0; i < 10; i++)
	  fprintf(stdout, "%f %f\n", v[i].re, v[i].im);
  fprintf(stdout, "gain:\n");
  for (int i = 0; i < 9; i++)
	  fprintf(stdout, "%f %f\n", gain[i].re, gain[i].im);
  fprintf(stdout, "matrix:\n");
  for (int i = 0; i < 9; i++)
	  fprintf(stdout, "%f %f\n", matrix[i].re, matrix[i].im);
  fprintf(stdout, "offset:\n");
  for (int i = 0; i < 3; i++)
	  fprintf(stdout, "%f %f\n", offset[i].re, offset[i].im);
	  */

  double x = 0.0;
  double y = 0.0;
  double z = 0.0;
  double x_out = 0.0;
  double y_out = 0.0;
  double z_out = 0.0;
  argInit_1486x1_real_T("11.txt", x_m_tmp);
  /* Initialize function input argument 'y_m'. */
  argInit_1486x1_real_T("22.txt", y_m_tmp);
  /* Initialize function input argument 'z_m'. */
  argInit_1486x1_real_T("33.txt", z_m_tmp);
  /* Call the entry-point 'ellipsoid_fit_test'. */
  for (int i = 0; i < 1486; i++)
  {
	  x = x_m_tmp[i] - offset[0].re;
	  y = y_m_tmp[i] - offset[1].re;
	  z = z_m_tmp[i] - offset[2].re;

	  x_out = matrix[0].re * x + matrix[0 + 3].re * y + matrix[0 + 6].re * z;
	  y_out = matrix[1].re * x + matrix[1 + 3].re * y + matrix[1 + 6].re * z;
	  z_out = matrix[2].re * x + matrix[2 + 3].re * y + matrix[2 + 6].re * z;
	  fprintf(stdout, "%f %f %f\n", x_out, y_out, z_out);
  }
}

/*
 * Arguments    : int argc
 *                char **argv
 * Return Type  : int
 */
int main(int argc, char **argv)
{
  (void)argc;
  (void)argv;
  /* The initialize function is being called automatically from your entry-point
   * function. So, a call to initialize is not included here. */
  /* Invoke the entry-point functions.
You can call entry-point functions multiple times. */
//  main_ellipsoid_fit();
  main_ellipsoid_fit_test();
  /* Terminate the application.
You do not need to do this more than one time. */
  ellipsoid_fit_terminate();
  return 0;
}

/*
 * File trailer for main.c
 *
 * [EOF]
 */
