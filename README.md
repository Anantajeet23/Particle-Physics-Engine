# Particle Physics Engine

## Overview

 This engine enables simulation of particle-based physics. It includes support for diverse force generators, robust collision detection, and efficient resolution algorithms, making it a fun tool to play around with. 
 
## Demo

### 1. Spinning Particle Collisions
![Spinning Particle Collisions](assets/spin.gif)

### 2. Fireworks Simulation
![Buoyancy Simulation](assets/fireworks.gif)

## Features

- **Physics Simulation:**
  - Particle-based physics with accurate integration using Newton-Euler methods.
  - Collision detection and resolution with interpenetration handling.
  - Force generators for various effects:
    - Gravity
    - Drag
    - Springs (linear and anchored)
    - Buoyancy
    - Pairwise gravitational forces
- **Customizability:**
  - Cube boundaries for particle containment.
  - Configurable forces for different simulation scenarios.
- **Visualization:**
  - SFML-based rendering for real-time 3D visualization.
  - OpenGL integration for enhanced graphics.

## Installation

### Prerequisites
- **C++ Compiler**: Ensure you have a compiler that supports C++11 or later.
- **SFML Library**: For graphics and windowing.
- **OpenGL**: For 3D rendering.

### Steps
1. Install SFML:<br>
  brew install sfml  # On macOS with Homebrew<br>
  sudo apt-get install libsfml-dev  # On Ubuntu/Debian.

2. Build and run the project using the provided Makefile:<br>
  make<br>
  ./spin



## License
This project is licensed under the MIT License. See the LICENSE file for details.

## Acknowledgments
This project is based on the book "Game Physics Engine Development" by Ian Millington. Improvements were made to support additional features and better visualization.