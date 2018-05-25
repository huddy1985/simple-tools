/* Copyright (C) 1994 Free Software Foundation, Inc.
This file is part of the GNU C Library.

The GNU C Library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public License as
published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

The GNU C Library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with the GNU C Library; see the file COPYING.LIB.  If
not, write to the Free Software Foundation, Inc., 675 Mass Ave,
Cambridge, MA 02139, USA.  */

#include <ansidecl.h>
#include <errno.h>
#include <sys/socket.h>
#include <hurd.h>
#include <hurd/fd.h>
#include <hurd/socket.h>

/* Read N bytes into BUF from socket FD.
   Returns the number read or -1 for errors.  */
int
DEFUN(recv, (fd, buf, n, flags),
      int fd AND PTR buf AND size_t n AND int flags)
{
  error_t err;
  mach_port_t addrport;
  char *bufp = buf;
  unsigned int nread = n;
  mach_port_t *ports;
  unsigned int nports;
  char *cdata = NULL;
  unsigned int clen = 0;

  if (err = HURD_DPORT_USE (fd, __socket_recv (port, &addrport,
					       flags, &bufp, &nread,
					       &ports, &nports,
					       &cdata, &clen,
					       &flags,
					       n)))
    return __hurd_dfail (fd, err);

  __mach_port_deallocate (__mach_task_self (), addrport);
  __vm_deallocate (__mach_task_self (), (vm_address_t) cdata, clen);

  if (bufp != buf)
    {
      memcpy (buf, bufp, nread);
      __vm_deallocate (__mach_task_self (), (vm_address_t) bufp, nread);
    }

  return nread;
}
