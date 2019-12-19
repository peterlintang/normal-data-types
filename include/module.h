
#ifndef __PRIV_MODULE_H__
#define __PRIV_MODULE_H__

#define MODULE	priv

#define CONN2(mod, name)	mod##_##name
#define CONN3(mod, type, name) mod##_##type##_##name

#define MODULE_TYPE_NAME_FUN(mod, type, name)	CONN3(mod, type, name)

/* for outside useage , in function definition, should use the MODULE_FUN_NAME too, and we can only modify MODULE to rename functions' names */
#define MODULE_FUN_NAME(type, name)	MODULE_TYPE_NAME_FUN(MODULE, type, name)

#endif // end of __PRIV_MODULE_H__
