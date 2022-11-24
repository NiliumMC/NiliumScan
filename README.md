<img src="icon.png" align="left" width="128px">

# TMCScan 3.0
Only Linux Lightweight Minecraft Port Scanner Written In C/Ncurses

<p align='center'>
[![](https://img.shields.io/badge/Language-C-black?style=for-the-badge)](https://en.wikipedia.org/wiki/C%20%28programming%20language%29)
[![](https://img.shields.io/badge/C%20Standard%20-Gnu99-black?style=for-the-badge)](https://gcc.gnu.org/onlinedocs/gcc-3.3.6/gcc/Standards.html)
[![](https://img.shields.io/badge/CMake%20-3.10-black?style=for-the-badge)](https://cmake.org/cmake/help/v3.10/index.html)
[![](https://img.shields.io/badge/Latest%20Version-v3.0-black?style=for-the-badge)](https://github.com/0Magenta0/TMCScan)
[![](https://img.shields.io/github/v/release/TuiMC/TMCScan?label=Latest%20Release&color=black&style=for-the-badge)](https://github.com/TuiMC/TMCScan/releases)
[![](https://img.shields.io/badge/License-GPLv3-black?style=for-the-badge)](https://github.com/TuiMC/TMCScan/blob/master/LICENSE)
</p>

The Author assume no responsibility for errors/omissions/damages resulting from the use of this program!

### Resources
[wiki.vg](https://wiki.vg) - Minecraft Protocol Wiki  
[minecraft.fandom.com](https://minecraft.fandom.com) - Minecraft Wiki

### Requirements
* [CMake](https://cmake.org/files) (>= 3.10)
* [Ncurses](https://invisible-island.net/ncurses/)
* [Json-C](https://github.com/json-c/json-c)

## Manual Building
Clone the repo go to the `build` directory.
```
$ git clone https://github.com/0Magenta0/TMCScan
$ cd TMCScan/build
```
Now you can build it with `RELEASE` or `DEBUG` type.
```
$ ./builder.sh R
$ ./builder.sh D
```
So you can combine this with `clean` flag.
```
$ ./builder.sh CR
$ ./builder.sh CD
```
And in the build folder you can see `tmcscan` binary.  
  
Note: If you are using Termux you also need to install Json-C packages.
```
$ apt update
$ apt install json-c json-c-static
```

### TODO
* SOCKS Proxy Support
* Render Pretty MOTD
* Render Server Icon
* Saving Servers List to File
* Default Config
* Scanning Servers from File
* Servers Auth Type Check

### LICENSE
TMCScan uses GNU GPLv3 license.  
Copyright (C) 2022 \_Magenta\_

