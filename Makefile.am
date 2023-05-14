.PHONY: doc

SUBDIRS           = . test/pattern
bin_SCRIPTS       = bin/bgen bin/lib.bg
lib_LTLIBRARIES   = libjj.la
libjj_la_SOURCES  = src/errno.cpp src/pattern.cpp
nobase_include_HEADERS  = jj/errno.h jj/pattern.h
AM_CPPFLAGS       = -Iinclude
ACLOCAL_AMFLAGS   = -Im4

#----------------------------------------------------------------------------
# doxygen
if HAVE_DOXYGEN

doc:
	cd src; doxygen
	cd html; rsync -rltvz --delete ./* wtech.jp:/var/www/doc.mywiki.bar/jj-pattern/
# NOTE: '-rlt' options above means -a without -p,-t,g,-o,-D to follow host stick bit 'chmod g+s'
endif
#----------------------------------------------------------------------------
