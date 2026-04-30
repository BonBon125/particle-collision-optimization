#pragma once

inline const float RECORDING_DURATION = 2.0f;
inline const int WIDTH_HEIGHT = 1000;
inline const int NUM_CIRCLE_SEGMENTS = 10;
inline const int NUM_PARTICLES = 10000;
inline const float MIN_BALL_RADIUS = 0.005f;
inline const float MAX_BALL_RADIUS = 0.01f;
inline const float RESTITUTION = 0.9f;
inline const float BORDER_THICKNESS = 0.001f;
inline const float BALL_RADIUS = 0.01f;
inline const float WORLD_MIN = -1.0f;
inline const float WORLD_MAX = 1.0f;
inline const float MAX_BALL_SPEED = 0.1f;

// Spatial Grid Constants
inline const float CELL_SIZE = MAX_BALL_RADIUS * 2.5f;
inline const int GRID_WIDTH = (int)((WORLD_MAX - WORLD_MIN) / CELL_SIZE) + 1;
inline const int GRID_HEIGHT = GRID_WIDTH;
