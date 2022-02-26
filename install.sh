#!/usr/bin/bash

mkdir -p /home/$(whoami)/.config/systemd/user/
git clone https://github.com/AshleyTheNeko/FumoScreen.git /tmp/fumo
cd /tmp/fumo
sed -e "s/ashley/$(whoami)/g" default.cpp > fumo.cpp
mv assets/cirno.png /home/$(whoami)/.config/systemd/user/
make
mv fumo /home/$(whoami)/.config/systemd/user/
sed -e "s/ashley/$(whoami)/g" default_service > fumo.service
mv fumo.service /home/$(whoami)/.config/systemd/user/
systemctl --user enable fumo.service