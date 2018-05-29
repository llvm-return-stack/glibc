#ifndef _SETJMP_H
#include <setjmp/setjmp.h>

#ifndef _ISOMAC
/* Now define the internal interfaces.  */

/* Internal machine-dependent function to restore context sans signal mask.  */
extern void __longjmp (__jmp_buf __env, int __val)
     __attribute__ ((__noreturn__)) attribute_hidden;
#ifdef WITH_RETURN_STACK_SUPPORT
extern void __safe_longjmp (__jmp_buf __env, int __val)
     __attribute__ ((__noreturn__)) attribute_hidden;
#endif

extern void ____longjmp_chk (__jmp_buf __env, int __val)
     __attribute__ ((__noreturn__)) attribute_hidden;
#ifdef WITH_RETURN_STACK_SUPPORT
extern void ____safe_longjmp_chk (__jmp_buf __env, int __val)
     __attribute__ ((__noreturn__));
#endif

/* Internal function to possibly save the current mask of blocked signals
   in ENV, and always set the flag saying whether or not it was saved.
   This is used by the machine-dependent definition of `__sigsetjmp'.
   Always returns zero, for convenience.  */
extern int __sigjmp_save (jmp_buf __env, int __savemask);

extern void _longjmp_unwind (jmp_buf env, int val);

extern void __libc_siglongjmp (sigjmp_buf env, int val)
	  __attribute__ ((noreturn));
#ifdef WITH_RETURN_STACK_SUPPORT
extern void __libc_safe_siglongjmp (sigjmp_buf env, int val)
	  __attribute__ ((noreturn));
#endif
extern void __libc_longjmp (sigjmp_buf env, int val)
     __attribute__ ((noreturn));
libc_hidden_proto (__libc_longjmp)
#ifdef WITH_RETURN_STACK_SUPPORT
extern void __libc_safe_longjmp (sigjmp_buf env, int val)
     __attribute__ ((noreturn));
libc_hidden_proto (__libc_safe_longjmp)
#endif

libc_hidden_proto (_setjmp)
libc_hidden_proto (__sigsetjmp)
#ifdef WITH_RETURN_STACK_SUPPORT
libc_hidden_proto (_safe_setjmp)
libc_hidden_proto (__safe_sigsetjmp)
#endif

# if IS_IN (rtld) && !defined NO_RTLD_HIDDEN
extern __typeof (__sigsetjmp) __sigsetjmp attribute_hidden;
# endif

/* Check jmp_buf sizes, alignments and offsets.  */
# include <stddef.h>
# include <jmp_buf-macros.h>

# define STR_HELPER(x) #x
# define STR(x) STR_HELPER(x)

# define TEST_SIZE(type, size) \
  _Static_assert (sizeof (type) == size, \
		  "size of " #type " != " \
		  STR (size))
# define TEST_ALIGN(type, align) \
  _Static_assert (__alignof__ (type) == align , \
		  "align of " #type " != " \
		  STR (align))
# define TEST_OFFSET(type, member, offset) \
  _Static_assert (offsetof (type, member) == offset, \
		  "offset of " #member " field of " #type " != " \
		  STR (offset))

/* Check if jmp_buf have the expected sizes.  */
TEST_SIZE (jmp_buf, JMP_BUF_SIZE);
TEST_SIZE (sigjmp_buf, SIGJMP_BUF_SIZE);

/* Check if jmp_buf have the expected alignments.  */
TEST_ALIGN (jmp_buf, JMP_BUF_ALIGN);
TEST_ALIGN (sigjmp_buf, SIGJMP_BUF_ALIGN);

/* Check if internal fields in jmp_buf have the expected offsets.  */
TEST_OFFSET (struct __jmp_buf_tag, __mask_was_saved,
	     MASK_WAS_SAVED_OFFSET);
TEST_OFFSET (struct __jmp_buf_tag, __saved_mask,
	     SAVED_MASK_OFFSET);
#endif

#endif
