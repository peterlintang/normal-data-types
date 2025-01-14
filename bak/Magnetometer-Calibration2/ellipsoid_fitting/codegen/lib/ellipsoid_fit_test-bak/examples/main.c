/*
 * File: main.c
 *
 * MATLAB Coder version            : 5.3
 * C/C++ source code generated on  : 02-Aug-2024 09:16:34
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
#include "ellipsoid_fit_test.h"
#include "ellipsoid_fit_test_emxAPI.h"
#include "ellipsoid_fit_test_terminate.h"
#include "ellipsoid_fit_test_types.h"
#include "rt_nonfinite.h"
#include <stdio.h>

/* Function Declarations */
static emxArray_real_T *argInit_Unboundedx1_real_T(char *file);

static double argInit_real_T(FILE *fp);

static void main_ellipsoid_fit_test(void);

/* Function Definitions */
/*
 * Arguments    : void
 * Return Type  : emxArray_real_T *
 */
static emxArray_real_T *argInit_Unboundedx1_real_T(char *file)
{
  emxArray_real_T *result;
  double *result_data;
  const int i = 1486;
  int idx0;
  /* Set the size of the array.
Change this size to the value that the application requires. */
  result = emxCreateND_real_T(1, &i);
  result_data = result->data;
  /* Loop over the array to initialize each element. */
  FILE *fp = NULL;
  fp = fopen(file, "r");
  for (idx0 = 0; idx0 < result->size[0U]; idx0++) {
    /* Set the value of the array element.
Change this value to the value that the application requires. */
    result_data[idx0] = argInit_real_T(fp);
	printf("i: %d, value: %f\n", idx0, result_data[idx0]);
  }
  fclose(fp);
  return result;
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
}

/*
 * Arguments    : void
 * Return Type  : void
 */
static void main_ellipsoid_fit_test(void)
{
  emxArray_real_T *x_m;
  emxArray_real_T *y_m;
  emxArray_real_T *z_m;
  creal_T v[10];
  creal_T gain[9];
  creal_T matrix[9];
  creal_T offset[3];
  for (int i = 0; i < 10; i++)
  {
	  v[i].re = 0.0;
	  v[i].im = 0.0;
  }
  for (int i = 0; i < 9; i++)
  {
	  gain[i].re = 0.0;
	  gain[i].im = 0.0;
  }
  for (int i = 0; i < 9; i++)
  {
	  matrix[i].re = 0.0;
	  matrix[i].im = 0.0;
  }
  for (int i = 0; i < 3; i++)
  {
	  offset[i].re = 0.0;
	  offset[i].im = 0.0;
  }

  /* Initialize function 'ellipsoid_fit_test' input arguments. */
  /* Initialize function input argument 'x_m'. */
  fprintf(stdout, "x value:\n");
  x_m = argInit_Unboundedx1_real_T("11.txt");
  /* Initialize function input argument 'y_m'. */
  fprintf(stdout, "y value:\n");
  y_m = argInit_Unboundedx1_real_T("22.txt");
  /* Initialize function input argument 'z_m'. */
  fprintf(stdout, "z value:\n");
  z_m = argInit_Unboundedx1_real_T("33.txt");
  /* Call the entry-point 'ellipsoid_fit_test'. */
  ellipsoid_fit_test(x_m, y_m, z_m, v, offset, gain, matrix);
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
  emxDestroyArray_real_T(z_m);
  emxDestroyArray_real_T(y_m);
  emxDestroyArray_real_T(x_m);
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
  main_ellipsoid_fit_test();
  /* Terminate the application.
You do not need to do this more than one time. */
  ellipsoid_fit_test_terminate();
  return 0;
}

/*
 * File trailer for main.c
 *
 * [EOF]
 */
