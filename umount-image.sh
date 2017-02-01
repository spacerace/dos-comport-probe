#!/bin/bash
umount -v hda* 
losetup -v -d hda.img
