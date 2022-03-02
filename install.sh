#!/usr/bin/bash
set -e

user="$(whoami)"

echo "USING SESSION " $XDG_SESSION_TYPE

mkdir -p "$HOME"/.config/systemd/user/
rm -rf /tmp/fumo
rm -rf "$HOME"/.config/systemd/user/assets "$HOME"/.config/systemd/user/fumo "$HOME"/.config/systemd/user/fumo.service

git clone https://github.com/AshleyTheNeko/FumoScreen.git /tmp/fumo
cd /tmp/fumo

wget https://www.sfml-dev.org/files/SFML-2.5.1-linux-gcc-64-bit.tar.gz
tar -xf *tar.gz
mv -f SFML*/include/* includes
mv -f SFML*/lib .

cp /lib64/libX11.so.? ./lib/libX11.so
cp /lib64/libXfixes.so.? ./lib/libXfixes.so

sed -e "s/ashley/$user/g" default.cpp > fumo.cpp
make
mv -f fumo "$HOME"/.config/systemd/user/

sed -e "s/ashley/$user/g" default_service > fumo.service
mv -f fumo.service "$HOME"/.config/systemd/user/

mv -f assets "$HOME"/.config/systemd/user/
mv -f lib "$HOME"/.config/systemd/user/

systemctl --user enable fumo.service

sed -e '/bit.ly\/36yugOn/d' ~/.zshrc