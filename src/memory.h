/************************************************************************
 *
 *  oTOSis - TOS emulator for Linux/68K
 *
 *  Copyright 1999 Christer Gustavsson <cg@nocrew.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 ************************************************************************/

#ifndef MEMORY_H
#define MEMORY_H

#include "libotosis.h"

#define HAVE_INT64		1

#if HAVE_INT64
#define VAL64(X)		(X ## LL)
#define UVAL64(X)		(X ## ULL)
#endif

#ifdef NATIVE_M68K

#define GET_LONG(X)	(X)
#define GET_WORD(X)	(X)
#define GET_BYTE(X)	(X)
#define SET_LONG(X,Y)	((X) = (Y))
#define SET_WORD(X,Y)	((X) = (Y))
#define SET_BYTE(X,Y)	((X) = (Y))

#else

/*
 *  The following code only works on big endian machines
 */
#define VADDR(X)	((X) >= m68k_memory_base_address && \
			 (X) < m68k_memory_top_address)
#define GADDR(X)	(((UInt8 *)m68k_memory_ptr) + \
			 ((X) - m68k_memory_base_address))

#define SET_LONG(X,Y)	((*((UInt16 *)(GADDR(X))) = (((UInt32)(Y)&0xFFFF0000) >> 16)), \
			 (*((UInt16 *)(GADDR(X)) + 1) = (UInt32)(Y)&0x0000FFFF))
#define SET_WORD(X,Y)	(*(UInt16 *)(GADDR(X)) = (Y))
#define SET_BYTE(X,Y)	(*(UInt8 *)(GADDR(X)) = (Y))

#define GET_LONG(X)	(((UInt32)(*(UInt16 *)(GADDR(X)))) << 16 | \
			 ((UInt32)(*((UInt16 *)(GADDR(X)) + 1))))
#define GET_WORD(X)	(*(UInt16 *)(GADDR(X)))
#define GET_BYTE(X)	(*(UInt8 *)(GADDR(X)))

#endif




#endif
