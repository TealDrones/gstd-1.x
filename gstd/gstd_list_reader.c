/*
 * Gstreamer Daemon - Gst Launch under steroids
 * Copyright (C) 2017 RidgeRun Engineering <support@ridgerun.com>
 *
 * This file is part of Gstd.
 *
 * Gstd is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Gstd is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Gstd.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <gst/gst.h>

#include "gstd_ireader.h"
#include "gstd_list_reader.h"
#include "gstd_object.h"
#include "gstd_list.h"
#include "gstd_property_int.h"

/* Gstd Core debugging category */
GST_DEBUG_CATEGORY_STATIC (gstd_list_reader_debug);
#define GST_CAT_DEFAULT gstd_list_reader_debug

#define GSTD_DEBUG_DEFAULT_LEVEL GST_LEVEL_INFO

static GstdObject * gstd_list_reader_read (GstdIReader * iface,
    GstdObject * object, const gchar * name);

static GstdObject * gstd_list_reader_read_count (GstdIReader * iface,
    GstdObject * object);
static GstdObject * gstd_list_reader_read_child (GstdIReader * iface,
    GstdObject * object, const gchar * name);


typedef struct _GstdListReaderClass GstdListReaderClass;

/**
 * GstdListReader:
 * A wrapper for the conventional list_reader
 */
struct _GstdListReader
{
  GObject parent;
};

struct _GstdListReaderClass
{
  GObjectClass parent_class;
};


static void
gstd_ireader_interface_init (GstdIReaderInterface * iface)
{
  iface->read = gstd_list_reader_read;
}

G_DEFINE_TYPE_WITH_CODE (GstdListReader, gstd_list_reader,
    G_TYPE_OBJECT, G_IMPLEMENT_INTERFACE (GSTD_TYPE_IREADER,
        gstd_ireader_interface_init));

static void
gstd_list_reader_class_init (GstdListReaderClass * klass)
{
  guint debug_color;

  /* Initialize debug category with nice colors */
  debug_color = GST_DEBUG_FG_BLACK | GST_DEBUG_BOLD | GST_DEBUG_BG_WHITE;
  GST_DEBUG_CATEGORY_INIT (gstd_list_reader_debug, "gstdlistreader",
      debug_color, "Gstd Pipeline Reader category");
}

static void
gstd_list_reader_init (GstdListReader * self)
{
  GST_INFO_OBJECT (self, "Initializing list reader");
}

static GstdObject *
gstd_list_reader_read (GstdIReader * iface, GstdObject * object, const gchar * name)
{
    GstdObject * resource;
    
    g_return_val_if_fail (iface, NULL);
    g_return_val_if_fail (GSTD_IS_LIST(object), NULL);
    g_return_val_if_fail (name, NULL);

    /* In the case of lists, "count" is a keyword */
    if (!g_strcmp0 ("count", name)) {
	resource = gstd_list_reader_read_count (iface, object);
    } else {
	resource = gstd_list_reader_read_child (iface, object, name);
    }

    if (!resource) {
	GST_ERROR_OBJECT (iface, "No resource %s in %s", name, GST_OBJECT_NAME(object));
    }

    if (!GSTD_IS_OBJECT(resource)) {
	GST_ERROR_OBJECT (iface, "%s is not a valid resource", name);
	g_object_unref (resource);
	resource = NULL;
    }
    
    return resource;
}


static GstdObject *
gstd_list_reader_read_count (GstdIReader * iface,
    GstdObject * object)
{
    GstdPropertyInt * count_value;

    g_return_val_if_fail (iface, NULL);
    g_return_val_if_fail (object, NULL);

    count_value = g_object_new(GSTD_TYPE_PROPERTY_INT, "name", "count", "target", object, NULL);
    return count_value;
}

static GstdObject *
gstd_list_reader_read_child (GstdIReader * iface,
    GstdObject * object, const gchar * name)
{
    g_return_val_if_fail (iface, NULL);
    g_return_val_if_fail (object, NULL);
    g_return_val_if_fail (name, NULL);

    return gstd_list_find_child (GSTD_LIST(object), name);
}
