<img src="icon.png" align="left" width="128px">

# TMCScan 3.0
Only Linux Lightweight Minecraft Port Scanner Written In C/Ncurses

<p align='center'>
  <a href='https://en.wikipedia.org/wiki/C%20%28programming%20language%29'><img src='https://img.shields.io/badge/Language-C-black?style=for-the-badge'></a>
  <a href='https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1548.pdf'><img src='https://img.shields.io/badge/C%20Standard%20-C11-black?style=for-the-badge'></a>
  <a href='https://cmake.org/cmake/help/v3.10/index.html'><img src='https://img.shields.io/badge/CMake%20-3.10-black?style=for-the-badge'></a>
  <a href='https://github.com/TuiMC/TMCScan'><img src='https://img.shields.io/badge/Latest%20Version-v3.0-black?style=for-the-badge'></a>
  <a href='https://github.com/TuiMC/TMCScan/releases'><img src='https://img.shields.io/github/v/release/TuiMC/TMCScan?label=Latest%20Release&color=black&style=for-the-badge'></a>
  <a href='https://github.com/TuiMC/TMCScan/blob/master/LICENSE'><img src='https://img.shields.io/badge/License-GPLv3-black?style=for-the-badge'></a>
  <a href='https://github.com/TuiMC/stargazers'><img src='https://img.shields.io/github/stars/TuiMC/TMCScan?color=black&style=for-the-badge'></a>
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
$ git clone https://github.com/TuiMC/TMCScan
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
Also you can enable the Sanitize with Debug mode.
```
$ ./builder.sh CSD
```
  
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
Copyright (C) 2022 0Magenta0

