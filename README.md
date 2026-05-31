# YetAnotherVoronoi

Minimal CMake scaffold for a small Voronoi-oriented workspace:

- `libyav`: core C++ library (always built)
- `yav`: optional CLI test program (`cxxopts` via `ExternalProject`)
- `yavui`: optional Qt Widgets test program (`qtbase` fetch step via `ExternalProject`)

## Build

Dependencies for `libyav`: Boost headers and Armadillo.

```bash
cmake -S . -B build
cmake --build build
```

Enable optional targets with CMake options:

```bash
cmake -S . -B build -DYAV_BUILD_CLI=ON -DYAV_BUILD_UI=ON
cmake --build build
```

See each subfolder README for details.
