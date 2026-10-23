<h1>
    <img width="36" align="left" src="res/icon-256x256.png" />
    Castle Mod Manager
</h1>

A mod manager for the game Castle Strike.

# Building 
## Windows
Requires [MinGW](https://github.com/niXman/mingw-builds-binaries) and [CMake](https://cmake.org/)
```
$ cmake .. -B build --preset mingw-windows-release
$ cd build
$ mingw32-make -j 8
```
## Linux
Requires GCC and CMake
```
$ cmake .. -B build --preset gcc-linux-release
$ cd build
$ make -j 8
```

# Disclaimer

Castle Mod Manager is not affiliated with or endorsed by the game's rights holders.
