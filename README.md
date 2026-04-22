# 2D Particle Collision Simulation (OpenGL + C++)

<p align="center">
  <img src="gif-directory/simulation.gif" width="600">
</p>

A high-performance **2D particle collision simulation** written in
modern C++ using **OpenGL** and **GLFW**.\
The project simulates thousands of particles with real-time rendering
and collision detection while recording the simulation output to **PNG
frames**, **MP4 video**, or **GIF**.

The primary goal of the project is to explore **performance-oriented
simulation techniques**, including **spatial partitioning**,
**multithreaded frame encoding**, and **efficient collision
resolution**.

The simulation renders up to **30,000 particles** and demonstrates two
different collision detection strategies: -   A naive **O(n²)** implementation
-   A **grid-based spatial partitioning** system that significantly
    improves performance

The rendered output can be exported as media using **FFmpeg** pipelines
directly from the program.

------------------------------------------------------------------------

# Interesting Techniques

## Spatial Partitioning with Uniform Grids

The optimized particle system (`ParticleSystem_V2`) reduces collision
checks using a **uniform spatial grid**.

Instead of testing every particle against every other particle, the
world is divided into cells. Each particle is assigned to a grid cell,
and collision checks are performed only against particles in nearby
cells.

This reduces the complexity from:

    O(n²)

to approximately:

    O(n)

for evenly distributed particles.

The grid is rebuilt every frame based on particle positions.

------------------------------------------------------------------------

## Elastic Collision Resolution

Particle collisions are resolved using **vector projection** onto normal
and tangent vectors.

Velocity components along the collision normal are swapped while tangent
components remain unchanged.

This produces realistic **equal-mass elastic collisions**.

Mathematically:

    v_normal = v · n
    v_tangent = v · t

The implementation also includes **positional correction** to prevent
particle overlap ("sticking").

------------------------------------------------------------------------

## Multithreaded Frame Recording

Frame capture is performed on the **render thread**, while PNG encoding
runs on a **dedicated writer thread**.

Captured frames are pushed into a thread-safe queue:

    std::queue<std::vector<unsigned char>>

Synchronization is handled with:

-   `std::mutex`
-   `std::condition_variable`
-   `std::atomic`

This architecture prevents expensive image encoding from blocking the
render loop.

------------------------------------------------------------------------

## OpenGL Framebuffer Capture

The renderer captures raw pixel data using:

    glReadPixels()

The pixel buffer is vertically flipped and written to disk using
**stb_image_write**.

------------------------------------------------------------------------

## Automatic Media Export via FFmpeg

The recorder builds **FFmpeg commands dynamically** to convert captured
PNG frames into:

-   MP4 videos
-   Optimized GIF animations

GIF generation uses the **palette generation pipeline**:

    palettegen → paletteuse

This produces significantly better color quality and smaller files than
naïve GIF conversion.

------------------------------------------------------------------------

# Libraries and Technologies

This project uses several libraries that experienced developers may find
interesting.

### OpenGL

Low-level graphics API used for rendering particles.

https://www.opengl.org/

------------------------------------------------------------------------

### GLFW

Handles window creation, input, and OpenGL context management.

https://www.glfw.org/

------------------------------------------------------------------------

### stb_image_write

A lightweight single-header library used to encode PNG images directly
from memory buffers.

https://github.com/nothings/stb

------------------------------------------------------------------------

### FFmpeg

Used to convert image sequences into video and GIF formats.

https://ffmpeg.org/

------------------------------------------------------------------------

### C++ Concurrency Utilities

The recorder relies on modern C++ threading primitives:

-   `std::thread`
-   `std::atomic`
-   `std::mutex`
-   `std::condition_variable`

Documentation:

https://en.cppreference.com/w/cpp/thread

------------------------------------------------------------------------

# Project Structure

    .
    ├── images
    ├── gif-directory
    └── stb_image_write.h

### images

Stores PNG frames generated during recording.

Frame filenames follow the format:

    frame_00000.png
    frame_00001.png
    ...

These images are later converted into media using FFmpeg.

------------------------------------------------------------------------

### gif-directory

Stores exported media files generated from captured frames.

Examples include:

-   simulation GIFs used in documentation
-   MP4 recordings of the simulation

------------------------------------------------------------------------

### stb_image_write.h

Single-header image encoding library used to write PNG frames.

------------------------------------------------------------------------

# Rendering Overview

Particles are drawn using **triangle fan primitives** to approximate
circles.

    GL_TRIANGLE_FAN

The number of segments is configurable via:

    NUM_CIRCLE_SEGMENTS

This allows tuning between rendering quality and performance.

------------------------------------------------------------------------

# How to run

    mkdir build
    cd build
    cmake ../
    cmake --build .
    ./MyOpenGLApp

------------------------------------------------------------------------

# Simulation Characteristics

| Property | Value |
|----------|-------|
| Particle count | 30,000 |
| Rendering | OpenGL |
| Collision detection | Grid spatial partitioning |
| Frame capture | OpenGL framebuffer |
| Output formats | PNG, MP4, GIF |
| Recording | Multithreaded |


------------------------------------------------------------------------

# Development Notes

The project is intentionally minimal and avoids external frameworks
beyond GLFW.

This makes it a good reference for:

-   **high-performance particle simulations**
-   **OpenGL rendering fundamentals**
-   **multithreaded media encoding pipelines**
-   **spatial partitioning algorithms**
