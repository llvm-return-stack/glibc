long-double-fcts = yes

ifeq ($(subdir),elf)
sysdep-dl-routines += tlsdesc dl-tlsdesc
gen-as-const-headers += dl-link.sym
endif

ifeq ($(subdir),csu)
gen-as-const-headers += tlsdesc.sym
endif

ifeq ($(subdir),gmon)
CFLAGS-mcount.c += -mgeneral-regs-only
endif

ifeq ($(subdir),debug)
defines += -DWITH_RETURN_STACK_SUPPORT
sysdep_routines += ____safe_longjmp_chk
endif

ifeq ($(subdir),setjmp)
defines += -DWITH_RETURN_STACK_SUPPORT
sysdep_routines += safe_setjmp __safe_longjmp
endif
