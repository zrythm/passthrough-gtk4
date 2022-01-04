Passthrough GTK4
================

A passthrough plugin for testing GTK4 plugin support in
LV2 hosts.

# Dependencies

LV2 and GTK4

# Building & Installing

Configure
```
meson build
```

Build
```
ninja -C build
```

The following will install the LV2 bundle under
`/your/path/usr/lib/lv2/PassthroughGtk4.lv2`
```
DESTDIR=/your/path ninja -C build install
```

# Copying

Copyright © 2022 Alexandros Theodotou

Released under the GNU AGPL version 3 or later.
See the file [COPYING](COPYING) for details.
