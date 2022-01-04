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

cdata = configuration_data ()
cdata.set_quoted (
  'PLUGIN_URI',
  'https://www.zrythm.org/plugins/passthrough-gtk4')

config_h = configure_file (
  output: 'passthrough-config.h',
  configuration: cdata)

pl_deps = [
  config_h,
  cc.find_library ('m'),
  lv2_dep,
  gtk_dep,
  ]

# create dsp shared library
inc = include_directories(['.'])
dsp_lib = shared_library (
  'passthrough-gtk4',
  name_prefix: '',
  sources: [ config_h, 'dsp.c' ],
  dependencies: [
    cc.find_library ('m'), lv2_dep ],
  include_directories: inc,
  install: true,
  install_dir: lv2dir / 'passthrough-gtk4.lv2',
  c_args: common_cflags,
  )
