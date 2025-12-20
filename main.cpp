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

const int NUM_CIRCLE_SEGMENTS = 100;
const int NUM_BALLS = 100;
std::default_random_engine gen;
std::uniform_real_distribution<float> distribution(-1.0, 1.0);
const float borderThickness = 0.01f;

std::string BACKGROUND_COLOUR = "Black";

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
    { true, "Turquoise" },
    { false, "Chocolate" }
};

void drawCircle(float cx, float cy, float r)
{
    // This function draws the circle using segments as the number of triangles to be drawn
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);

    for (int i = 0; i <= NUM_CIRCLE_SEGMENTS; i++) {
        float angle = 2.0f * 3.1415926f * i / NUM_CIRCLE_SEGMENTS;
        // converting polar coordinates to cartesian
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
    float x = 0.0f;
    float y = 0.0f;
    float vx = 0.0f;
    float vy = 0.0f;
    float radius = 0.1f;
    bool is_colliding = false;
};

class BallCollection {
private:
    std::vector<Ball*> Balls;

    float get_random_float()
    {
        // return a value between -1 and 1
        return distribution(gen);
    }
    Ball* createNewBall()
    {
        float new_x = get_random_float();
        float new_y = get_random_float();
        float new_vx = get_random_float();
        float new_vy = get_random_float();
        Ball* newBall = new Ball;
        newBall->x = new_x;
        newBall->y = new_y;
        newBall->vx = new_vx;
        newBall->vy = new_vy;
        return newBall;
    }

public:
    BallCollection(int numBalls = NUM_BALLS)
    {
        Balls.resize(numBalls);
        for (int i = 0; i < numBalls; i++) {
            Balls[i] = createNewBall();
        }
    }
    void renderBalls()
    {

        clearScreen();
        glLoadIdentity();
        for (int i = 0; i < Balls.size(); i++) {
            float x = Balls[i]->x;
            float y = Balls[i]->y;
            float radius = Balls[i]->radius;

            auto& bc = COLOURS.at("Black");

            glColor3f(bc.r, bc.g, bc.b);
            drawCircle(x, y, radius + borderThickness);

            auto& c = COLOURS.at(BALL_COLOURS.at(Balls[i]->is_colliding));

            glColor3f(c.r, c.g, c.b);
            drawCircle(x, y, radius);
        }
    }

    void handleWallCollisions(Ball* ball)
    {
        if (ball->x + ball->radius > 1.0f) {
            ball->x = 1.0f - ball->radius;
            ball->vx = -ball->vx;
        }
        if (ball->x - ball->radius < -1.0f) {
            ball->x = -1.0f + ball->radius;
            ball->vx = -ball->vx;
        }

        if (ball->y + ball->radius > 1.0f) {
            ball->y = 1.0f - ball->radius;
            ball->vy = -(ball->vy);
        }
        if (ball->y - ball->radius < -1.0f) {
            ball->y = -1.0f + ball->radius;
            ball->vy = -(ball->vy);
        }
    }

    bool areTouching(Ball* ball1, Ball* ball2)
    {
        // check the distance between their centers
        float x1 = ball1->x;
        float y1 = ball1->y;
        float x2 = ball2->x;
        float y2 = ball2->y;

        float distance = pow(pow(x1 - x2, 2) + pow(y1 - y2, 2), 0.5);

        if (distance <= ball1->radius + ball2->radius) {
            return true;
        }

        return false;
    }

    void particleCollisionCalculation(Ball* ball1, Ball* ball2)
    {
        // calculate and change the balls' velocity and position
        // draw a line between their centers and calculate the
    }

    void handleParticleCollisions(Ball* ball, int ballIndex)
    {
        ball->is_colliding = false;
        for (int nextBallIndex = 0; nextBallIndex < Balls.size(); nextBallIndex++) {
            if (nextBallIndex == ballIndex) {
                continue;
            }
            Ball* nextBall = Balls[nextBallIndex];
            bool touching = areTouching(ball, nextBall);
            if (touching) {
                ball->is_colliding = true;
            }
            if (touching && nextBallIndex > ballIndex) {
                particleCollisionCalculation(ball, nextBall);
            }
        }
    }

    void moveBall(Ball* ball, float timeDelta)
    {
        ball->x += ball->vx * timeDelta;
        ball->y += ball->vy * timeDelta;
    }

    void updateBallPositions(float timeDelta)
    {

        for (int ballIndex = 0; ballIndex < Balls.size(); ballIndex++) {
            // Update position
            Ball* ball = Balls[ballIndex];

            moveBall(ball, timeDelta);
            handleWallCollisions(ball);
            handleParticleCollisions(ball, ballIndex);

            // we want to move the x and y coord then handle wall collisions and particle collisions
            // we need to pass in the current ball

            // Bounce off walls
        }
    }
};

int main()
{
    if (!glfwInit())
        return -1;

    GLFWwindow* window = glfwCreateWindow(800, 800, "Bouncing Ball", nullptr, nullptr);
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

    float lastTime = (float)glfwGetTime();
    BallCollection balls = BallCollection();

    while (!glfwWindowShouldClose(window)) {
        float currentTime = (float)glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        balls.updateBallPositions(deltaTime);
        // Render
        balls.renderBalls();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
