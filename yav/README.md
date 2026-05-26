# yav

Minimal command-line test program for `libyav`.

It uses `cxxopts`, fetched and built through CMake `ExternalProject`.

## Build from root

```bash
cmake -S . -B build -DYAV_BUILD_CLI=ON
cmake --build build --target yav
```
