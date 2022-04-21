Run QEMU:
qemu-system-x86_64 -drive file=ubuntu.img,format=raw,index=0,media=disk -cdrom ../Downloads/ubuntu-16.04.7-desktop-amd64.iso -m 1600M -boot order=cd
