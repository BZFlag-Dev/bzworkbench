SUBDIRS = src

EXTRA_DIST = \
	AUTHORS \
	BUGS \
	COPYING \
	DEVINFO \
	INSTALL \
	MSVC \
	Makefile.am \
	README \
	README.MSWindows \
	TODO \
	autogen.sh \
	bzwb_icon.png \
	configure.ac \
	include \
	platform \
	plugins \
	share

MAINTAINERCLEANFILES = \
	Makefile.in \
	aclocal.m4 \
	config.h.in \
	config.h.in~ \
	configure \
	depcomp \
	install-sh \
	missing

# certain subdirectories are not distributed
dist-hook:
	find $(distdir) -type d \( -name .deps -o -name .svn -o -name autom4te.cache \) -prune -exec rm -rf {} \;
