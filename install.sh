#!/usr/bin/bash
set -e

user="$(whoami)"

echo "USING SESSION " $XDG_SESSION_TYPE

mkdir -p "$HOME"/.config/systemd/user/
git clone https://github.com/AshleyTheNeko/FumoScreen.git /tmp/fumo
cd /tmp/fumo
sed -e "s/ashley/$user/g" default.cpp > fumo.cpp
mv assets "$HOME"/.config/systemd/user/
make
mv fumo "$HOME"/.config/systemd/user/
sed -e "s/ashley/$user/g" default_service > fumo.service
mv fumo.service "$HOME"/.config/systemd/user/
systemctl --user enable fumo.service