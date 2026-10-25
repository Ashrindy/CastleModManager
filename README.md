<h1>
    <img width="36" align="left" src="res/icon-256x256.png" />
    Castle Mod Manager
</h1>

A mod manager for the game Castle Strike.

## Installation

### Windows

Download the `windows` binary (exe) from the `Releases` tab, store it anywhere.

### Linux

Manual installation through flatpak is recommended. <br/>
Other method is downloading the `linux` archive from the `Releases` tab, extract anywhere and use.

## Building

### Windows

Requires [MinGW](https://github.com/niXman/mingw-builds-binaries) and [CMake](https://cmake.org/)

```
$ cmake .. -B build --preset mingw-windows-release
$ cd build
$ mingw32-make -j 8
```

### Linux

Requires GCC and CMake

```
$ cmake .. -B build --preset gcc-linux-release
$ cd build
$ make -j 8
```

# Credits
Huge inspiration from [HedgeModManager](https://www.github.com/hedge-dev/HedgeModManager) and [HEML](https://www.github.com/hedge-dev/HE1ML).

# Disclaimer

Castle Mod Manager is not affiliated with or endorsed by the game's rights holders.
