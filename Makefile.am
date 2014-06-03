AUTOMAKE_OPTIONS = foreign
AM_CPPFLAGS = -I@PKG_CONFIG_SYSROOT_DIR@/usr/include/gstreamer-1.0 -I@PKG_CONFIG_SYSROOT_DIR@/usr/lib/glib-2.0/include -I@PKG_CONFIG_SYSROOT_DIR@/usr/include/glib-2.0
ACLOCAL_AMFLAGS = -I m4

CFLAGS = -Wall -pedantic
include_HEADERS = rtsp.h

bin_PROGRAMS=rtsp
rtsp_SOURCES=rtsp.c
rtsp_LDADD= @PKG_CONFIG_SYSROOT_DIR@/usr/lib/libgstrtspserver-1.0.la 
