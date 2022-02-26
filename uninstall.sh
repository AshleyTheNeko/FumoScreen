#!/bin/env bash

systemctl --user disable fumo.service && cd ~/.config/systemd/user && rm fumo fumo.service cirno.png cirno.ogg
