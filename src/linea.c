/************************************************************************
 *
 *  oTOSis - TOS emulator for Linux/68K
 *
 *  Copyright 2001 Christer Gustavsson <cg@nocrew.org>
 *
 *  linea.c - Fake Line-A routines
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

#include "client_endian.h"
#include "lineabind.h"

typedef struct
{
/* Type    Name       Offset   Function		    Comments		     */
/* ------------------------------------------------------------------------- */
  short _V_CEL_MX;    /* -44 */
  short _V_CEL_MY;    /* -42 */
  short _V_CEL_WR;    /* -40 */
  short _V_CEL_BG;    /* -38 */
  short _V_CEL_FG;    /* -36 */
  long  _V_CUR_AD;    /* -34 */
  short _V_CUR_OF;    /* -30 */
  short _V_CUR_XY[2]; /* -28 */
  char  _V_PERIOD;    /* -24 */
  char  _V_CUR_CT;    /* -23 */
  long  _V_FNT_AD;    /* -22 */
  short _V_FNT_ND;    /* -18 */
  short _V_FNT_ST;    /* -16 */
  short _V_FNT_WD;    /* -14 */
  short _V_REZ_HZ;    /* -12 */
  short _V_OFF_AD;    /* -10 */
  short _RESVD_M6;    /*  -6 */
  short _V_REZ_VT;    /*  -4 */
  short _BYTES_LINE;  /*  -2 */
} LINEA_BOTTOM;


typedef struct
{
  LINEA_BOTTOM bottom;
  __LINEA      top;
} LINEA_ALL;

static LINEA_ALL linea;


__LINEA * __aline = &linea.top;


void
linea0(void)
{
  /* Just fake values for now */
  linea.bottom._V_CEL_MX = HW_TO_CW(79);
  linea.bottom._V_CEL_MY = HW_TO_CW(24);
  linea.bottom._V_REZ_HZ = HW_TO_CW(640);
  linea.bottom._V_REZ_VT = HW_TO_CW(480);
}
