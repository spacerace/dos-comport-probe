#!/bin/bash
losetup -v -f hda.img
partprobe /dev/loop0
mount /dev/loop0p1 hda_c

losetup -a
