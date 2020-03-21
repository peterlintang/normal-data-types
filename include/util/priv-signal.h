/**************************************************************************
 *
 * Copyright: Copyright (c) 2018
 * All rights reserved. 
 *
 * Create On: 	2019-02-14
 * Filename: 	priv-signal.h
 * Description:	process signal 
 * Author: 		xiaojie.lin
 * Modified by:
 *
 **************************************************************************/


#ifndef __MOZI_SIGNAL_H__
#define __MOZI_SIGNAL_H__

/***********************************************************************
 *
 * NOTICE should be at the first of main before any pthreads created
 * description: block signals and create a thread to handle signals
 *
 **********************************************************************/
int mozi_signal_init(void);

#endif // end of __MOZI_SIGNAL_H__



