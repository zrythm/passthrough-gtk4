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
 */

#include "passthrough-config.h"

#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "common.h"

typedef struct Passthrough
{
  /** Plugin ports. */
  const LV2_Atom_Sequence * control;
  LV2_Atom_Sequence *       notify;
  const float *             cv_in;
  const float *             audio_in;
  const float *             control_in;

  /* outputs */
  float *                   cv_out;
  float *                   audio_out;
  LV2_Atom_Sequence *       midi_out;

  PluginCommon              common;

} Passthrough;

static LV2_Handle
instantiate (
  const LV2_Descriptor*     descriptor,
  double                    rate,
  const char*               bundle_path,
  const LV2_Feature* const* features)
{
  Passthrough * self = calloc (1, sizeof (Passthrough));

  PluginCommon * common = &self->common;

  common->samplerate = rate;

  int ret =
    plugin_common_instantiate (
      common, features, 0);
  if (ret)
    goto fail;

  /* map uris */
  map_common_uris (common->map, &common->uris);

  /* init atom forge */
  lv2_atom_forge_init (
    &common->forge, common->map);

  /* init logger */
  lv2_log_logger_init (
    &common->logger, common->map, common->log);

  return (LV2_Handle) self;

fail:
  free (self);
  return NULL;
}

static void
connect_port (
  LV2_Handle instance,
  uint32_t   port,
  void *     data)
{
  Passthrough * self = (Passthrough*) instance;

  switch ((PortIndex) port)
    {
    case PASSTHROUGH_CONTROL:
      self->control =
        (const LV2_Atom_Sequence *) data;
      break;
    case PASSTHROUGH_NOTIFY:
      self->notify =
        (LV2_Atom_Sequence *) data;
      break;
    case PASSTHROUGH_CV_IN:
      self->cv_in = (const float *) data;
      break;
    case PASSTHROUGH_AUDIO_IN:
      self->audio_in = (const float *) data;
      break;
    case PASSTHROUGH_CONTROL_IN:
      self->control_in = (const float *) data;
      break;
    case PASSTHROUGH_CV_OUT:
      self->cv_out = (float *) data;
      break;
    case PASSTHROUGH_AUDIO_OUT:
      self->audio_out = (float *) data;
      break;
    default:
      break;
    }
}

static void
activate (
  LV2_Handle instance)
{
  /*Passthrough * self = (Passthrough *) instance;*/
}

static void
run (
  LV2_Handle instance,
  uint32_t n_samples)
{
  Passthrough * self = (Passthrough *) instance;
  PluginCommon * common = &self->common;

  /* TODO MIDI */
#if 0
  /* write an empty Sequence header to the output */
  lv2_atom_sequence_clear (self->midi_out);
  self->midi_out->atom.type = self->control->atom.type;
#endif

  /* read incoming events from host and UI */
  LV2_ATOM_SEQUENCE_FOREACH (
    self->control, ev)
    {
      if (lv2_atom_forge_is_object_type (
            &common->forge, ev->body.type))
        {
          const LV2_Atom_Object * obj =
            (const LV2_Atom_Object*)&ev->body;
          if (obj->body.otype ==
                common->uris.time_Position)
            {
              /*update_position_from_atom_obj (*/
                /*&self->common, obj);*/
              /*xport_changed = 1;*/
            }
        }
    }

  for (uint32_t i = 0; i < n_samples; i++)
    {
      self->cv_out[i] = self->cv_in[i];
      self->audio_out[i] = self->audio_in[i];

    }
}

static void
deactivate (
  LV2_Handle instance)
{
}

static void
cleanup (
  LV2_Handle instance)
{
  free (instance);
}

static const void*
extension_data (
  const char* uri)
{
  return NULL;
}

static const LV2_Descriptor descriptor = {
  PLUGIN_URI_QUOTED,
  instantiate,
  connect_port,
  activate,
  run,
  deactivate,
  cleanup,
  extension_data
};

LV2_SYMBOL_EXPORT
const LV2_Descriptor*
lv2_descriptor (
  uint32_t index)
{
  switch (index)
    {
    case 0:
      return &descriptor;
    default:
      return NULL;
    }
}
