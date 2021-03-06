/************************************************************************
 *
 *  oTOSis - TOS emulator for Linux/68K
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds, Lars Wirzenius
 *  Copyright 1996 Roman Hodek <Roman.Hodek@informatik.uni-erlangen.de>
 *  Copyright 2001 Christer Gustavsson <cg@nocrew.org>
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

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <netinet/in.h>

#include "client_endian.h"
#include "div.h"
#include "prototypes.h"

/*
 * This global variable is set whenever the cursor is in the middle of the
 * output line, after the "Function( ... ) " part of the strace output. It is
 * used by DDEBUG and handle_trace() to print an extra newline (for output
 * sanity :-), and by strace_end() to restart the log line.
 */
int mid_of_line = 0;

/*
 * string representations of TOS errno values
 */
char *tos_errno_names[] = {
	"0 E_OK (No error)",
	"-1 ERROR (General error)",
	"-2 EDRVNR (Drive not ready)",
	"-3 EUNCMD (Unknown command)",
	"-4 E_CRC (CRC error)",
	"-5 EBADRQ (Bad request)",
	"-6 E_SEEK (Seek error)",
	"-7 EMEDIA (Unknown media)",
	"-8 ESECNF (Sector not found)",
	"-9 EPAPER (Out of paper)",
	"-10 EWRITF (Write fault)",
	"-11 EREADF (Read fault)",
	"-12 EWRPRO (Write protected)",
	/* -13 */ NULL,
	"-14 E_CHNG (Media change)",
	"-15 EUNDEV (Unknown device)",
	"-16 EBADSF (Bad sectors on format)",
	"-17 EOTHER (Insert other disk)",
	/* -18..-25 */ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	/* -26..-31 */ NULL, NULL, NULL, NULL, NULL, NULL,
	"-32 EINVFN (Invalid function)",
	"-33 EFILNF (File not found)",
	"-34 EPTHNF (Path not found)",
	"-35 ENHNDL (No more handles)",
	"-36 EACCDN (Access denied)",
	"-37 EIHNDL (Invalid handle)",
	/* -38  */ NULL,
	"-39 EINSMEM (Insufficient memory)",
	"-40 EIMBA (Invalid memory block address)",
	/* -41..-45 */ NULL, NULL, NULL, NULL, NULL, 
	"-46 EDRIVE (Invalid drive specification)",
	/* -47 */ NULL,
	"-48 ENSAME (Cross-device rename)",
	"-49 ENMFIL (No more files)",
	/* -50..-57 */ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	"-58 ELOCKED (Record is already locked)",
	"-59 ENSLOCK (Invalid lock removal request)",
	/* -60..-63 */ NULL, NULL, NULL, NULL,
	"-64 ERANGE (Range error)",
	"-65 EINTRN (Internal error)",
	"-66 EPLFMT (Invalid program load format)",
	"-67 EGSBF (Memory block growth failure)",
	/* -68..-75 */ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	/* -76..-79 */ NULL, NULL, NULL, NULL,
	"-80 ELOOP (Too many symbolic links)",
	/* -81..-88 */ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	/* -89..-96 */ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	/* -97..-104 */ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	/* -105..-112 */ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	/* -113..-120 */ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	/* -121..-128 */ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	/* -129..-136 */ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	/* -137..-144 */ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	/* -145..-152 */ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	/* -153..-160 */ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	/* -161..-168 */ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	/* -169..-176 */ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	/* -177..-184 */ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	/* -185..-192 */ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	/* -193..-199 */ NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	"-200 EMOUNT (Mount point crossed)",
};

#define	LAST_TOS_ERRNO	-200

/*
 * Structures for printing TOS structures
 */

typedef struct {
	unsigned offset;
	char     *name;
	char     *format;
} COMPONENT;

typedef struct {
	char      *name;
	unsigned  n_components;
	COMPONENT *components;
} TOSSTRUCT;

COMPONENT LINE_components[] = {
	{ 0, "maxlen", "%qd" },
	{ 1, "actlen", "%qd" },
	{ 2, "buf", "%z" }
};

COMPONENT DISKINFO_components[] = {
	{  0, "free", "%ld" },
	{  4, "total", "%ld" },
	{  8, "secsiz", "%ld" },
	{ 12, "clsiz", "%ld" }
};

COMPONENT DOSTIME_components[] = {
	{ 0, "time", "%T" },
	{ 2, "date", "%D" }
};

COMPONENT DTA_components[] = {
	{ 21, "attr", "%02qx" },
	{ 22, "time", "%T" },
	{ 24, "date", "%D" },
	{ 26, "size", "%ld" },
	{ 30, "name", "%z" }
};

COMPONENT BPB_components[] = {
	{  0, "recsiz", "%hd" },
	{  2, "clsiz", "%hd" },
	{  4, "clsizb", "%hd" },
	{  6, "rdlen", "%hd" },
	{  8, "fsiz", "%hd" },
	{ 10, "fatrec", "%hd" },
	{ 12, "datrec", "%hd" },
	{ 14, "numcl", "%hd" },
	{ 16, "flags", "%02x" }
};

COMPONENT XATTR_components[] = {
	{  0, "mode", "%04ho" },
	{  2, "index", "%ld" },
	{  6, "dev", "%04hx" },
	{ 10, "nlink", "%hu" },
	{ 12, "uid", "%hd" },
	{ 14, "gid", "%hd" },
	{ 16, "size", "%ld" },
	{ 20, "blksize", "%ld" },
	{ 24, "nblocks", "%ld" },
	{ 28, "mtime", "%T" },
	{ 30, "mdate", "%D" },
	{ 32, "atime", "%T" },
	{ 34, "adate", "%D" },
	{ 36, "ctime", "%T" },
	{ 38, "cdate", "%D" },
	{ 40, "attr", "%04hx" }
};

#define	DECLARE_STRUCT(name) \
    { #name, sizeof(name##_components)/sizeof(COMPONENT), name##_components }
	
TOSSTRUCT TOS_structures[] = {
	DECLARE_STRUCT(LINE),
	DECLARE_STRUCT(DISKINFO),
	DECLARE_STRUCT(DOSTIME),
	DECLARE_STRUCT(DTA),
	DECLARE_STRUCT(BPB),
	DECLARE_STRUCT(XATTR),
	{ NULL, 0, NULL }
};



/***************************** Prototypes *****************************/

static int tos_vfprintf( FILE *f, const char *fmt, const void *oargp, long
                         *extra_arg, int ffs );
static void single_arg(const char *format,
                       const void **oargpp);
static TOSSTRUCT *find_struct( const char *name );
static void print_TOS_retval( const char *fmt, const char *argp, long rv );

/************************* End of Prototypes **************************/



/*
 * Start strace log line with function name and arguments; cursor stops after
 * ')', to be continued by strace_end().
 */
void strace_begin( const char *call_name, const char *call_format,
				   const char *argp )
{
	fprintf( stderr, "%s(", call_name );
	tos_vfprintf( stderr, call_format, argp, NULL, 0 );
	fprintf( stderr, ")" );

	mid_of_line = 1;
}

/*
 * Finish an strace log line with the syscall's return value. If some other
 * output happened in the time between, reprint some information.
 */
void strace_end( const char *call_name, const char *rv_format,
				 const char *argp, long rv )
{
	/* if some other output intervened, reprint the function name */
	if (!mid_of_line)
		fprintf( stderr, "%s(...)", call_name );

	fprintf( stderr, " = " );
	print_TOS_retval( rv_format, argp, rv );
	fprintf( stderr, "\n" );
	
	mid_of_line = 0;
}
	

/*
 * This function does more or less the same as vfprintf(), but it treats short
 * arguments ('%...h.') really as 16-bit, since this is widely used in TOS.
 * And the standard libc function always assume shorts are promoted to int,
 * which is not the case for arguments passed from TOS programs.
 */

static int tos_vfprintf( FILE *f, const char *fmt, const void *oargp,
						 long *extra_arg, int ffs )
{
	unsigned long new_args[40], *nargp;		/* 40 should be enough... */
	const char *p, *q, *start;
	char sfmt[64];
	int cnt = 0;

	nargp = new_args;

	/* Parse the format to know about number and type of arguments */
	for( p = fmt; *p; ++p ) {
		if (*p != '%') {
            fprintf(stdout, "%c", *p);
			continue;
		}
		if (!*++p) break;

		start = p-1;
		++cnt;

		/* check for structure */
		if (*p == '{' && !ffs) {
			TOSSTRUCT *struc;
			COMPONENT *comp;
			const char *struct_p;
			const void *carg;
			int i;
			
			++p;
			if (!(q = strchr( p, '}'))) {
				fprintf( stderr, "Internal error: unterminated %%{ in "
						 "format spec\n" );
				rexit( 1 );
			}
			strncpy( sfmt, p, q-p ); sfmt[q-p] = 0;

			if (cnt == 1 && extra_arg)
				struct_p = *(const char **)extra_arg, extra_arg = NULL;
			else
            {
				struct_p = *((const char **)oargp);
                oargp += sizeof(const char *);
            }

            struct_p = (const char *)CL_TO_HL((UInt32)struct_p);

			if ((struc = find_struct( sfmt )))
            {
                fprintf(stdout, "{");
				for( i = 0, comp = struc->components; i < struc->n_components;
					 ++i, ++comp ) {
					fprintf(stdout, "%s=", comp->name);

					carg = (const void *)(struct_p + comp->offset);
					single_arg( comp->format, &carg);

					if (i != struc->n_components-1)
                    {
                      fprintf(stdout, " ");
                    }
				}
				fprintf(stdout, "}");
			}
			else
				fprintf( stderr, "Internal error: Cannot print struct %s\n",
						 sfmt );
			p = q;
		}
		else {
			p += strspn( p, "-0123456789lh" );
			if (!strchr( "duxospDTS", *p )) {
				fprintf( stderr, "Internal error: bad char '%c' in "
						 "format spec\n", *p );
				rexit( 1 );
			}
			strncpy( sfmt, start, p+1-start );
			sfmt[p+1-start] = 0;

			if (cnt == 1 && extra_arg) {
				/* special case for translated errno's: */
				if(ffs)
                {
                  fprintf(stdout,
                          "%s",
                          (char *)CL_TO_HL(*(UInt32 *)extra_arg));
				}
				else
					single_arg(sfmt, (const void **)&extra_arg);
				extra_arg = NULL;
			}
			else
				single_arg(sfmt, &oargp);
		}
	}

	return 0;
}

/*
 * Extensions to standard printf() format specs:
 *
 *  - modifier 'q' can be used for 8-bit arguments (not possible on the
 *    stack, but in structures)
 *
 *  - format code 'S' means "skip", i.e. skip argument of width determined by
 *    modifiers, but print nothing
 *
 *  - format code 'D' stands for TOS style date (16-bit)
 *  
 *  - format code 'T' stands for TOS style time (16-bit)
 *
 *  - format code 'x' automatically prints "0x" prefix
 *  
 *  - format code 's' automatically frames string in '"'
 *
 *  - format code 'z' is like 's', but the argument isn't a pointer to the
 *    string, but the string is at the argument position itself (for
 *    structures containing char arrays)
 */

static void single_arg(const char *format,
                       const void **oargpp)
{
	const char *fmt = format;
	int is_short = 0, is_quart = 0, is_signed = 0, is_date = 0, is_time = 0,
		is_direct_str = 0, do_skip = 0;
	char *prefix = NULL, *postfix = NULL;

	fmt++; /* Skip '%' */
	for( ; *fmt; ++fmt ) {
		if (isdigit(*fmt) || *fmt == '-')
			;
		else if (*fmt == 'l')
			is_short = 0;
		else if (*fmt == 'h')
			is_short = 1;
		else if (*fmt == 'q') {
			is_quart = 1;
			continue; /* don't copy 'q' into nfmt */
		}
		else if (*fmt == 'D') {
			is_date = 1;
			break;
		}
		else if (*fmt == 'T') {
			is_time = 1;
			break;
		}
		else if (*fmt == 'S') {
			do_skip = 1;
			break;
		}
		else if (*fmt == 'z') {
			is_direct_str = 1;
			break;
		}
		else if (strchr( "duxosp", *fmt )) {
			is_signed = (*fmt == 'd');
			if (*fmt == 'x') prefix = "0x";
			if (*fmt == 's') prefix = postfix = "\"";
			break;
		}
		else {
			fprintf( stderr, "Internal error: bad char '%c' in format spec\n",
					 *fmt );
			rexit( 1 );
		}
	}

    if(prefix != NULL)
    {
      fprintf(stdout, prefix);
    }

	if (is_date)
    {
		unsigned date;

        date = CW_TO_HW(*((unsigned short *)(*oargpp)));
        *oargpp += sizeof(unsigned short);

		fprintf(stdout, "%02d.%02d.%d",
                date & 0x1f, (date >> 5) & 0x0f,
                ((date >> 9) & 0x7f) + 1980 );
	}
	else if (is_time)
    {
		unsigned time;

        time = CW_TO_HW(*((unsigned short *)(*oargpp)));
        *oargpp += sizeof(unsigned short);

		fprintf(stdout, "%02d:%02d:%02d",
                (time >> 11) & 0x1f, (time >> 5) & 0x3f,
                (time & 0x1f) * 2 );
	}
	else if (do_skip) {
		/* new format empty -> print nothing */
		*oargpp += is_quart ? 1 : is_short ? 2 : 4;
	}
	else if (is_direct_str) {
		prefix = postfix = "\"";

		fprintf(stdout, "%s,", (char *)*oargpp++);
	}
	else
    {
		/* get the argument with correct width, do correct sign extend and
		 * push it onto new_args array */
		if (is_quart)
        {
			if (is_signed)
            {
			  fprintf(stdout, "%c,", *((signed char *)*oargpp));
              *oargpp += sizeof(signed char);
            }
			else
            {
			  fprintf(stdout, "%c,", *((unsigned char *)*oargpp));
              *oargpp += sizeof(unsigned char);
            }
		}

		if (is_short)
        {
			if (is_signed)
            {
			  fprintf(stdout, format, ntohs(*((signed short *)(*oargpp))));
              *oargpp += sizeof(signed short);
            }
			else
            {
			  fprintf(stdout, format, ntohs(*((unsigned short *)(*oargpp))));
              *oargpp += sizeof(unsigned short);
            }
		}
		else
        {
			if (is_signed)
            {
			  fprintf(stdout,
                      format,
                      (long)ntohl(*((signed long *)(*oargpp))));
              *oargpp += sizeof(signed long);
            }
			else
            {
			  fprintf(stdout, format, ntohl(*((unsigned long *)(*oargpp))));
              *oargpp += sizeof(unsigned long);
            }
		}
	}

    if(postfix != NULL)
    {
      fprintf(stdout, postfix);
    }
}


static TOSSTRUCT *find_struct( const char *name )
{
	TOSSTRUCT *p;
	
	for( p = TOS_structures; p->name; ++p )
		if (0 == strcmp( p->name, name )) break;
	return( p->name ? p : NULL );
}


/*
 * Special features for the return value format string:
 *
 *  - unless it starts with '#', small negative numbers are interpreted as TOS
 *    errno's, if that makes sense
 * 
 *  - if the first character is '@', only -1 is not interpreted as errno
 *
 */

static void print_TOS_retval( const char *fmt, const char *argp, long rv )
{
	int no_errno = 0, minus1_ok = 0, did_translate = 0;
	
	if (*fmt == '#')
		no_errno = 1, ++fmt;
	else if (*fmt == '@')
		minus1_ok = 1, ++fmt;

	if (!no_errno &&
		rv < (minus1_ok ? -1 : 0) && rv >= LAST_TOS_ERRNO &&
		tos_errno_names[-rv]) {
		rv = (long)tos_errno_names[-rv];
		did_translate = 1;
	}

    rv = HL_TO_CL(rv);

	/* Special hack for %D and %T formats (16-bit values!) */
	if(*fmt == '%' && ((fmt[1] == 'D') || (fmt[1] == 'T')))
    {
      UInt16 * rvP;

      rvP = (UInt16 *)&rv;

      rvP[0] = rvP[1];
      rvP[1] = 0;
    }

	tos_vfprintf( stderr, fmt, argp, &rv, did_translate );
}

/* Local Variables: */
/* tab-width: 4     */
/* End:             */
