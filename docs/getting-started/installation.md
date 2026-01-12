# Installation

## System dependency

You need GMP headers + libraries:

- `gmp`
- `gmpxx` (C++ wrapper)

Examples:

=== "Ubuntu / Debian"
    ```bash
    sudo apt-get update
    sudo apt-get install -y libgmp-dev
    ```

=== "Arch"
    ```bash
    sudo pacman -S gmp
    ```

=== "macOS (Homebrew)"
    ```bash
    brew install gmp
    ```

## Build

Typical out-of-tree build:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

If the project installs targets:

```bash
cmake --install build
```
