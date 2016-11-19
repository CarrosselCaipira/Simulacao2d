ACLOCAL_AMFLAGS=-I m4

SUBDIRS = . @LTDLDIR@ rcssbase

LIBTOOL_DEPS = @LIBTOOL_DEPS@
libtool:	$(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck

CLEANFILES = \
*~ \
core \
*.tar.bz2 \
*.tar.gz \
*.zip

EXTRA_DIST = \
BUGS \
AUTHORS \
ChangeLog \
NEWS \
README \
INSTALL \
COPYING.LESSER \
COPYING.rtf \
PLATFORMS \
SConstruct

##DISTCLEANFILES = @INCLUDE_MK_FILE@

##@INCLUDE_MK@
