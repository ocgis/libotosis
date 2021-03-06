/************************************************************************
 *
 *  oTOSis - TOS emulator for Linux/68K
 *
 *  Copyright 1996 Elias Martenson <elias@omicron.se>
 *  Copyright 1996 Roman Hodek <Roman.Hodek@informatik.uni-erlangen.de>
 *  Copyright 1998 Tomas Berndtsson <tomas@nocrew.org>
 *  Copyright 1998 - 2001 Christer Gustavsson <cg@nocrew.org>
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

#ifndef DIV_H
#define DIV_H

#ifndef PACKED /* FIXME for non GNU compilers */
#define PACKED __attribute__((packed))
#endif /* PACKED */

#include <sys/types.h>
#include <dirent.h>

#include "libotosis.h"
#include "memory.h"
#include "libotosis_option.h"
#include "config.h"

#define OTOSIS_GLOBAL_CONFIG	"/etc/tos.conf"
#define OTOSIS_USER_CONFIG	".tos"

#define	offsetof(s,e)	(&(((s *)0)->e))
#define	arraysize(a)	(sizeof(a)/sizeof((a)[0]))

#define ARG_USED(x)		(void)(x)


typedef struct {
  SInt16 recsiz;		/* bytes per sector */
  SInt16 clsiz;			/* sectors per cluster */
  SInt16 clsizb;		/* bytes per cluster */
  SInt16 rdlen;			/* sector lenght of root directory */
  SInt16 fsiz;			/* sectors per FAT */
  SInt16 fatrec;		/* starting sector of second FAT */
  SInt16 datrec;		/* starting sector of data */
  SInt16 numcl;			/* clusters per disk */
  SInt16 bflags;		/* bit 0: 1 = 16-bit, 0 = 12 bit */
} PACKED Bpb;

typedef struct
{
  long    high_time;
  long    time;           /* This has to be signed!  */
  ulong   mikroseconds;
} StatTime;

typedef struct {
  UInt64   dev;            /* inode's device */
  UInt32   ino;            /* inode's number */
  UInt32   mode;           /* inode protection mode */
  UInt32   nlink;          /* number of hard links */
  UInt32   uid;            /* user ID of the file's owner */
  UInt32   gid;            /* group ID of the file's group */
  UInt64   rdev;           /* device type */
  StatTime atime;          /* time of last access, UTC */
  StatTime mtime;          /* time of last data modification, UTC */
  StatTime ctime;          /* time of last file status change, UTC */
  UInt64   size;           /* file size, in bytes */
  UInt64   blocks;         /* blocks allocated for file */
  UInt32   blksize;        /* optimal blocksize for I/O */
  UInt32   flags;          /* user defined flags for file */
  UInt32   gen;            /* file generation number */
  
  SInt32   res[7];         /* sizeof = 128 bytes */
} PACKED Stat;


typedef struct {
  UInt32 b_free;		/* Number of free clusters */
  UInt32 b_total;		/* Total number of clusters on the drive */
  UInt32 b_secsize;		/* Bytes per sector */
  UInt32 b_clsize;		/* Sectors per cluster */
} Diskinfo;

typedef struct {
  DIR *dir_pointer;             /* Pointer to DIR */
  char directory[1024];         /* Name of directory */
  char file_mask[1024];         /* File mask */
  int find_mask;                /* Selection of file types to return */
} DirContext;

typedef struct {
  DIR *dir_pointer;		/* Pointer to DIR structure */
  int mode;			/* mode, 0 = long filenames, 1 = TOS */
} DopendirDirContext;

/*
 *  Pexec start modes
 */
#define TOS_PEXEC_LOAD_GO		0
#define TOS_PEXEC_LOAD_DONTDO		3
#define TOS_PEXEC_GO			4
#define TOS_PEXEC_CREATE_BASEPAGE	5
#define TOS_PEXEC_GO_AND_FREE		6
#define TOS_PEXEC_LOAD_GO_NOWAIT	100
#define TOS_PEXEC_GO_NOWAIT		104
#define TOS_PEXEC_GO_NOWAIT_NOSHARING	106
#define TOS_PEXEC_REPLACE_PROGRAM_GO	200

/*
 *  TOS file attibutes
 */
#define TOS_ATTRIB_READONLY	0x01
#define TOS_ATTRIB_HIDDEN	0x02
#define TOS_ATTRIB_SYSTEM	0x04
#define TOS_ATTRIB_VOLUME	0x08
#define TOS_ATTRIB_DIRECTORY	0x10
#define TOS_ATTRIB_ARCHIVE	0x20

/*
 *  TOS chmod flags
 */
#define TOS_S_IRUSR		0x100
#define TOS_S_IWUSR		0x80
#define TOS_S_IXUSR		0x40
#define TOS_S_IRGRP		0x20
#define TOS_S_IWGRP		0x10
#define TOS_S_IXGRP		0x08
#define TOS_S_IROTH		0x04
#define TOS_S_IWOTH		0x02
#define TOS_S_IXOTH		0x01

/*
 *  Tos Fxattr file type flags
 */
#define TOS_S_IFCHR		0x2000
#define TOS_S_IFDIR		0x4000
#define TOS_S_IFREG		0x8000
#define TOS_S_IFIFO		0xA000
#define TOS_S_IMEM		0xC000
#define TOS_S_IFLNK		0xE000

/*
 *  Tos Fcntl(FSTAT64) file type flags
 */
#define TOS_S_IFSOCK_64		0010000
#define TOS_S_IFCHR_64		0020000
#define TOS_S_IFDIR_64		0040000
#define TOS_S_IFBLK_64		0060000
#define TOS_S_IFREG_64		0100000
#define TOS_S_IFIFO_64		0120000
#define TOS_S_IMEM_64		0140000
#define TOS_S_IFLNK_64		0160000

/*
 *  TOS Fcntl commands
 */
#define TOS_F_DUPFD		0x0000
#define TOS_F_GETFD		0x0001
#define TOS_F_SETFD		0x0002
#define TOS_F_GETFL		0x0003
#define TOS_F_SETFL		0x0004
#define TOS_F_GETLK		0x0005
#define TOS_F_SETLK		0x0006
#define TOS_F_SETLKW		0x0007
#define TOS_FSTAT		0x4600
#define TOS_FIONREAD		0x4601
#define TOS_FIONWRITE		0x4602
#define TOS_FSTAT64		0x4606
#define TOS_TIOCGETP		0x5400
#define TOS_TIOCSETP		0x5401
#define TOS_TIOCGETC		0x5402
#define TOS_TIOCSETC		0x5403
#define TOS_TIOCGLTC		0x5404
#define TOS_TIOCSLTC		0x5405
#define TOS_TIOCGPGRP		0x5406
#define TOS_TIOCSPGRP		0x5407
#define TOS_TIOCGWINSZ		0x540B
#define TOS_TIOCSWINSZ		0x540C
#define TOS_TIOCSTOP		0x5409
#define TOS_TIOCSTART		0x540A
#define TOS_TIOCGXKEY		0x540D
#define TOS_TIOCSXKEY		0x540E

#define TOS_PPROCADDR		0x5001
#define TOS_PBASEADDR		0x5002
#define TOS_PCTXTSIZE		0x5003
#define TOS_PSETFLAGS		0x5004
#define TOS_PGETFLAGS		0x5005
#define TOS_PTRACESFLAGS	0x5006
#define TOS_PTRACEGFLAGS	0x5007
#define TOS_PTRACEGO		0x5008
#define TOS_PTRACEFLOW		0x5009
#define TOS_PTRACESTEP		0x500A
#define TOS_PTRACE11		0x500B
#define TOS_PLOADINFO		0x500C

#define TOS_SHMGETBLK		0x4D00
#define TOS_SHMSETBLK		0x4D01

/*
 *  MiNT Pwait* flags
 */

#define TOS_PW_NOBLOCK		0x01
#define TOS_PW_STOPPED		0x02

/*
 *  MiNT Pmsg commands
 */

#define TOS_PMSG_READ		0x0000
#define TOS_PMSG_WRITE		0x0001
#define TOS_PMSG_WRITEREAD	0x0002
#define TOS_PMSG_NONBLOCKING	0x8000

/*
 *  MiNT Psemaphore commands
 */

#define TOS_SEM_CREATE		0
#define TOS_SEM_DESTROY		1
#define TOS_SEM_LOCK		2
#define TOS_SEM_UNLOCK		3



/*
 *  Conveniencs macros for the drive mapping
 *
 */
#define IS_DRV(DRIVE)		((DRIVE) >= 0 && (DRIVE) <= 'z' - 'a' && \
				 prog->drive_map[ (DRIVE) ] != NULL)

#define CHECK_DRV(DRIVE)	({				\
				  if( !IS_DRV(DRIVE) ) {	\
				    return TOS_EDRIVE;		\
				  }				\
				})

#define TOS_TO_UNIX(D,S)   ({						  \
			     int ret;					  \
			     if( (ret = tos_to_unix( (D), (S) )) != 0 ) { \
			       return ret;				  \
			     }						  \
			   })
\
typedef SInt32 TosSystemCall( char * );
#ifdef DEBUG

extern int mid_of_line;		/* declared in strace.c */

#define CHECK_MID_OF_LINE()			\
    do {					\
	if (mid_of_line) {			\
	    fprintf( stderr, "\n" );		\
	    mid_of_line = 0;			\
	}					\
    } while(0)

#define DDEBUG(fmt,args...)			\
    do {					\
	if (Opt_debug) {			\
	    CHECK_MID_OF_LINE();		\
	    fprintf( stderr, fmt, ## args);	\
	}					\
    } while(0)

#define STRACE_BEGIN(mod,a)				\
    do {						\
	if (Opt_debug)					\
	    strace_begin( mod##_call_names[callnum],	\
			  mod##_call_args[callnum],	\
			  a );				\
	if (prog->trace) {				\
	    CHECK_MID_OF_LINE();			\
	    handle_trace( a );				\
	}						\
    } while(0)

#define STRACE_END(mod,a,r)				\
    do {						\
    	if (Opt_debug)					\
	    strace_end( mod##_call_names[callnum],	\
			mod##_call_retv[callnum],	\
			a, r );				\
    } while(0)

#else
#define DDEBUG(fmt,args...)
#define	STRACE_BEGIN(mod,a)
#define	STRACE_END(mod,a,r)
#endif

#define XFUNC(prefix,name) SInt32 prefix##_##name( char *_args )
#define	BIOSFUNC(name)     XFUNC(bios,name)
#define	XBIOSFUNC(name)    XFUNC(xbios,name)
#define	GEMDOSFUNC(name)   XFUNC(gemdos,name)
#define	XGEMDOSFUNC(name)  XFUNC(xgemdos,name)
#define	MINTFUNC(name)     XFUNC(mint,name)

#if USE_DIRECT_MOVES
#define TOSARG(type,name) type name = *((type *)_args)++
#else /* USE_DIRECT_MOVES */
#define GETPART(offset, shift) \
        (((UInt32)(unsigned char)_args[(offset)]) << (shift))
#define TOSARG(type, name) \
        type name = (type)((_args += sizeof(type)), (sizeof(type) == 2 ? \
             (GETPART(-2,  8) | GETPART(-1,  0)) : \
             (GETPART(-4, 24) | GETPART(-3, 16) | \
              GETPART(-2,  8) | GETPART(-1,  0))))
#endif /* USE_DIRECT_MOVES */

#define XUNIMP(prefix,name)						\
	SInt32 prefix##_##name( char *args ) {				\
		printf( "Unimplemented " #prefix " call " #name "\n" );	\
		return TOS_EINVFN;					\
	}
#define	BIOS_UNIMP(name)    XUNIMP(bios,name)
#define	XBIOS_UNIMP(name)   XUNIMP(xbios,name)
#define	GEMDOS_UNIMP(name)  XUNIMP(gemdos,name)
#define	XGEMDOS_UNIMP(name) XUNIMP(xgemdos,name)
#define	MINT_UNIMP(name)    XUNIMP(mint,name)

/* The attribute 'SIGSTACK_STATIC' is used before certain local variables that
 * can cause stack overflows if there are no sigstacks. It's currently defined
 * to declare such stuff static to move it from the stack, but as soon as the
 * sigstack feature is in common kernels, it can go away (then define it
 * empty) */
#define SIGSTACK_STATIC static

#endif /* DIV_H */

/* Local Variables:              */
/* tab-width: 8                  */
/* compile-command: "make -C .." */
/* End:                          */
