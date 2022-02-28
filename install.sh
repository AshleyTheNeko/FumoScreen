#!/usr/bin/bash
set -e

user="$(whoami)"

echo "USING SESSION " $XDG_SESSION_TYPE

mkdir -p "$HOME"/.config/systemd/user/
rm -rf "$HOME"/.config/systemd/user/assets "$HOME"/.config/systemd/user/fumo "$HOME"/.config/systemd/user/fumo.service

git clone https://github.com/AshleyTheNeko/FumoScreen.git /tmp/fumo
cd /tmp/fumo

sed -e "s/ashley/$user/g" default.cpp > fumo.cpp
make
mv -f fumo "$HOME"/.config/systemd/user/

sed -e "s/ashley/$user/g" default_service > fumo.service
mv -f fumo.service "$HOME"/.config/systemd/user/

mv -f assets "$HOME"/.config/systemd/user/

systemctl --user enable fumo.service