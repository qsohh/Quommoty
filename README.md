# QuantLib-based comodity analysis tool: Quommoty - Futures Pricer

This project is a futures pricing framework built in C++ with [QuantLib](https://www.quantlib.org/).

It provides a common pricing interface and specialized pricers for crude oil and electricity futures.  

This project is an iterative prototype project, and is intended primarily for personal learning and practice purposes.

## Features

- Base Pricer class with shared utilities.
- Specialized pricers:
  - CrudeFuturesPricer – cost-of-carry model (storable commodities).
  - ElectricityFuturesPricer – forward-anchor model with delivery averaging (non-storable commodities).
- Utility functions for printing and typing.
- Simple test demos included.

## Project Structure

```
Quommoty
├── CMakeLists.txt
├── include/
│   ├── utils/
│   ├── CrudeFuturesPricer.h
│   ├── ElectricityFuturesPricer.h
│   └── Pricer.h
├── src/
│   ├── utils/
│   ├── CrudeFuturesPricer.cpp
│   ├── ElectricityFuturesPricer.cpp
│   ├── Pricer.cpp
│   └── main.cpp
├── tests/
│   ├── demo.cpp
│   └── demo.h
├── data/
├── docs/
└── build/
```

## Requirements

- C++17 or higher
- [QuantLib](https://www.quantlib.org/)
- [CMake](https://cmake.org/) (>= 3.21 recommended)

Dependencies are managed with [vcpkg](https://github.com/microsoft/vcpkg) in manifest mode. See `vcpkg.json`. Alternatively, you can install QuantLib manually (e.g. via apt, brew, or from source).

## Build

This project uses [CMake](https://cmake.org/) as the build system.

### Windows (tested)
On my local machine I build with CMake presets:

```PowerShell
cmake --preset default

cmake --build --preset default-build -j
```

You can also run the commands directly if you have a working `CMakePresets.json`.

### Linux / macOS (not tested)

In principle the project should also build with a standard CMake workflow:

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

**! Note:** I mainly tested on Windows with presets. On other systems, you might need to adjust compilers, paths, or provide your own `CMakePresets.json`.

## Run

After building, the executable will be placed under the `build/` directory.

### Windows (tested)
If you use the provided presets or PowerShell scripts, you can run:

```PowerShell
.\build\Release\Quommoty.exe
```

### Linux / macOS (not fully tested)

```bash
./build/Quommoty
```

**! Note:** The exact path may vary depending on your generator and build configuration (Debug/Release).

## Contributing

Contributions are welcome!  
If you find a bug or have an idea for improvement, feel free to open an issue or submit a pull request.

## License

This project is licensed under the MIT License — see the [LICENSE](LICENSE) file for details.