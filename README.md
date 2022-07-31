<img src="icon.png" align="left" width="128px">

# TMCScan 1.0.4
Only Linux Lightweight Minecraft Port Scanner Written In C/Ncurses

[![](https://img.shields.io/badge/Language-C-blue)](https://en.wikipedia.org/wiki/C%20%28programming%20language%29) [![](https://img.shields.io/badge/C%20Standard%20-Gnu99-blue)](https://gcc.gnu.org/onlinedocs/gcc-3.3.6/gcc/Standards.html) [![](https://img.shields.io/badge/CMake%20-3.10-blue)](https://cmake.org/cmake/help/v3.10/index.html) [![](https://img.shields.io/badge/Latest%20Version-1.0.4-green)](https://github.com/0Magenta0/TMCScan) [![](https://img.shields.io/badge/Latest%20Release%20-1.0.0-orange)](https://github.com/0Magenta0/TMCScan/releases/tag/1.0.0)

The Author assume no responsibility for errors/omissions/damages resulting from the use of this program!

### Resources
[wiki.vg](https://wiki.vg) - Minecraft Protocol Wiki  
[minecraft.fandom.com](https://minecraft.fandom.com) - Minecraft Wiki  

## Manual Building
First of all you need to install CMake and Make utilities!  
You need [ncurses](https://invisible-island.net/ncurses/) and [json-c](https://github.com/json-c/json-c) libraries too.  
After that you can clone repo and build TMCScan.
```
$ git clone https://github.com/0Magenta0/TMCScan
$ cd TMCScan
$ mkdir build && cd build
$ cmake .. && make
```
And in the build folder you can see `tmcscan` binary.  
  
If you want to build TMCScan with Debug run this command.
```
$ cmake .. -DISDEBUG=ON && make
```
  
Note: If you are using Termux you need to install "json-c-static" package.
```
$ apt update
$ apt install json-c-static
```

### TODO
* SOCKS Proxy Support
* CLI Mode
* Render Pretty MOTD
* Render Server Icon
* Saving Servers List to File
* Default Config
* Scanning Servers from File
* Servers Auth Type Check

### LICENSE
TMCScan uses GNU GPL v3 license.  
Copyright (C) 2022 \_Magenta\_

