Run QEMU:
qemu-system-x86_64 -drive file=ubuntu.img,format=raw,index=0,media=disk -cdrom ../Downloads/ubuntu-16.04.7-desktop-amd64.iso -m 1600M -boot order=cd
How to clone on Linux cli (for ubuntu build vm)
git clone "https://pat:x-oauth-basic@github.com/aric-volman/mechanical-babulya.git"


You should copy this AppDir OUTSIDE of this repo's folder, everything here is just a placeholder.
Build like normal with Qt Creator, and then place the mechanical-babulya binary into the bin folder in the external AppDir

You need Ubuntu 16.04, preferrably a VM
