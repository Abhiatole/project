# CGAL Boolean Operations

This project demonstrates the use of CGAL's Boolean Set Operations to compute the union of two 2D polygons using `CGAL::join()`.

## Prerequisites

- MSYS2 with MinGW64
- CMake (3.10 or higher)
- CGAL library
- GMP library
- MPFR library

## Installation with MSYS2

1. Install MSYS2 from [https://www.msys2.org/](https://www.msys2.org/)

2. Open MSYS2 MinGW64 terminal and update the package database:
   ```bash
   pacman -Syu
   ```

3. Install required packages:
   ```bash
   pacman -S mingw-w64-x86_64-cmake
   pacman -S mingw-w64-x86_64-cgal
   pacman -S mingw-w64-x86_64-gmp
   pacman -S mingw-w64-x86_64-mpfr
   pacman -S mingw-w64-x86_64-gcc
   pacman -S mingw-w64-x86_64-make
   ```

## Building the Project

1. Clone or download this repository
2. Navigate to the project directory in MSYS2 MinGW64 terminal
3. Create a build directory and navigate to it:
   ```bash
   mkdir build
   cd build
   ```

4. Configure the project with CMake:
   ```bash
   cmake -G "MSYS Makefiles" ..
   ```

5. Build the project:
   ```bash
   make
   ```

## Running the Program

After building, run the executable:
```bash
./boolean_operations
```

The program will compute the union of a square and a triangle, and display the resulting polygon's outer boundary and any holes.

## Project Structure

- `include/BooleanOperations.h` - Header file with class declarations
- `src/BooleanOperations.cpp` - Implementation of the Boolean operations
- `main.cpp` - Main program that demonstrates the union operation
- `CMakeLists.txt` - CMake configuration file

## Example Output

```
Computing the union of a square and a triangle...

Union result:
Result contains 1 polygon(s) with holes:
Polygon 1:
Outer boundary:
  (0, 0)
  (10, 0)
  (10, 5)
  (15, 5)
  (10, 15)
  (5, 5)
  (0, 5)
  (0, 0)

```

## Troubleshooting

If you encounter issues with linking libraries, ensure that:
1. All required packages are installed correctly
2. The CMake configuration can find the libraries
3. The library paths are correctly set in the build environment

For more detailed errors during compilation, use:
```bash
make VERBOSE=1
```