AC_INIT([Scian Financial Tools], [0.1], [contact@sciantechnologies.com], [scian_financial_tools], [http://www.sciantechnologies.com/])
AC_PREREQ([2.59])
AM_INIT_AUTOMAKE([1.10 no-define])
AC_CONFIG_HEADERS([config.h])
AC_PROG_CXX
AC_LIBTOOL_DLOPEN
AC_PROG_LIBTOOL
AM_PROG_LIBTOOL
AC_SUBST(LTLIBOBJS)

##################################################
# MAMA: --with-mama
# Whether or not to link in the Avis middleware
##################################################
# Whether to build and where to find the MAMA API headers and
# libraries (usually /usr/local):
MAMA_HOME=[${MAMA_HOME}]
AC_ARG_WITH(
    mama,
    [AC_HELP_STRING([--with-mama=DIR],
                    [Location of MAMA API include/library files in DIR])],
    [
     if test -d "$withval"; then
         MAMA_HOME="$withval"
     fi
     AC_CHECK_FILES(
         [$MAMA_HOME/include/mama/mama.h],
         [
		CPPFLAGS="$CPPFLAGS -I$MAMA_HOME/include"
		LDFLAGS="$LDFLAGS -L$MAMA_HOME/lib"
		AC_MSG_NOTICE([MAMA HOME set to $MAMA_HOME])
		with_mama=true
         ],
         [
            AC_MSG_ERROR([Valid MAMA installation not found at: $MAMA_HOME)])
         ])
    ])
AM_CONDITIONAL(WITH_MAMA, test x$with_mama = "xtrue")

AC_CHECK_HEADERS([mama/mama.h])

# Produce the makefiles - this *must* come last
AC_OUTPUT([Makefile \
        framework/Makefile \
        mama2rss/Makefile \
        thirdparty/Makefile \
        thirdparty/mongoose/Makefile \
        ])

