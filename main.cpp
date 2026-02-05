#include <GLFW/glfw3.h>
#include <bits/stdc++.h>
#include <cmath>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

const int WIDTH_HEIGHT = 1000;

const int NUM_CIRCLE_SEGMENTS = 10;
const int NUM_BALLS = 5000;

const float MIN_BALL_RADIUS = 0.01f;
const float MAX_BALL_RADIUS = 0.01f;
const float RESTITUTION = 0.9f;
const float BORDER_THICKNESS = 0.001f;
const float BALL_RADIUS = 0.01f;
const float BALL_ACCELERATION = -0.0f;
const float WALL_COLLISION_ENERGY_LOSS = 1.00f;

const float WORLD_MIN = -1.0f;
const float WORLD_MAX = 1.0f;

const float CELL_SIZE = MAX_BALL_RADIUS * 2.5f;
const int GRID_WIDTH = int((WORLD_MAX - WORLD_MIN) / CELL_SIZE) + 1;
const int GRID_HEIGHT = GRID_WIDTH;

const float MAX_BALL_SPEED = 0.05f;

// Random number generation
std::random_device rd;
std::default_random_engine gen(rd());
std::uniform_real_distribution<float> velocity_dist(-MAX_BALL_SPEED, MAX_BALL_SPEED);
std::uniform_real_distribution<float> position_dist(WORLD_MIN, WORLD_MAX);
std::uniform_real_distribution<float> radius_dist(MIN_BALL_RADIUS, MAX_BALL_RADIUS);

// Colour customisation
struct Colour {
    float r, g, b;
};

const std::unordered_map<std::string, Colour> COLOURS = {
    // Blues
    { "Light Blue", { 0.2f, 0.7f, 1.0f } },
    { "Sky Blue", { 0.4f, 0.7f, 1.0f } },
    { "Deep Blue", { 0.1f, 0.2f, 0.8f } },
    { "Navy", { 0.0f, 0.0f, 0.5f } },
    { "Cyan", { 0.0f, 1.0f, 1.0f } },

    // Reds
    { "Red", { 1.0f, 0.0f, 0.0f } },
    { "Dark Red", { 0.6f, 0.0f, 0.0f } },
    { "Crimson", { 0.86f, 0.08f, 0.24f } },
    { "Salmon", { 1.0f, 0.5f, 0.45f } },

    // Greens
    { "Green", { 0.0f, 1.0f, 0.0f } },
    { "Lime", { 0.5f, 1.0f, 0.0f } },
    { "Forest Green", { 0.13f, 0.55f, 0.13f } },
    { "Mint", { 0.6f, 1.0f, 0.6f } },

    // Yellows & Oranges
    { "Yellow", { 1.0f, 1.0f, 0.0f } },
    { "Gold", { 1.0f, 0.84f, 0.0f } },
    { "Orange", { 1.0f, 0.5f, 0.0f } },
    { "Dark Orange", { 1.0f, 0.35f, 0.0f } },

    // Purples & Pinks
    { "Purple", { 0.6f, 0.2f, 0.8f } },
    { "Violet", { 0.5f, 0.0f, 1.0f } },
    { "Magenta", { 1.0f, 0.0f, 1.0f } },
    { "Pink", { 1.0f, 0.4f, 0.7f } },
    { "Hot Pink", { 1.0f, 0.1f, 0.6f } },

    // Neutrals
    { "White", { 1.0f, 1.0f, 1.0f } },
    { "Light Gray", { 0.8f, 0.8f, 0.8f } },
    { "Gray", { 0.5f, 0.5f, 0.5f } },
    { "Dark Gray", { 0.2f, 0.2f, 0.2f } },
    { "Black", { 0.0f, 0.0f, 0.0f } },

    // Earth tones
    { "Brown", { 0.59f, 0.29f, 0.0f } },
    { "Chocolate", { 0.82f, 0.41f, 0.12f } },
    { "Tan", { 0.82f, 0.71f, 0.55f } },
    { "Olive", { 0.5f, 0.5f, 0.0f } },

    // Extras
    { "Teal", { 0.0f, 0.5f, 0.5f } },
    { "Turquoise", { 0.25f, 0.88f, 0.82f } },
    { "Indigo", { 0.29f, 0.0f, 0.51f } }
};

const std::unordered_map<bool, std::string> BALL_COLOURS = {
    { true, "Red" },
    { false, "Teal" }
};
std::string BACKGROUND_COLOUR = "Black";

void drawCircle(float cx, float cy, float r)
{
    // This function draws the circle using segments as the number of triangles to be drawn
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);

    for (int i = 0; i <= NUM_CIRCLE_SEGMENTS; i++) {
        float angle = 2.0f * 3.1415926f * i / NUM_CIRCLE_SEGMENTS;
        // Converting polar coordinates to cartesian
        float x = cx + cos(angle) * r;
        float y = cy + sin(angle) * r;
        glVertex2f(x, y);
    }

    glEnd();
}

void clearScreen()
{
    auto& c = COLOURS.at(BACKGROUND_COLOUR);
    glClearColor(c.r, c.g, c.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

struct Ball {
    float x, y;
    float vx, vy;
    float radius;
    bool is_colliding = false;
};

class BallCollection {
private:
    std::vector<Ball*> Balls;
    std::vector<std::vector<int>> grid;

    int cellX(float x) { return int((x - WORLD_MIN) / CELL_SIZE); }
    int cellY(float y) { return int((y - WORLD_MIN) / CELL_SIZE); }
    int cellIndex(int x, int y) { return y * GRID_WIDTH + x; }

    Ball* createBall()
    {
        Ball* newBall = new Ball;
        newBall->x = position_dist(gen);
        newBall->y = position_dist(gen);
        newBall->vx = velocity_dist(gen);
        newBall->vy = velocity_dist(gen);
        newBall->radius = radius_dist(gen);
        return newBall;
    }

public:
    BallCollection(int numBalls = NUM_BALLS)
    {
        Balls.resize(numBalls);
        for (int i = 0; i < numBalls; i++) {
            Balls[i] = createBall();
        }
        grid.resize(GRID_WIDTH * GRID_HEIGHT);
    }

    void rebuildGrid()
    {
        // Remove all values in the grid
        for (auto& cell : grid) {
            cell.clear();
        }

        // Add each ball to the corresponding gridbox
        for (int i = 0; i < Balls.size(); i++) {
            Ball* b = Balls[i];

            // Convert the x and y values to the grid x and y values
            // Clamp ensures the values do not exceed the limits provided
            int cx = std::clamp(cellX(b->x), 0, GRID_WIDTH - 1);
            int cy = std::clamp(cellY(b->y), 0, GRID_HEIGHT - 1);
            grid[cellIndex(cx, cy)].push_back(i);
        }
    }

    bool touching(Ball* a, Ball* b)
    {
        // Returns wether the two balls are touching
        float dx = a->x - b->x;
        float dy = a->y - b->y;
        float r = a->radius + b->radius;
        return dx * dx + dy * dy <= r * r;
    }

    void render()
    {

        clearScreen();
        glLoadIdentity();
        for (int i = 0; i < Balls.size(); i++) {
            float x = Balls[i]->x;
            float y = Balls[i]->y;
            float radius = Balls[i]->radius;

            // auto& bc = COLOURS.at("Black");
            //
            // glColor3f(bc.r, bc.g, bc.b);
            // drawCircle(x, y, radius + BORDER_THICKNESS);

            auto& c = COLOURS.at(BALL_COLOURS.at(Balls[i]->is_colliding));

            glColor3f(c.r, c.g, c.b);
            drawCircle(x, y, radius);
        }
    }

    void resolveCollision(Ball* ball1, Ball* ball2)
    {
        // Vector between centers
        float dx = ball2->x - ball1->x;
        float dy = ball2->y - ball1->y;
        float distance = std::sqrt(dx * dx + dy * dy);
        if (distance == 0.0f) {
            return; // Avoid division by zero
        }

        // Normal vector
        float nx = dx / distance;
        float ny = dy / distance;

        // Tangent vector
        float tx = -ny;
        float ty = nx;

        // Project velocities onto normal and tangent
        float v1n = ball1->vx * nx + ball1->vy * ny;
        float v1t = ball1->vx * tx + ball1->vy * ty;

        float v2n = ball2->vx * nx + ball2->vy * ny;
        float v2t = ball2->vx * tx + ball2->vy * ty;

        // Equal mass elastic collision → swap normal components
        // Equal mass inelastic collision using coefficient of restitution
        float v1n_after = (v1n + v2n - RESTITUTION * (v1n - v2n)) * 0.5f;
        float v2n_after = (v1n + v2n + RESTITUTION * (v1n - v2n)) * 0.5f;

        // Convert scalar normal/tangent velocities back to vectors
        ball1->vx = v1n_after * nx + v1t * tx;
        ball1->vy = v1n_after * ny + v1t * ty;

        ball2->vx = v2n_after * nx + v2t * tx;
        ball2->vy = v2n_after * ny + v2t * ty;

        // Optional: positional correction to prevent sticking
        float overlap = (ball1->radius + ball2->radius) - distance;
        if (overlap > 0.0f) {
            float correction = overlap * 0.5f;
            ball1->x -= nx * correction;
            ball1->y -= ny * correction;
            ball2->x += nx * correction;
            ball2->y += ny * correction;
        }

        ball1->is_colliding = true;
        ball2->is_colliding = true;
    }

    void handleParticleCollisions()
    {
        // First assume no balls are colliding
        for (auto* b : Balls) {
            b->is_colliding = false;
        }

        for (int i = 0; i < Balls.size(); i++) {
            // For every ball
            Ball* b = Balls[i];

            // Calculate the grid square the ball is in
            int cx = cellX(b->x);
            int cy = cellY(b->y);

            // Iterate over all the grid squares that are adjacent to the one the ball is in
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    int nx = cx + dx;
                    int ny = cy + dy;

                    // If the grid square is outside of bounds, continue
                    if (nx < 0 || ny < 0 || nx >= GRID_WIDTH || ny >= GRID_HEIGHT) {
                        continue;
                    }
                    for (int j : grid[cellIndex(nx, ny)]) {
                        // Only check collisions with balls further in the grid
                        if (j <= i) {
                            continue;
                        }
                        // If they are touching,
                        if (touching(b, Balls[j])) {
                            resolveCollision(b, Balls[j]);
                        }
                    }
                }
            }
        }
    }

    void handleAcceleration(Ball* ball, float dt)
    {
        // For each ball, apply acceleration to update
        // We have initial velocity, time and acceleration
        // Calculate final velocity
        ball->vy = ball->vy + BALL_ACCELERATION * dt;
    }

    void handleWallCollisions(Ball* ball)
    {
        bool colliding = false;
        if (ball->x + ball->radius > 1.0f) {
            ball->x = 1.0f - ball->radius;
            ball->vx = -ball->vx;
            ball->vx *= WALL_COLLISION_ENERGY_LOSS;
            colliding = true;
        }
        if (ball->x - ball->radius < -1.0f) {
            ball->x = -1.0f + ball->radius;
            ball->vx = -ball->vx;
            ball->vx *= WALL_COLLISION_ENERGY_LOSS;
            colliding = true;
        }

        if (ball->y + ball->radius > 1.0f) {
            ball->y = 1.0f - ball->radius;
            ball->vy = -(ball->vy);
            ball->vy *= WALL_COLLISION_ENERGY_LOSS;
            colliding = true;
        }
        if (ball->y - ball->radius < -1.0f) {
            ball->y = -1.0f + ball->radius;
            ball->vy = -(ball->vy);
            ball->vy *= WALL_COLLISION_ENERGY_LOSS;
            colliding = true;
        }
        ball->is_colliding = (ball->is_colliding) ? true : colliding;
    }

    void moveBall(Ball* ball, float timeDelta)
    {
        ball->x += ball->vx * timeDelta;
        ball->y += ball->vy * timeDelta;
    }

    void update(float timeDelta)
    {
        rebuildGrid();
        handleParticleCollisions();

        for (auto* b : Balls) {
            handleWallCollisions(b);
            handleAcceleration(b, timeDelta);
            moveBall(b, timeDelta);
        }
    }
};

void displayFPS(float dt)
{
    float fps = 1.0 / dt;
    std::cout << "\r" << fps << std::flush;
}

int main()
{
    if (!glfwInit())
        return -1;

    GLFWwindow* window = glfwCreateWindow(WIDTH_HEIGHT, WIDTH_HEIGHT, "Bouncing Ball", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // 2D projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    glfwSwapBuffers(window);
    glfwPollEvents();

    BallCollection balls = BallCollection();

    int x;
    std::cin >> x;

    float lastTime = (float)glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        float currentTime = (float)glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        balls.update(deltaTime);
        // Render
        balls.render();

        glfwSwapBuffers(window);
        glfwPollEvents();

        displayFPS(deltaTime);
    }

    glfwTerminate();
    return 0;
}
