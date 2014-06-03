/* GStreamer
 * Copyright (C) 2008 Wim Taymans <wim.taymans at gmail.com>
 * Copyright (c) 2012 enthusiasticgeek <enthusiasticgeek@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */


//Edited by: enthusiasticgeek (c) 2012 for Stack Overflow Sept 11, 2012

//###########################################################################
//Important
//###########################################################################

//On ubuntu: sudo apt-get install libgstrtspserver-0.10-0 libgstrtspserver-0.10-dev

//Play with VLC
//rtsp://localhost:8554/test

//video decode only:  gst-launch -v rtspsrc location="rtsp://localhost:8554/test" ! rtph264depay ! ffdec_h264 ! autovideosink
//audio and video: 
//gst-launch -v rtspsrc location="rtsp://localhost:8554/test" name=demux demux. ! queue ! rtph264depay ! ffdec_h264 ! ffmpegcolorspace ! autovideosink sync=false demux. ! queue ! rtppcmadepay  ! alawdec ! autoaudiosink

//###########################################################################
#include <gst/gst.h>

//#include <gst/rtsp-server/rtsp-server.h>
//gstrtspbase64.h  gstrtspconnection.h  gstrtspdefs.h  gstrtsp-enumtypes.h  gstrtspextension.h  gstrtspmessage.h  gstrtsprange.h  gstrtsptransport.h  gstrtspurl.h
#include <gst/rtsp/gstrtspdefs.h>

/* define this if you want the resource to only be available when using
 * user/admin as the password */
#undef WITH_AUTH

/* this timeout is periodically run to clean up the expired sessions from the
 * pool. This needs to be run explicitly currently but might be done
 * automatically as part of the mainloop. */
static gboolean
timeout (GstRTSPServer * server, gboolean ignored)
{
  GstRTSPSessionPool *pool;

  pool = gst_rtsp_server_get_session_pool (server);
  gst_rtsp_session_pool_cleanup (pool);
  g_object_unref (pool);

  return TRUE;
}

int
main (int argc, char *argv[])
{
  GMainLoop *loop;
  GstRTSPServer *server;
  GstRTSPMediaMapping *mapping;
  GstRTSPMediaFactory *factory;
#ifdef WITH_AUTH
  GstRTSPAuth *auth;
  gchar *basic;
#endif

  gst_init (&argc, &argv);

  loop = g_main_loop_new (NULL, FALSE);

  /* create a server instance */
  server = gst_rtsp_server_new ();

  /* get the mapping for this server, every server has a default mapper object
   * that be used to map uri mount points to media factories */
  mapping = gst_rtsp_server_get_media_mapping (server);

#ifdef WITH_AUTH
  /* make a new authentication manager. it can be added to control access to all
   * the factories on the server or on individual factories. */
  auth = gst_rtsp_auth_new ();
  basic = gst_rtsp_auth_make_basic ("user", "admin");
  gst_rtsp_auth_set_basic (auth, basic);
  g_free (basic);
  /* configure in the server */
  gst_rtsp_server_set_auth (server, auth);
#endif

  /* make a media factory for a test stream. The default media factory can use
   * gst-launch syntax to create pipelines.
   * any launch line works as long as it contains elements named pay%d. Each
   * element with pay%d names will be a stream */
  factory = gst_rtsp_media_factory_new ();

  gst_rtsp_media_factory_set_launch (factory, "( "
      "videotestsrc ! video/x-raw-yuv,width=320,height=240,framerate=10/1 ! "
      "x264enc ! queue ! rtph264pay name=pay0 pt=96 ! audiotestsrc ! audio/x-raw-int,rate=8000 ! alawenc ! rtppcmapay name=pay1 pt=97 "")");

  /* attach the test factory to the /test url */
  gst_rtsp_media_mapping_add_factory (mapping, "/test", factory);

  /* don't need the ref to the mapper anymore */
  g_object_unref (mapping);

  /* attach the server to the default maincontext */
  if (gst_rtsp_server_attach (server, NULL) == 0)
    goto failed;

  /* add a timeout for the session cleanup */
  g_timeout_add_seconds (2, (GSourceFunc) timeout, server);

  /* start serving, this never stops */
  g_main_loop_run (loop);

  return 0;

  /* ERRORS */
failed:
  {
    g_print ("failed to attach the server\n");
    return -1;
  }
}

