all:
	make qemu
qemu:
	qemu-system-i386 -M isapc -m 32 -serial stdio -hda hda.img -net none
