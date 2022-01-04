/*
 * Copyright (C) 2022 Alexandros Theodotou <alex at zrythm dot org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * This file incorporates work covered by the following copyright and
 * permission notice:
 *
  Copyright 2012-2019 David Robillard <http://drobilla.net>

  Permission to use, copy, modify, and/or distribute this software for any
  purpose with or without fee is hereby granted, provided that the above
  copyright notice and this permission notice appear in all copies.

  THIS SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include "passthrough-config.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"

#include <lv2/atom/util.h>
#include <lv2/ui/ui.h>

#include <gtk/gtk.h>

typedef struct PassthroughUi
{
  /** Port values (TODO). */

  /* Non-port values */

  PluginCommon         common;

  LV2UI_Write_Function write;
  LV2UI_Controller     controller;

  /**
   * This is the window passed in the features from
   * the host.
   */
  void *               parent_window;

  /**
   * Resize handle for the parent window.
   */
  LV2UI_Resize *       resize;

  /** Box containing the contents. */
  GtkBox *             box;

} PassthroughUi;

static LV2UI_Handle
instantiate (
  const LV2UI_Descriptor*   descriptor,
  const char*               plugin_uri,
  const char*               bundle_path,
  LV2UI_Write_Function      write_function,
  LV2UI_Controller          controller,
  LV2UI_Widget*             widget,
  const LV2_Feature* const* features)
{
  PassthroughUi * self = calloc (1, sizeof (PassthroughUi));
  self->write = write_function;
  self->controller = controller;

  PluginCommon * common = &self->common;

#define HAVE_FEATURE(x) \
  (!strcmp(features[i]->URI, x))

  for (int i = 0; features[i]; ++i)
    {
      if (HAVE_FEATURE (LV2_UI__parent))
        {
          self->parent_window = features[i]->data;
        }
      else if (HAVE_FEATURE (LV2_UI__resize))
        {
          self->resize =
            (LV2UI_Resize*)features[i]->data;
        }
      else if (HAVE_FEATURE (LV2_URID__map))
        {
          common->map =
            (LV2_URID_Map *) features[i]->data;
        }
      else if (HAVE_FEATURE (LV2_LOG__log))
        {
          common->log =
            (LV2_Log_Log *) features[i]->data;
        }
    }

#undef HAVE_FEATURE

  if (!common->map)
    {
      lv2_log_error (
        &common->logger, "Missing feature urid:map\n");
    }

  /* map uris */
  map_common_uris (common->map, &common->uris);

  lv2_atom_forge_init (
    &common->forge, common->map);

  /* create UI and set the native window to the
   * widget */
  self->box =
    GTK_BOX (
      gtk_box_new (GTK_ORIENTATION_VERTICAL, 2));
  GtkWidget * lbl = gtk_label_new ("Test label");
  gtk_box_append (GTK_BOX (self->box), lbl);
  *widget = (LV2UI_Widget) self->box;

  return self;
}

static void
cleanup (LV2UI_Handle handle)
{
  PassthroughUi * self = (PassthroughUi *) handle;
  PluginCommon * common = &self->common;

  free (self);
}

/**
 * Port event from the plugin.
 */
static void
port_event (
  LV2UI_Handle handle,
  uint32_t     port_index,
  uint32_t     buffer_size,
  uint32_t     format,
  const void*  buffer)
{
  PassthroughUi * self = (PassthroughUi *) handle;
  PluginCommon * common = &self->common;

  /* TODO */
}

/* Optional non-embedded UI show interface. */
static int
ui_show (LV2UI_Handle handle)
{
  PassthroughUi * self = (PassthroughUi *) handle;

  return 0;
}

/* Optional non-embedded UI hide interface. */
static int
ui_hide (LV2UI_Handle handle)
{
  PassthroughUi * self = (PassthroughUi *) handle;

  return 0;
}

/**
 * LV2 idle interface for optional non-embedded
 * UI.
 */
static int
ui_idle (LV2UI_Handle handle)
{
  PassthroughUi * self = (PassthroughUi *) handle;

  return 0;
}

/**
 * LV2 resize interface for the host.
 */
static int
ui_resize (
  LV2UI_Feature_Handle handle, int w, int h)
{
  PassthroughUi * self = (PassthroughUi *) handle;
  return 0;
}

/**
 * Called by the host to get the idle and resize
 * functions.
 */
static const void*
extension_data (const char* uri)
{
  static const LV2UI_Idle_Interface idle = {
    ui_idle };
  static const LV2UI_Resize resize = {
    0 ,ui_resize };
  static const LV2UI_Show_Interface show = {
    ui_show, ui_hide };
  if (!strcmp(uri, LV2_UI__idleInterface))
    {
      return &idle;
    }
  if (!strcmp(uri, LV2_UI__resize))
    {
      return &resize;
    }
  if (!strcmp(uri, LV2_UI__showInterface))
    {
      return &show;
    }
  return NULL;
}

static const LV2UI_Descriptor descriptor = {
  PLUGIN_UI_URI_QUOTED,
  instantiate,
  cleanup,
  port_event,
  extension_data,
};

LV2_SYMBOL_EXPORT
const LV2UI_Descriptor*
lv2ui_descriptor (uint32_t index)
{
  switch (index)
    {
    case 0:
      return &descriptor;
    default:
      return NULL;
    }
}
