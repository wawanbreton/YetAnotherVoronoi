# yavui

Minimal Qt Widgets test program for `libyav`.

It includes an `ExternalProject` step for `qtbase` and links against `Qt6::Widgets`.

## Build from root

```bash
cmake -S . -B build -DYAV_BUILD_UI=ON
cmake --build build --target yavui
```

A local Qt 6 Widgets installation discoverable by CMake is required.
