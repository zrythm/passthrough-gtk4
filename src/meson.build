# Copyright (C) 2022 Alexandros Theodotou <alex at zrythm dot org>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

pl_uri = 'https://www.zrythm.org/plugins/passthrough-gtk4'
pl_ui_uri = pl_uri + '#UI'
cdata = configuration_data ()
cdata.set ('PLUGIN_URI', pl_uri)
cdata.set_quoted ('PLUGIN_URI_QUOTED', pl_uri)
cdata.set_quoted ('PLUGIN_UI_URI_QUOTED', pl_ui_uri)

plugin_lv2_dir = lv2dir / 'PassthroughGtk4.lv2'

config_h = configure_file (
  output: 'passthrough-config.h',
  configuration: cdata)
manifest_ttl = configure_file (
  input: 'manifest.ttl.in',
  output: 'manifest.ttl',
  configuration: cdata,
  install: true,
  install_dir: plugin_lv2_dir)
dsp_ttl = configure_file (
  input: 'dsp.ttl.in',
  output: 'PassthroughGtk4_dsp.ttl',
  configuration: cdata,
  install: true,
  install_dir: plugin_lv2_dir)
ui_ttl = configure_file (
  input: 'ui.ttl.in',
  output: 'PassthroughGtk4_ui.ttl',
  configuration: cdata,
  install: true,
  install_dir: plugin_lv2_dir)

# create dsp shared library
inc = include_directories(['.'])
dsp_lib = shared_library (
  'PassthroughGtk4_dsp',
  name_prefix: '',
  sources: [ config_h, 'dsp.c' ],
  dependencies: [
    cc.find_library ('m'), lv2_dep ],
  include_directories: inc,
  install: true,
  install_dir: plugin_lv2_dir,
  c_args: common_cflags,
  )

# create ui shared library
ui_lib = shared_library (
  'PassthroughGtk4_ui',
  name_prefix: '',
  sources: [ config_h, 'ui.c' ],
  dependencies: [ lv2_dep, gtk_dep ],
  include_directories: inc,
  install: true,
  install_dir: plugin_lv2_dir,
  c_args: common_cflags,
  )
