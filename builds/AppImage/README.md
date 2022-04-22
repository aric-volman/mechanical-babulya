Run QEMU:
qemu-system-x86_64 -drive file=ubuntu.img,format=raw,index=0,media=disk -cdrom ../Downloads/ubuntu-16.04.7-desktop-amd64.iso -m 1600M -boot order=cd
How to clone on Linux cli (for ubuntu build vm)
git clone "https://<username>:<pat>@github.com/aric-volman/mechanical-babulya"

You should copy this AppDir OUTSIDE of this repo's folder, everything here is just a placeholder.
