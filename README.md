# Route Planning Project

This project implements an A* search algorithm to find the shortest path between two points on an OpenStreetMap. The route is visualized using SFML (Simple and Fast Multimedia Library) with rich map rendering including roads, buildings, water bodies, parks, and more.

<img src="map.png" width="600" height="450" />

## Features

- **A* Path Finding Algorithm**: Efficient route planning using the A* search algorithm with heuristic optimization
- **Rich Map Visualization**:
  - Roads with accurate widths and colors based on type (motorways, residential, etc.)
  - Buildings with outlines
  - Water bodies (lakes, rivers)
  - Landuse areas (forests, parks, commercial, residential zones)
  - Leisure areas (parks, playgrounds)
  - Railways with proper styling
- **Interactive Route Display**:
  - Orange path line showing your calculated route
  - Green marker at start position
  - Red marker at end position
  - Real-time distance calculation in meters

## Table of Contents
- [Prerequisites](#prerequisites)
- [Dependencies Installation](#dependencies-installation)
- [Building the Project](#building-the-project)
- [Running the Application](#running-the-application)
- [Testing](#testing)
- [Project Structure](#project-structure)
- [Troubleshooting](#troubleshooting)

## Prerequisites

Before building this project, ensure you have the following installed:

### All Operating Systems
- **CMake** >= 3.11.3
- **Make** >= 4.1 (Linux, Mac), 3.81 (Windows)
- **C++ Compiler** with C++17 support (gcc/g++ >= 7.4.0 or clang)

### Platform-Specific Requirements

#### macOS
- Xcode Command Line Tools
- Homebrew package manager

#### Linux (Ubuntu/Debian)
- Build essentials package
- Standard development tools

#### Windows
- MinGW or Visual Studio 2017+ with C++17 support
- Or use WSL (Windows Subsystem for Linux)

## Dependencies Installation

### 1. Install SFML

SFML is used for graphics rendering and window management. This project uses **SFML 3.0+** which has native Apple Silicon support.

#### macOS
```bash
# Install Homebrew if you haven't already
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install SFML
brew install sfml
```

#### Linux (Ubuntu/Debian)
```bash
# Update package list
sudo apt update

# Install dependencies
sudo apt install build-essential cmake

# Install SFML 3.0 (if available in your distro)
sudo apt install libsfml-dev

# If SFML 3.0 is not available, build from source:
git clone https://github.com/SFML/SFML.git
cd SFML
git checkout 3.0.0
mkdir build && cd build
cmake ..
make
sudo make install
```

#### Windows
```bash
# Using vcpkg (recommended)
vcpkg install sfml

# Or download pre-built binaries from:
# https://www.sfml-dev.org/download.php
```

### 2. Verify Installation

```bash
# Check CMake version
cmake --version

# Check compiler version
g++ --version  # or clang++ --version

# Verify SFML installation (the output should show SFML 3.x)
pkg-config --modversion sfml-all  # Linux/Mac
```

## Building the Project

### Step 1: Clone the Repository

When cloning this project, use the `--recurse-submodules` flag to include dependencies:

```bash
# Using HTTPS
git clone https://github.com/Bootcamp-AI/CppND-Route-Planning-Project.git --recurse-submodules

# Or using SSH
git clone git@github.com:Bootcamp-AI/CppND-Route-Planning-Project.git --recurse-submodules
```

If you already cloned without submodules:
```bash
cd CppND-Route-Planning-Project
git submodule update --init --recursive
```

### Step 2: Create Build Directory

```bash
cd CppND-Route-Planning-Project
mkdir build
cd build
```

### Step 3: Configure with CMake

```bash
cmake ..
```

Expected output should show:
```
-- Found SFML 3.x.x in /path/to/sfml
-- Configuring done
-- Generating done
```

### Step 4: Build the Project

```bash
# On Linux/Mac
make

# On Windows with Visual Studio
cmake --build . --config Release

# Or with multiple cores (faster)
make -j$(nproc)  # Linux
make -j$(sysctl -n hw.ncpu)  # Mac
```

The build creates two executables:
- `OSM_A_star_search` - Main application
- `test` - Unit tests

## Running the Application

### Basic Usage

From the `build` directory:

```bash
./OSM_A_star_search
```

You'll be prompted to enter coordinates:
```
Reading OpenStreetMap data from the following file: ../map.osm
The map coordinates begin at (0,0) in the lower left corner and end at (100,100) in the upper right.
Enter a start x between 0 and 100
10
Enter a start y between 0 and 100
10
Enter an end x between 0 and 100
90
Enter an end y between 0 and 100
90
Path found with 33 nodes.
Distance: 873.416 meters.
```

A window will open displaying:
- The OpenStreetMap with all features (roads, buildings, water, parks)
- Your calculated route as an **orange line**
- **Green circle** at the start position
- **Red circle** at the end position

### Using a Custom Map File

```bash
./OSM_A_star_search -f ../map.osm
# Or use your own OSM file
./OSM_A_star_search -f /path/to/your/custom_map.osm
```

### Example Coordinate Pairs to Try

```bash
# Diagonal route (long distance)
Start: (10, 10)
End: (90, 90)

# Short route
Start: (25, 25)
End: (35, 35)

# Horizontal route
Start: (10, 50)
End: (90, 50)

# Vertical route
Start: (50, 10)
End: (50, 90)
```

### Closing the Application

- Click the window's close button (×)
- The window will close automatically when you're done viewing the route

## Testing

The project includes comprehensive unit tests for the A* algorithm implementation.

### Running All Tests

From the `build` directory:

```bash
./test
```

Expected output:
```
Running main() from .../gtest_main.cc
[==========] Running 4 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 4 tests from RoutePlannerTest
[ RUN      ] RoutePlannerTest.TestCalculateHValue
[       OK ] RoutePlannerTest.TestCalculateHValue (X ms)
[ RUN      ] RoutePlannerTest.TestAddNeighbors
[       OK ] RoutePlannerTest.TestAddNeighbors (X ms)
[ RUN      ] RoutePlannerTest.TestConstructFinalPath
[       OK ] RoutePlannerTest.TestConstructFinalPath (X ms)
[ RUN      ] RoutePlannerTest.TestAStarSearch
Path found with 33 nodes.
[       OK ] RoutePlannerTest.TestAStarSearch (X ms)
[----------] 4 tests from RoutePlannerTest (XXX ms total)

[----------] Global test environment tear-down
[==========] 4 tests from 1 test suite ran. (XXX ms total)
[  PASSED  ] 4 tests.
```

### What the Tests Verify

1. **TestCalculateHValue**: Validates the heuristic function (Euclidean distance)
2. **TestAddNeighbors**: Ensures neighbors are correctly added to the open list
3. **TestConstructFinalPath**: Verifies the path is built correctly from goal to start
4. **TestAStarSearch**: End-to-end test of the complete A* algorithm

All tests must pass for the implementation to be considered correct.

## Project Structure

```
CppND-Route-Planning-Project/
├── src/
│   ├── main.cpp              # Application entry point
│   ├── model.cpp/h           # OpenStreetMap data model
│   ├── route_model.cpp/h     # Route-specific model extensions
│   ├── route_planner.cpp/h   # A* algorithm implementation
│   └── sfml_render.cpp/h     # SFML-based visualization
├── test/
│   └── utest_rp_a_star_search.cpp  # Unit tests
├── thirdparty/
│   ├── pugixml/              # XML parsing library
│   └── googletest/           # Testing framework
├── build/                    # Build directory (created by you)
├── map.osm                   # Default OpenStreetMap data
└── CMakeLists.txt            # Build configuration
```

## How It Works

### A* Search Algorithm

The project implements the A* pathfinding algorithm with these key components:

1. **Heuristic Function (h-value)**: Euclidean distance to the goal
2. **Cost Function (g-value)**: Actual distance traveled from start
3. **f-value**: g-value + h-value (total estimated cost)

The algorithm:
1. Starts at the user-specified start node
2. Explores neighbors, calculating f-values
3. Always expands the node with lowest f-value next
4. Continues until reaching the goal
5. Reconstructs the path by following parent pointers

### Visualization

The SFML renderer draws map elements in layers:
1. **Background**: Light beige
2. **Landuses**: Colored zones (forests, parks, commercial, etc.)
3. **Leisure areas**: Light green parks
4. **Water bodies**: Blue water features
5. **Railways**: Grey tracks with white center lines
6. **Roads**: Color and width based on road type
7. **Buildings**: Tan rectangles with outlines
8. **Path**: Orange line showing your route
9. **Markers**: Green (start) and Red (end) circles

## Troubleshooting

### Build Issues

#### CMake can't find SFML
```bash
# Linux: Ensure pkg-config is installed
sudo apt install pkg-config

# Mac: Ensure Homebrew SFML is properly linked
brew link sfml

# All platforms: Check that SFML 3.x is installed
pkg-config --modversion sfml-all
```

#### Compiler errors about C++17
Ensure your compiler supports C++17:
```bash
# Check compiler version
g++ --version  # Should be >= 7.4.0
clang++ --version  # Should be >= 5.0

# Explicitly set compiler (if needed)
cmake -DCMAKE_CXX_COMPILER=g++-9 ..
```

#### Submodule errors
If you see errors about missing pugixml or googletest:
```bash
# From project root
git submodule update --init --recursive
```

### Runtime Issues

#### Window doesn't appear / Black screen
- Ensure SFML is correctly installed
- Try rebuilding from scratch:
  ```bash
  cd build
  rm -rf *
  cmake ..
  make
  ```

#### "Failed to read map file"
- Ensure you're running from the `build` directory
- Check that `../map.osm` exists relative to `build/`
- Use absolute path: `./OSM_A_star_search -f /absolute/path/to/map.osm`

#### Segmentation fault / Crash
- Verify coordinates are between 0 and 100
- Ensure the map file is valid OpenStreetMap XML format
- Check that all dependencies built correctly

### Apple Silicon (M1/M2/M3) Specific

This project **fully supports Apple Silicon** Macs thanks to SFML 3.0:
- No Rosetta 2 translation needed
- Native ARM64 performance
- If you have issues, ensure you installed the ARM version of Homebrew:
  ```bash
  # Check architecture
  arch
  # Should output: arm64

  # If needed, install ARM Homebrew
  /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
  ```

### Getting Help

If you encounter issues not covered here:
1. Check that all prerequisites are correctly installed
2. Verify you're using compatible versions (SFML 3.x, CMake 3.11+)
3. Try a clean rebuild (`rm -rf build && mkdir build && cd build && cmake .. && make`)
4. Check the GitHub Issues page for similar problems

## Performance Notes

- First run may take a few seconds to parse the OSM file
- Typical A* search completes in < 100ms
- Rendering is smooth at 60 FPS
- Path finding time depends on distance between start/end points

## License

This project is part of the Udacity C++ Nanodegree program.

## Acknowledgments

- OpenStreetMap for map data
- SFML for cross-platform graphics
- Google Test for unit testing framework
- pugixml for XML parsing
