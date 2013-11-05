#!/bin/sh

OPK_NAME=abbaye.opk
echo ${OPK_NAME}

# create default.gcw0.desktop
cat > default.gcw0.desktop <<EOF
[Desktop Entry]
Name=Abbaye des Morts
Comment=Labyrinth roaming
Exec=abbaye
Terminal=false
Type=Application
StartupNotify=true
Icon=abbaye2
Categories=games;
EOF

# create opk
FLIST="fonts"
FLIST="${FLIST} graphics"
FLIST="${FLIST} mapa"
FLIST="${FLIST} music"
FLIST="${FLIST} sounds"
FLIST="${FLIST} README"
FLIST="${FLIST} abbaye"
FLIST="${FLIST} abbaye2.png"
FLIST="${FLIST} default.gcw0.desktop"

rm -f ${OPK_NAME}
mksquashfs ${FLIST} ${OPK_NAME} -all-root -no-xattrs -noappend -no-exports

cat default.gcw0.desktop
rm -f default.gcw0.desktop