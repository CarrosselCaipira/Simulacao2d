CLEANFILES = \
*~ \
core \
install_files

AM_CXXFLAGS = -Wall -I$(top_srcdir)
AM_LDFLAGS = -L../

if BUILD_TEST

check_PROGRAMS = tcpsockettest

tcpsockettest_SOURCES = tcpsockettest.cpp
tcpsockettest_LDADD = @TEST_LIBS@ -lrcssnet

TESTS = tcpsockettest

endif

install_files: Makefile Makefile.am
	@rm -f install_files
	@touch install_files
	@list='$(pkglib_LTLIBRARIES)'; \
	for p in $$list; do \
		f=`echo $$p | sed -e 's/\.la/.*/'`; \
		echo "%{_libdir}/@PACKAGE@/$$f" >> install_files; \
	done
	@list='$(lib_LTLIBRARIES)'; \
	for p in $$list; do \
		f=`echo $$p | sed -e 's/\.la/.*/'`; \
		echo "%{_libdir}/$$f" >> install_files; \
	done
	@list='$(pkgbin_PROGRAMS)'; \
	for p in $$list; do \
		echo "%{_bindir}/@PACKAGE@/$$p" >> install_files; \
	done
	@list='$(bin_PROGRAMS)'; \
	for p in $$list; do \
		echo "%{_bindir}/$$p" >> install_files; \
	done
	@list='$(pkginclude_HEADERS)'; \
	for p in $$list; do \
		echo "%{_includedir}/@PACKAGE@/$$p" >> install_files; \
	done
	@list='$(include_HEADERS)'; \
	for p in $$list; do \
		echo "%{_includedir}/$$p" >> install_files; \
	done
	@list='$(pkgdata_DATA)'; \
	for p in $$list; do \
		echo "%{_datadir}/@PACKAGE@/$$p" >> install_files; \
	done
	@list='$(data_DATA)'; \
	for p in $$list; do \
		echo "%{_datadir}/$$p" >> install_files; \
	done

#@INCLUDE_MK@
