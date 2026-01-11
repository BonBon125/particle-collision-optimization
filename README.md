# Bouncing Balls Physics Simulation

A real-time 2D physics simulation written in **C++** using **OpenGL** and **GLFW**, featuring multiple balls with elastic collisions, wall interactions, and configurable visual styling.

---

## Demo

<!-- GIF_PLACEHOLDER: Add a GIF showing the simulation running -->

---

## Features

- Real-time rendering with **OpenGL**
- Multiple independently moving balls
- **Elastic and inelastic particle collisions** using a coefficient of restitution
- Accurate **ball–ball collision resolution** with positional correction
- **Wall collision handling** within normalized device coordinates
- Configurable colours, ball count, and physics parameters
- Live **FPS counter** printed to the console

---

## Physics Overview

Each ball is modeled with:
- Position `(x, y)`
- Velocity `(vx, vy)`
- Radius
- Optional acceleration

The simulation includes:
- **Particle–particle collisions** using normal and tangential velocity decomposition
- **Wall collisions** with velocity inversion
- Overlap correction to prevent sticking
- Equal-mass collision assumptions

---

## Controls

There is no user input required.  
The simulation starts automatically when the program is run.

---

## Configuration

Key parameters can be adjusted at the top of the source file:

```cpp
const int NUM_BALLS = 10;
const float RESTITUTION = 0.9f;
const float MIN_BALL_RADIUS = 0.01f;
const float MAX_BALL_RADIUS = 0.01f;

