AC_INIT([fibonacci],[1.0])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AC_CONFIG_HEADERS([config.h:config.in])
AC_CONFIG_FILES([Makefile])

AC_PROG_CC()

AC_TYPE_UINTMAX_T()
AC_ARG_WITH([gmp],[AS_HELP_STRING([--without-gmp],[Disables usage of the GNU Multi-Precision libaray @<:@default=check@:>@])],[],[with_gmp=check])

AS_IF([test "x$with_gmp" != xno],
    [AC_CHECK_HEADERS([gmp.h],
	[
	 AC_DEFINE([ELEM_T],[mpz_t],[Defines the type of variable to use for the fibStruct data array])
	 AC_DEFINE([FIBSET(set, val)],[do {mpz_init_set_ui(set,val);} while(0)],[Defines the function for initializing and setting fibonacci values])
	 AC_DEFINE([FIBADD(fib, val1, val2)],[do{mpz_init(fib); mpz_add(fib,val1,val2);} while(0)],[Defines the function for adding two fibonacci values together])
	],
	[if test "x$with_gmp" != xcheck; then
	    AC_MSG_ERROR([Fatal error: GMP explicitly requested but not found])
	 else
	    AC_DEFINE([ELEM_T],[uintmax_t],[Defines the type of variable to use for the fibStruct data array])
	    AC_DEFINE([FIBSET(set, val)],[do {set = val;} while(0)],[Defines the function for initializing and setting fibonacci values])
	    AC_DEFINE([FIBADD(fib, val1, val2)],[do {fib = val1 + val2;} while(0)],[Defines the function for adding two fibonacci values together])
	 fi
	],
    )],
    [
	AC_DEFINE([ELEM_T],[uintmax_t],[Defines the type of variable to use for the fibStruct data array])
	AC_DEFINE([FIBSET(set, val)],[do {set = val;} while(0)],[Defines the function for initializing and setting fibonacci values])
	AC_DEFINE([FIBADD(fib, val1, val2)],[do {fib = val1 + val2;} while(0)],[Defines the function for adding two fibonacci values together])
    ]
)

AC_CHECK_HEADERS([stdint.h inttypes.h])

#@TODO FIBGET

AC_CHECK_HEADERS([readline/readline.h],[],[
    AC_CHECK_HEADERS([unistd.h],[],[
	AC_MSG_ERROR([Fatal error: Cannot find either readline/readline.h or unistd.h, one of which is required.])
    ])
])

# Now, process if we want to disable any of the above features

AC_OUTPUT()
