/* Return stack-safe version of longjmp_chk.

   Copyright (C) 2011-2018 Free Software Foundation, Inc.

   Return stack support added by
   Philipp Zieris <philipp.zieris@aisec.fraunhofer.de>
   Copyright (C) 2018 Fraunhofer AISEC.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library.  If not, see
   <http://www.gnu.org/licenses/>.  */

#include <jmpbuf-offsets.h>
#include <sysdep.h>
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stackinfo.h>

#ifdef _STACK_GROWS_DOWN
#define called_from(this, saved) ((this) < (saved))
#else
#define called_from(this, saved) ((this) > (saved))
#endif

extern void ____safe_longjmp_chk (__jmp_buf __env, int __val)
  __attribute__ ((__noreturn__));

/* Safe version of `longjmp'.  Restors the return stack pointer by unwinding
   the return stack until the marker is reached.  */
void ____safe_longjmp_chk (__jmp_buf env, int val)
{
  void *this_frame = __builtin_frame_address (0);
  void *saved_frame = JB_FRAME_ADDRESS (env);
  INTERNAL_SYSCALL_DECL (err);
  stack_t ss;

  /* If "env" is from a frame that called us, we're all set.  */
  if (called_from(this_frame, saved_frame))
    __safe_longjmp (env, val);

  /* If we can't get the current stack state, give up and do the longjmp. */
  if (INTERNAL_SYSCALL (sigaltstack, err, 2, NULL, &ss) != 0)
    __safe_longjmp (env, val);

  /* If we we are executing on the alternate stack and within the
     bounds, do the longjmp.  */
  if (ss.ss_flags == SS_ONSTACK &&
      (this_frame >= ss.ss_sp && this_frame < (ss.ss_sp + ss.ss_size)))
    __safe_longjmp (env, val);

  __fortify_fail ("safe_longjmp causes uninitialized stack frame");
}
