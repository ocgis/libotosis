/************************************************************************
 *
 *  oTOSis - TOS emulator
 *
 *  endian.h - Endian related macros
 *
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

#ifndef CLIENT_ENDIAN_H
#define CLIENT_ENDIAN_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef WORDS_BIGENDIAN

#define CW_TO_HW(cw) (cw)
#define HW_TO_CW(hw) (hw)
#define CL_TO_HL(cl) (cl)
#define HL_TO_CL(hl) (hl)
#define CLL_TO_HLL(cll) (cll)
#define HLL_TO_CLL(hll) (hll)

#else

#include <netinet/in.h>

static
inline
long long
swap_long_long(long long ll)
{
  long long retval;

  retval =
    ((ll >> 56) & 0x00000000000000ff) |
    ((ll >> 40) & 0x000000000000ff00) |
    ((ll >> 24) & 0x0000000000ff0000) |
    ((ll >>  8) & 0x00000000ff000000) |
    ((ll <<  8) & 0x000000ff00000000) |
    ((ll << 24) & 0x0000ff0000000000) |
    ((ll << 40) & 0x00ff000000000000) |
    ((ll << 56) & 0xff00000000000000);

  return retval;
}


#define CW_TO_HW(cw) ntohs(cw)
#define HW_TO_CW(hw) htons(hw)
#define CL_TO_HL(cl) ntohl(cl)
#define HL_TO_CL(hl) htonl(hl)
#define CLL_TO_HLL(cll) swap_long_long(cll)
#define HLL_TO_CLL(hll) swap_long_long(hll)

#endif /* WORDS_BIGENDIAN */

#endif /* CLIENT_ENDIAN_H */
