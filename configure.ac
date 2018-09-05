AC_INIT([fibonacci],[1.0])
AC_CONFIG_HEADERS([config.h:config.in])

AC_TYPE_UINTMAX_T()
AC_CHECK_HEADERS([gmp.h],
    [
     AC_DEFINE([ELEM_T],[mpz_t],[Defines the type of variable to use for the fibStruct data array])
     AC_DEFINE([FIBSET(set, val)],[do {mpz_init_set_ui(set,val);} while(0)],[Defines the function for initializing and setting fibonacci values])
     AC_DEFINE([FIBADD(fib, val1, val2)],[do{mpz_init(fib); mpz_add(fib,val1,val2);} while(0)],[Defines the function for adding two fibonacci values together])
    ],
    [
     AC_DEFINE([ELEM_T],[uintmax_t],[Defines the type of variable to use for the fibStruct data array])
     AC_DEFINE([FIBSET(set, val)],[do {*set = val;} while(0)],[Defines the function for initializing and setting fibonacci values])
     AC_DEFINE([FIBADD(fib, val1, val2)],[do {*fib = val1 + val2;} while(0)],[Defines the function for adding two fibonacci values together])
    ])
#@TODO FIBGET
AC_CHECK_HEADERS([readline/readline.h])

AC_OUTPUT()
