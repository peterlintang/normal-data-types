/*************************************
 *
 * filename:	atom.h
 * description:	implement the atom operations
 * author:
 * date:		2019-09-27
 * version:		0.0.1
 *
 * interfaces:
 *
 *************************************/

#ifndef CI_ATOM_INCLUDED
#define CI_ATOM_INCLUDED

#include "module.h"

/* 
 * interface: atom_length
 * description:  return the length of atom 
 * args: the string of atom
 * return value: return the length of the @str atom in buckets
 */
int MODULE_FUN_NAME(atom, length)(const char *str);

/* 
 * interface: atom_new
 * description: create a new atom of @str, if the @str
 * 				is already in the atom buckets, return 
 * 				the pointer to the string of atom
 * args:	@str:	a string
 * 			@len:	the length of @str
 * return value: the pointer to the string of atom
 */
const char *MODULE_FUN_NAME(atom, new)
		(const char *str, int len);

/* 
 * interface: atom_string
 * description: create a new atom of str,
 * args: str
 * return value: the pointer to the string of atom
 */
const char *MODULE_FUN_NAME(atom, string)(const char *str);

/* 
 * interface: atom_int
 * description: convert @n to string and put it 
 * 				into the atom buckets;
 * args:	n
 * return value: the pointer point to string of n's atom
 */
const char *MODULE_FUN_NAME(atom, int)(long n);

/**** print interfaces *****/
void MODULE_FUN_NAME(atom, print)(void);


#endif
