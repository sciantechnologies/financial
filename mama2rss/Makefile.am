FRAMEWORK_DIR		=	../framework
include ../framework/Makefile.am

VPATH			=	@srcdir@/../framework/

bin_PROGRAMS		=	mama2rss
AM_LDFLAGS		=	-L@srcdir@/../framework/ -L@srcdir@/../thirdparty/mongoose
AM_CPPFLAGS		=	-I@srcdir@/../thirdparty/
LDADD			=	-lmongoose -lwombatcommon -lmama -lmamacpp -lrt -ldl -lz

mama2rss_SOURCES	=	mama2rss.cpp $(SCIAN_FINANCIAL_FRAMEWORK_SRC)
