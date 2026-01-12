# CMake integration

## add_subdirectory

If you vendor `unumber` in your repository:

```cmake
add_subdirectory(external/unumber)
target_link_libraries(your_target PRIVATE unumber gmp gmpxx)
```

## FetchContent

```cmake
include(FetchContent)

FetchContent_Declare(
  unumber
  GIT_REPOSITORY https://github.com/Usub-development/unumber.git
  GIT_TAG main
)
FetchContent_MakeAvailable(unumber)

target_link_libraries(your_target PRIVATE unumber gmp gmpxx)
```

Notes:

- Linking to `gmpxx` often pulls `gmp` automatically, but keep both if your toolchain requires it.
- If you use `find_package(GMP REQUIRED)`, link whatever targets your find-module exports.
