AUTOMAKE_OPTIONS = foreign
AM_CPPFLAGS = -I@PKG_CONFIG_SYSROOT_DIR@/usr/include/gstreamer-1.0 -I@PKG_CONFIG_SYSROOT_DIR@/usr/lib/glib-2.0/include -I@PKG_CONFIG_SYSROOT_DIR@/usr/include/glib-2.0
ACLOCAL_AMFLAGS = -I m4

CFLAGS = -Wall -pedantic
include_HEADERS = rtsp-server.h

bin_PROGRAMS=rtsp-server
rtsp_server_SOURCES=rtsp-server.c
rtsp_server_LDADD= @PKG_CONFIG_SYSROOT_DIR@/usr/lib/libgstrtspserver-1.0.la 
