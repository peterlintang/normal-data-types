/*
 * File: s2dot2_115_bus_creator.c
 *
 * Code generated for Simulink model 's2dot2_115_bus_creator'.
 *
 * Model version                  : 1.1
 * Simulink Coder version         : 23.2 (R2023b) 01-Aug-2023
 * C/C++ source code generated on : Thu Nov 28 11:33:33 2024
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "s2dot2_115_bus_creator.h"

/* External inputs (root inport signals with default storage) */
ExtU_s2dot2_115_bus_creator_T s2dot2_115_bus_creator_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_s2dot2_115_bus_creator_T s2dot2_115_bus_creator_Y;

/* Real-time model */
static RT_MODEL_s2dot2_115_bus_creat_T s2dot2_115_bus_creator_M_;
RT_MODEL_s2dot2_115_bus_creat_T *const s2dot2_115_bus_creator_M =
  &s2dot2_115_bus_creator_M_;

/* Model step function */
void s2dot2_115_bus_creator_step(void)
{
  /* BusCreator: '<Root>/Bus Creator' incorporates:
   *  Inport: '<Root>/In1'
   *  Inport: '<Root>/In2'
   *  Outport: '<Root>/Out1'
   */
  s2dot2_115_bus_creator_Y.Out1.a1 = s2dot2_115_bus_creator_U.In1;
  s2dot2_115_bus_creator_Y.Out1.a2 = s2dot2_115_bus_creator_U.In2;
}

/* Model initialize function */
void s2dot2_115_bus_creator_initialize(void)
{
  /* (no initialization code required) */
}

/* Model terminate function */
void s2dot2_115_bus_creator_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
