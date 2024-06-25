/*
 * A simple kernel FIFO implementation.
 *
 * Copyright (C) 2004 Stelian Pop <stelian@popies.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */
#ifndef AES_HEADER_H__
#define AES_HEADER_H__

#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char AES_U8;
typedef unsigned int  AES_U16;

#define AES_LENGTH        16

#define CODE_MEM_TYP  code

#define Nc (4)          
#define Nr (10)         


//extern AES_U8 encrypt_key[16];


/*_____ D E F I N I T I O N __________________________________________________*/

/*_____ D E C L A R A T I O N ________________________________________________*/

void AES_ExpandKey(AES_U8 *key, AES_U8 *expKey);
void AES_Encrypt(AES_U8 *in, AES_U8 *expKey, AES_U8 *out);
void AES_Decrypt(AES_U8 *in, AES_U8 *expKey, AES_U8 *out);
int AES_encrypt_memory(unsigned char *key, unsigned char *ptr_in, unsigned char *ptr_out, int len);
int AES_decrypt_memory(unsigned char *key, unsigned char *ptr_in, unsigned char *ptr_out, int len);


#ifdef __cplusplus
}
#endif

#endif // end of AES_HEADER_H__


