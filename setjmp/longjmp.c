/* Copyright (C) 1991-2018 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#include <stddef.h>
#include <setjmpP.h>
#include <signal.h>


/* Set the signal mask to the one specified in ENV, and jump
   to the position specified in ENV, causing the setjmp
   call there to return VAL, or 1 if VAL is 0.  */
void
__libc_siglongjmp (sigjmp_buf env, int val)
{
  /* Perform any cleanups needed by the frames being unwound.  */
  _longjmp_unwind (env, val);

  if (env[0].__mask_was_saved)
    /* Restore the saved signal mask.  */
    (void) __sigprocmask (SIG_SETMASK,
			  (sigset_t *) &env[0].__saved_mask,
			  (sigset_t *) NULL);

  /* Call the machine-dependent function to restore machine state.  */
  __longjmp (env[0].__jmpbuf, val ?: 1);
}

#ifdef WITH_RETURN_STACK_SUPPORT
void
__libc_safe_siglongjmp (sigjmp_buf env, int val)
{
  /* Perform any cleanups needed by the frames being unwound.  */
  _longjmp_unwind (env, val);

  if (env[0].__mask_was_saved)
    /* Restore the saved signal mask.  */
    (void) __sigprocmask (SIG_SETMASK, &env[0].__saved_mask,
			  (sigset_t *) NULL);

  /* Call the safe version of longjmp to restore the machine state.  */
  __safe_longjmp (env[0].__jmpbuf, val ?: 1);
}
#endif

#ifndef __libc_siglongjmp
strong_alias (__libc_siglongjmp, __libc_longjmp)
libc_hidden_def (__libc_longjmp)
# ifdef WITH_RETURN_STACK_SUPPORT
strong_alias (__libc_safe_siglongjmp, __libc_safe_longjmp)
# endif
weak_alias (__libc_siglongjmp, _longjmp)
weak_alias (__libc_siglongjmp, longjmp)
weak_alias (__libc_siglongjmp, siglongjmp)
# ifdef WITH_RETURN_STACK_SUPPORT
weak_alias (__libc_safe_siglongjmp, _safe_longjmp)
weak_alias (__libc_safe_siglongjmp, safe_longjmp)
weak_alias (__libc_safe_siglongjmp, safe_siglongjmp)
# endif
#endif
