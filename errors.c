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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <errno.h>
#include <string.h>
#include <netdb.h>
#include "libesmtp-private.h"
#include "api.h"

#ifndef USE_PTHREADS
/* This is the totally naive and useless version, I can't believe I've
   actually just written this!  Less embarrasing code follows.
 */

static int libesmtp_errno;

/* The library's internal function */
void
set_error (int code)
{
  libesmtp_errno = code;
}

/* Internal version of the library's API function */
int
smtp_errno (void)
{
  return libesmtp_errno;
}

#else
/* Real, non-embarrasing code :-) */

#include <pthread.h>

static pthread_key_t libesmtp_errno;
static pthread_once_t libesmtp_errno_once = PTHREAD_ONCE_INIT;

static void
libesmtp_errno_alloc (void)
{
  pthread_key_create (&libesmtp_errno, NULL);
}

void
set_error (int code)
{
  pthread_once (&libesmtp_errno_once, libesmtp_errno_alloc);
  pthread_setspecific (libesmtp_errno, (void *) code);
}

int
smtp_errno (void)
{
  return (int) pthread_getspecific (libesmtp_errno);
}

#endif

static const char *libesmtp_errors[] =
  {
    "No Error",
    "",
    "Nothing to do",					/* NOTHING_TO_DO */
    "SMTP server dropped connection",			/* DROPPED_CONNECTION */
    "Invalid SMTP syntax in server response",		/* INVALID_RESPONSE_SYNTAX */
    "SMTP Status code mismatch on continuation line",	/* STATUS_MISMATCH */
    "Invalid SMTP status code in server response",	/* INVALID_RESPONSE_STATUS */
    "Invalid API function argument",			/* INVAL */
    "requested SMTP extension not available",		/* EXTENSION_NOT_AVAILABLE */
    "Host not found",					/* HOST_NOT_FOUND */
    "No address",					/* NO_ADDRESS */
    "No recovery",					/* NO_RECOVERY */
    "Temporary DNS failure; try again later",		/* TRY_AGAIN */
  };

char *
smtp_strerror (int error, char buf[], int buflen)
{
  const char *text;
  int len;

  if (error < 0)
#ifdef HAVE_STRERROR_R
    return strerror_r (-error, buf, buflen);
#else
    text = strerror (-error);
#endif
  else if (error < (sizeof libesmtp_errors / sizeof libesmtp_errors[0]))
    text = libesmtp_errors[error];
  else
    text = (const char *) 0;

  if (text == (const char *) 0)
    snprintf (buf, buflen, "Error %d", error);
  else
    {
      len = strlen (text);
      if (len > buflen - 1)
        len = buflen - 1;
      memcpy (buf, text, len);
      buf[len] = '\0';
    }
  return buf;
}

/* Map the values of h_errno to libESMTP's error codes */
void
set_herror (int code)
{
  int smtp_code;

  switch (code)
    {
    case HOST_NOT_FOUND:	smtp_code = SMTP_ERR_HOST_NOT_FOUND; break;
    case NO_ADDRESS:		smtp_code = SMTP_ERR_NO_ADDRESS; break;
    case NO_RECOVERY:		smtp_code = SMTP_ERR_NO_RECOVERY; break;
    case TRY_AGAIN:		smtp_code = SMTP_ERR_TRY_AGAIN; break;
    default: /* desperation */	smtp_code = SMTP_ERR_INVAL; break;
    }
  set_error (smtp_code);
}

/* store the value of errno in libESMTP's error variable. */
void
set_errno (int code)
{
  set_error (-code);
}
