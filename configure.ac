AC_PREREQ([2.69])
AC_INIT([fibonacci],[1.0],[thomas.lenz96@gmail.com])

# Enable automake warnings, make them fatal since they're less silly than GCC warnings, and disable GNU Standard enforcement
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

# Make a header defining various features in order to make the info available to the pre-processor
AC_CONFIG_HEADERS([config.h:config.in])

# Make a Makefile out of this with standard targets
AC_CONFIG_FILES([Makefile])

# Verify we have a C compiler, and set it to the variable CC here and in the Makefile
AC_PROG_CC

# Set inline to either something the compiler can use, or ignore the keyword if the compiler can't work with it
AC_C_INLINE

# If the compiler/libraries do not define an appropriate size_t, do so ourselves
AC_TYPE_SIZE_T

# Checks for libraries and verifies they have viable functions
AC_CHECK_LIB([gmp], [mpz_init])
AC_CHECK_LIB([readline], [readline])

# Verify strtoumax is defined in inttypes.h
AC_CHECK_FUNCS([strtoumax],[],[AC_MSG_ERROR([Fatal error. The function strtoumax is not defined. Per C99, it should be defined in inttypes.h])])

# Verifies standard headers we're including actually exist
AC_CHECK_HEADERS([inttypes.h stdint.h stdlib.h string.h unistd.h])

# I'm putting two statements here looking for uintmax_t. One is to guarentee it is defined, whether natively or via macro
# The other is to tell automake whether or not stdint.h and inttypes.h need to be included, since I cannot set variables in the first command
AC_TYPE_UINTMAX_T
AC_CHECK_TYPE([uintmax_t],[have_uintmax_t=yes],[have_uintmax_t=no])

# Tell Makefile whether or not uintmax_t is native. This will affect header requirements
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

# Allow GMP and GNU Readline libs to be manually disabled in configure
AC_ARG_WITH([gmp],[AS_HELP_STRING([--without-gmp],[Disables usage of the GNU Multi-Precision libaray @<:@default=check@:>@])],[with_gmp=no],[with_gmp=check])
AC_ARG_WITH([readline],[AS_HELP_STRING([--without-readline],[Disables usage of the GNU Realine library @<:@default=check@:>@])],[with_readline=no],[with_readline=check])

# If GMP isn't manually disabled, check for its existance. Set appropriate macros in config.h, such that whichever one we're using is irrelevant to the source code we're writing. The pre-processor will fill in the blanks based on configure's instructions.
AS_IF([test "x$with_gmp" != xno],
[
    AC_CHECK_HEADERS([gmp.h],
    [
	AC_DEFINE([ELEM_T],[mpz_t],[Defines the type of variable to use for the fibStruct data array using GMP])
	AC_DEFINE([FIBSET(set, val)],[do {mpz_init_set_ui(set,val);} while(0)],[Defines the function for initializing and setting fibonacci values using GMP])
	AC_DEFINE([FIBADD(fib, val1, val2)],[do{mpz_init(fib); mpz_add(fib,val1,val2);} while(0)],[Defines the function for adding two fibonacci values together using GMP])
	AC_DEFINE([FIBGET(fibElem, val)],[do{*val = mpz_get_str(NULL,10,fibElem);}while(0)],[Defines the function for getting the fibonacci value from a fibonacci structure using GMP])
	AM_CONDITIONAL([WITH_GMP],[true])
	AC_DEFINE([WITH_GMP],[1],[Define as 1 if you are using GNU Multi-Precision libraries])
    ],
    [
     # This shouldn't be here, since I don't have an option to get here. However, if we do, there's a handle for it. If GMP is manually requested (it cannot be), but cannot be found, exit
     if test "x$with_gmp" != xcheck; then
	AC_MSG_ERROR([Fatal error: GMP explicitly requested but not found])
     else
	using_gmp=no
     fi
    ],
)],
[
    using_gmp=no
])

# Since there are multiple routes to this, save some time on modification and trigger it if needed
AS_IF([test "x$using_gmp" = xno],
[
    AM_CONDITIONAL([WITH_GMP],[false])
    AC_DEFINE([ELEM_T],[uintmax_t],[Defines the type of variable to use for the fibStruct data array without using GMP])
    AC_DEFINE([FIBSET(set, val)],[do {set = val;} while(0)],[Defines the function for initializing and setting fibonacci values without using GMP])
    AC_DEFINE([FIBADD(fib, val1, val2)],[do {fib = val1 + val2;} while(0)],[Defines the function for adding two fibonacci values together without using GMP])
    AC_DEFINE([FIBGET(fibElem,val)],[do {*val = malloc(sizeof(char) * 256); sprintf(*val,"%ju",fibElem);} while(0)],[Defines the function for getting the fibonacci value from a fibonacci structure without using GMP])
])

# Now, do the same mess for Readline! That one should be a lot less bulky considering the fact that it won't need so many definitions
AS_IF([test "x$with_readline" != xno],
[
    AC_CHECK_HEADERS([readline/readline.h],
    [
	using_readline_readline_h=yes
	AC_DEFINE([READ_STDIN(str,prompt)],[do {*str = readline(prompt);} while(0)],[Defines the function used to read from stdin the number of fibonacci numbers to calculate with GNU Realine])
	AC_DEFINE([WITH_READLINE_READLINE_H],[1],[Define as 1 if you are using GNU Realine libraries])
    ],
    [
	using_readline_readline_h=no
    ])
],
[
    using_readline_readline_h=no
])

# If we don't have (or don't want) GNU Readline, verify we have a unistd.h to pull. If not, error out
AS_IF([test "x$using_readline_readline_h" = xno],
[
    AC_CHECK_HEADERS([unistd.h],
    [
	using_stdio_h=yes
    ],
    [
	using_stdio_h=no
	AC_MSG_ERROR([Fatal error: Cannot find either readline/readline.h or unistd.h, one of which is required.])
    ])
])

# If we are using the alternative, we need to set the function we're using to read from stdin
AS_IF([test "x$using_stdio_h" = xyes],
[
    AC_DEFINE([READ_STDIN(str,prompt)],[do {*str = malloc(sizeof(char) * 256); printf(prompt); fgets(*str,256,stdin);} while(0)],[Defines the function used to read from stdin the number of fibonacci numbers to calculate without GNU Readline])
])

# Same as above, this needs to be able to be processed every time, so we check it outside the if statement
AM_CONDITIONAL([WITH_READLINE_READLINE_H],[test "x$using_readline_readline_h" = xyes])
AM_CONDITIONAL([WITH_STDIO_H],[test "x$using_readline_readline_h" = xno && "x$using_unistd_h" = xyes])

# Let autoconf know it's got all the instructions, go to work!
AC_OUTPUT
