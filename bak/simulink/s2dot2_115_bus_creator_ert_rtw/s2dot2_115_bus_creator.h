/*
 * File: s2dot2_115_bus_creator.h
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

#ifndef RTW_HEADER_s2dot2_115_bus_creator_h_
#define RTW_HEADER_s2dot2_115_bus_creator_h_
#ifndef s2dot2_115_bus_creator_COMMON_INCLUDES_
#define s2dot2_115_bus_creator_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                             /* s2dot2_115_bus_creator_COMMON_INCLUDES_ */

#include "s2dot2_115_bus_creator_types.h"
#include "a.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

/* External inputs (root inport signals with default storage) */
typedef struct {
  real_T In1;                          /* '<Root>/In1' */
  real_T In2;                          /* '<Root>/In2' */
} ExtU_s2dot2_115_bus_creator_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  a Out1;                              /* '<Root>/Out1' */
} ExtY_s2dot2_115_bus_creator_T;

/* Real-time Model Data Structure */
struct tag_RTM_s2dot2_115_bus_creato_T {
  const char_T * volatile errorStatus;
};

/* External inputs (root inport signals with default storage) */
extern ExtU_s2dot2_115_bus_creator_T s2dot2_115_bus_creator_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_s2dot2_115_bus_creator_T s2dot2_115_bus_creator_Y;

/* Model entry point functions */
extern void s2dot2_115_bus_creator_initialize(void);
extern void s2dot2_115_bus_creator_step(void);
extern void s2dot2_115_bus_creator_terminate(void);

/* Real-time Model object */
extern RT_MODEL_s2dot2_115_bus_creat_T *const s2dot2_115_bus_creator_M;

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 's2dot2_115_bus_creator'
 */
#endif                                /* RTW_HEADER_s2dot2_115_bus_creator_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
