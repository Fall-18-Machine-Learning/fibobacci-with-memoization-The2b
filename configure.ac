AC_INIT([fibonacci],[1.0])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AC_CONFIG_HEADERS([config.h:config.in])
AC_CONFIG_FILES([Makefile])

AC_PROG_CC()

# I'm putting two statements here looking for uintmax_t. One is to guarentee it is defined, whether natively or via macro
# The other is to tell automake whether or not stdint.h and inttypes.h need to be included
AC_TYPE_UINTMAX_T()
AC_CHECK_TYPE([uintmax_t],[have_uintmax_t=yes],[have_uintmax_t=no])

# Tell Makefile whether or not uintmax_t is native
AM_CONDITIONAL([HAVE_UINTMAX_T],[test "x$have_uintmax_t" = xyes])

# If uintmax_t is native, check which header files we have and update automake vars
AS_IF([test "x$have_uintmax_t" = xyes],
    [
     AC_CHECK_HEADERS(
     	[stdint.h],
	[have_stdint_h=yes],
	[have_stdint_h=no])
     AC_CHECK_HEADERS(
	[inttypes.h],
	[have_inttypes_h=yes],
	[have_inttypes_h=no])
    ]
)

# These will auto-fail if $have_uintmax_t is false, since they will be empty. However, they need to be tested in all builds, or automake may screw stuff up
 AM_CONDITIONAL([HAVE_STDINT_H],[test "x$have_stdint_h" = xyes])
 AM_CONDITIONAL([HAVE_INTTYPES_H],[test "x$have_inttypes_h" = xyes])

# Do a similar check for readline
AC_CHECK_HEADERS([readline/readline.h],[have_readline_readline_h=yes],
    [
     have_readline_readline_h=no
     AC_CHECK_HEADERS([unistd.h],[have_unistd_h = yes],[
	 have_unistd_h = no
	 AC_MSG_ERROR([Fatal error: Cannot find either readline/readline.h or unistd.h, one of which is required.])
    ])
])


AC_ARG_WITH([gmp],[AS_HELP_STRING([--without-gmp],[Disables usage of the GNU Multi-Precision libaray @<:@default=check@:>@])],[with_gmp=no],[with_gmp=check])

AS_IF([test "x$with_gmp" != xno],
    [
     AC_CHECK_HEADERS([gmp.h],
	[
	 AC_DEFINE([ELEM_T],[mpz_t],[Defines the type of variable to use for the fibStruct data array using GMP])
	 AC_DEFINE([FIBSET(set, val)],[do {mpz_init_set_ui(set,val);} while(0)],[Defines the function for initializing and setting fibonacci values using GMP])
	 AC_DEFINE([FIBADD(fib, val1, val2)],[do{mpz_init(fib); mpz_add(fib,val1,val2);} while(0)],[Defines the function for adding two fibonacci values together using GMP])
	 AC_DEFINE([FIBGET(fibElem, val)],[do{*val = mpz_get_str(NULL,10,fibElem);}while(0)],[Defines the function for getting the fibonacci value from a fibonacci structure using GMP])
	 AM_CONDITIONAL([WITH_GMP],[true])
	],
	[
	 if test "x$with_gmp" != xcheck; then
	    AC_MSG_ERROR([Fatal error: GMP explicitly requested but not found])
	 else
	    using_gmp=no
	 fi
	],
    )],
    [
	using_gmp=no
    ]
)

# Since there are multiple routes to this, save some time on modification and trigger it if needed
AS_IF([test "x$using_gmp" = xno],
[
    AM_CONDITIONAL([WITH_GMP],[false])
    AC_DEFINE([ELEM_T],[uintmax_t],[Defines the type of variable to use for the fibStruct data array without using GMP])
    AC_DEFINE([FIBSET(set, val)],[do {set = val;} while(0)],[Defines the function for initializing and setting fibonacci values without using GMP])
    AC_DEFINE([FIBADD(fib, val1, val2)],[do {fib = val1 + val2;} while(0)],[Defines the function for adding two fibonacci values together without using GMP])
    AC_DEFINE([FIBGET(fibElem,val)],[do {*val = malloc(sizeof(char) * 256); sprintf(*val,"%ju",fibElem);} while(0)],[Defines the function for getting the fibonacci value from a fibonacci structure without using GMP])
])

# @TODO Make this optional
AM_CONDITIONAL([HAVE_READLINE_READLINE_H],[test "x$have_readline_readline_h" = xyes])
AM_CONDITIONAL([HAVE_UNISTD_H],[test "x$have_readline_readline_h" = xno && "x$have_unistd_h" = xyes])

AC_OUTPUT()
