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

#ifndef __LIBOTOSIS_H__
#define __LIBOTOSIS_H__

/* FIXME */
#define PACKED __attribute__((packed))

typedef unsigned char UInt8;
typedef unsigned short UInt16;
typedef unsigned int UInt32;

typedef signed char SInt8;
typedef signed short SInt16;
typedef signed int SInt32;

#ifdef NATIVE_M68K
typedef void * Ptr32;
#else
typedef UInt32 Ptr32;
#endif

#define TOS_PROGRAM_MAGIC	0x601A

#define TEXT_SEGMENT(H,X)	((Ptr32 )((UInt32)(X)))
#define DATA_SEGMENT(H,X)	((Ptr32 )((UInt32)TEXT_SEGMENT(H,X) + ntohl((H)->tsize)))
#define BSS_SEGMENT(H,X)	((Ptr32 )((UInt32)DATA_SEGMENT(H,X) + ntohl((H)->dsize)))
#define FIXUP_OFFSET(H)		((Ptr32 )((UInt32)BSS_SEGMENT(H,sizeof(TosExecHeader)) + ntohl((H)->ssize)))

typedef struct
{
  UInt16 magic;			/* Magic number */
  UInt32 tsize;			/* Size of TEXT segment */
  UInt32 dsize;			/* Size of DATA segment */
  UInt32 bsize;			/* Size of BSS segment */
  UInt32 ssize;			/* Size of symbol table */
  UInt32 res1;			/* Reserved */
  UInt32 prgflags;		/* Program flags */
  UInt16 absflag;		/* Absolute program flag */
} PACKED TosExecHeader;

typedef struct {
  char d_reserved[ 21 ];	/* Reserved */
  char d_attrib;		/* Attribute */
  UInt16 d_time;		/* Time */
  UInt16 d_date;		/* Date */
  UInt32 d_length;		/* Size */
  char d_fname[ 14 ];		/* File name */
} PACKED Dta;

typedef struct {
  Ptr32 lowtpa;			/* Pointer to the TPA */
  Ptr32 hitpa;			/* Pointer to end of TPA */
  Ptr32 tbase;			/* Pointer to TEXT segment */
  UInt32 tlen;			/* Length of TEXT segment */
  Ptr32 dbase;			/* Pointer to DATA segment */
  UInt32 dlen;			/* Lenght of DATA segment */
  Ptr32 bbase;			/* Pointer to BSS segment */
  UInt32 blen;			/* Lenght of BSS segment */
  Dta *dta;			/* Pointer to DTA */
  Ptr32 parent;			/* Pointer to parent basepage */
  UInt32 reserved;		/* Reserved */
  char *env;			/* Pointer to environment */
  char undef[ 80 ];		/* Undefined string */
  char cmdlin[ 128 ];		/* Command line */
} TosBasepage;

typedef struct
{
  Ptr32 text;			/* Pointer to TEXT segment */
  Ptr32 data;			/* Pointer to DATA segment */
  Ptr32 bss;			/* Pointer to BSS segment */
  TosBasepage *basepage;	/* Pointer to basepage */
  Dta *dta;			/* Pointer to DTA */
  UInt32 size;			/* Size of allocated memory */
  int domain;			/* Current MiNT domain */
  SInt32 usrval;		/* Usr value */
  int emulate_mint;		/* MiNT emulation */
  int super;			/* Emulate supervisor */
  int trace;			/* Trace active */
  int curdrv;			/* Current drive */
  char *curdir[ 'z' - 'a' + 1 ]; /* Current directory */
  char *drive_map[ 'z' - 'a' + 1 ]; /* Drive map */
  int gem;			/* Use GEM */
} TosProgram;

typedef struct {
  UInt32 cookie;
  UInt32 value;
} Cookie;

/*
 *  Program flags
 */
#define TOS_PRGFLAG_FASTLOAD		0x01
#define TOS_PRGFLAG_LOAD_TTRAM		0x02
#define TOS_PRGFLAG_MALLOC_TTRAM	0x04

#endif __LIBOTOSIS_H__
