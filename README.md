# bignumber

## GNU MP install

1.

```bash
wget https://gmplib.org/download/gmp/gmp-6.3.0.tar.xz
```

2.

```bash
tar -xJf gmp-6.3.0.tar.xz
```

3.

```bash
./configure --enable-cxx
```

4.

```bash
make -j$(nproc)
```

5.

```bash
sudo make install
```

## Package install

Installation:

1. Extract the appropriate tar.gz file to your system:
   `sudo tar -xzf bignumber-linux-amd64.tar.gz -C /`

2. Update library cache:
   `sudo ldconfig`

3. Use in CMake projects:

```cmake
find_package(bignumber REQUIRED)
target_link_libraries(your_target usub::unumber)
```
