#ifndef _concatenate_h
#define _concatenate_h
/*
 *  This file is part of libESMTP, a library for submission of RFC 822
 *  formatted electronic mail messages using the SMTP protocol described
 *  in RFC 821.
 *
 *  Copyright (C) 2001  Brian Stafford  <brian@stafford.uklinux.net>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

struct catbuf
  {
    char *buffer;
    int string_length;
    int allocated;
  };

void cat_init (struct catbuf *catbuf, int minimum_length);
void cat_reset (struct catbuf *catbuf, int minimum_length);
void cat_free (struct catbuf *catbuf);
char *cat_buffer (struct catbuf *catbuf, int *len);
char *concatenate (struct catbuf *catbuf, const char *string, int len);
char *vconcatenate (struct catbuf *catbuf, ...);
int cat_printf (struct catbuf *catbuf, const char *format, ...);

#endif
