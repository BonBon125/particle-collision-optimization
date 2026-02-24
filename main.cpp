#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <algorithm>
#include <array>
#include <bits/stdc++.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <iostream>
#include <ostream>
#include <random>
#include <string>
#include <unistd.h>
#include <unordered_map>
#include <vector>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

const float RECORDING_DURATION = 4.0f;

const int WIDTH_HEIGHT = 1000;

const int NUM_CIRCLE_SEGMENTS = 10;
const int NUM_PARTICLES = 30000;

const float MIN_BALL_RADIUS = 0.0015f;
const float MAX_BALL_RADIUS = 0.0025f;
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

const float MAX_BALL_SPEED = 0.1f;

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

struct Particle {
    float x, y;
    float vx, vy;
    float radius;
    bool is_colliding = false;
};

class ParticleSystem {
protected:
    std::array<Particle*, NUM_PARTICLES> mParticles;

    Particle* createParticle()
    {
        Particle* newParticle = new Particle;
        newParticle->x = position_dist(gen);
        newParticle->y = position_dist(gen);
        newParticle->vx = velocity_dist(gen);
        newParticle->vy = velocity_dist(gen);
        newParticle->radius = radius_dist(gen);
        return newParticle;
    }

    bool areTouching(Particle* a, Particle* b)
    {
        float dx = a->x - b->x;
        float dy = a->y - b->y;
        float hypot = std::hypot(dx, dy);
        float r = a->radius + b->radius;
        return hypot <= r;
    }

    void resetParticleCollisions()
    {
        for (Particle* particle : mParticles) {
            particle->is_colliding = false;
        }
    }

    void resolveParticleCollision(Particle* particle1, Particle* particle2)
    {
        // Vector between centers
        float dx = particle2->x - particle1->x;
        float dy = particle2->y - particle1->y;
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
        float v1n = particle1->vx * nx + particle1->vy * ny;
        float v1t = particle1->vx * tx + particle1->vy * ty;

        float v2n = particle2->vx * nx + particle2->vy * ny;
        float v2t = particle2->vx * tx + particle2->vy * ty;

        // Equal mass elastic collision → swap normal components
        // Equal mass inelastic collision using coefficient of restitution
        float v1n_after = (v1n + v2n - 1.0 * (v1n - v2n)) * 0.5f;
        float v2n_after = (v1n + v2n + 1.0 * (v1n - v2n)) * 0.5f;

        // Convert scalar normal/tangent velocities back to vectors
        particle1->vx = v1n_after * nx + v1t * tx;
        particle1->vy = v1n_after * ny + v1t * ty;

        particle2->vx = v2n_after * nx + v2t * tx;
        particle2->vy = v2n_after * ny + v2t * ty;

        // // Optional: positional correction to prevent sticking
        float overlap = (particle1->radius + particle2->radius) - distance;
        if (overlap > 0.0f) {
            float correction = overlap * 0.5f;
            particle1->x -= nx * correction;
            particle1->y -= ny * correction;
            particle2->x += nx * correction;
            particle2->y += ny * correction;
        }

        particle1->is_colliding = true;
        particle2->is_colliding = true;
    }

    void handleWallCollisions(Particle* particle)
    {
        float abs_x_boundary = WORLD_MAX - particle->radius;
        float abs_y_boundary = WORLD_MAX - particle->radius;

        particle->x = std::clamp(particle->x, -abs_x_boundary, abs_x_boundary);
        particle->y = std::clamp(particle->y, -abs_y_boundary, abs_y_boundary);

        particle->vx = (std::abs(particle->x) == abs_x_boundary) ? -particle->vx : particle->vx;
        particle->vy = (std::abs(particle->y) == abs_y_boundary) ? -particle->vy : particle->vy;

        particle->is_colliding = (std::abs(particle->x) == abs_x_boundary || std::abs(particle->y) == abs_y_boundary) ? true : particle->is_colliding;
    }

    void moveParticle(Particle* particle, float timeDelta)
    {
        particle->x += particle->vx * timeDelta;
        particle->y += particle->vy * timeDelta;
    }

public:
    ParticleSystem()
    {
        for (int i = 0; i < NUM_PARTICLES; i++) {
            mParticles[i] = createParticle();
        }
    }

    void render()
    {
        clearScreen();
        glLoadIdentity();
        for (int i = 0; i < mParticles.size(); i++) {
            float x = mParticles[i]->x;
            float y = mParticles[i]->y;
            float radius = mParticles[i]->radius;
            auto& c = COLOURS.at(BALL_COLOURS.at(mParticles[i]->is_colliding));

            glColor3f(c.r, c.g, c.b);
            drawCircle(x, y, radius);
        }
    }

    virtual void update(float timeDelta) = 0;
};

// We want to try and create a subclass that extends the ParticleSystem
// This subclass should just have the adjustment for spatial partitioning

class ParticleSystem_V1 : public ParticleSystem {
private:
    void handleParticleCollisions()
    {
        resetParticleCollisions();

        for (int i = 0; i < NUM_PARTICLES; i++) {
            for (int j = i; j < NUM_PARTICLES; j++) {
                if (areTouching(mParticles[i], mParticles[j])) {
                    resolveParticleCollision(mParticles[i], mParticles[j]);
                }
            }
        }
    }

public:
    void update(float timeDelta)
    {
        handleParticleCollisions();

        for (Particle* pParticle : mParticles) {
            handleWallCollisions(pParticle);
            moveParticle(pParticle, timeDelta);
        }
    }
};

class ParticleSystem_V2 : public ParticleSystem {
private:
    std::vector<std::vector<int>> grid;

    int cellX(float x)
    {
        return int((x - WORLD_MIN) / CELL_SIZE);
    }
    int cellY(float y)
    {
        return int((y - WORLD_MIN) / CELL_SIZE);
    }
    int cellIndex(int x, int y)
    {
        return y * GRID_WIDTH + x;
    }

    void rebuildGrid()
    {
        // Remove all values in the grid
        for (auto& cell : grid) {
            cell.clear();
        }

        // Add each ball to the corresponding gridbox
        for (int i = 0; i < NUM_PARTICLES; i++) {
            Particle* p = mParticles[i];

            // Convert the x and y values to the grid x and y values
            int cx = std::clamp(cellX(p->x), 0, GRID_WIDTH - 1);
            int cy = std::clamp(cellY(p->y), 0, GRID_HEIGHT - 1);
            grid[cellIndex(cx, cy)].push_back(i);
        }
    }
    void handleParticleCollisions()
    {
        resetParticleCollisions();

        for (int i = 0; i < NUM_PARTICLES; i++) {
            // For every ball
            Particle* p = mParticles[i];

            int cx = std::clamp(cellX(p->x), 0, GRID_WIDTH - 1);
            int cy = std::clamp(cellY(p->y), 0, GRID_HEIGHT - 1);

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
                        if (areTouching(p, mParticles[j])) {
                            resolveParticleCollision(p, mParticles[j]);
                        }
                    }
                }
            }
        }
    }

public:
    ParticleSystem_V2()
    {
        grid.resize(GRID_WIDTH * GRID_HEIGHT);
    }
    void update(float timeDelta)
    {
        rebuildGrid();
        handleParticleCollisions();

        for (Particle* p : mParticles) {
            handleWallCollisions(p);
            moveParticle(p, timeDelta);
        }
    }
};

void displayFPS(float dt)
{
    float fps = 1.0 / dt;
    std::cout << "\r" << fps << std::flush;
}

// TODO: We want to take the frames from openGL then create pngs according to those images

struct ThreadedPNGRecorder {
    int width = WIDTH_HEIGHT;
    int height = WIDTH_HEIGHT;
    int fps = 60;
    std::atomic<bool> recording { false };
    std::atomic<bool> done { false };
    int frameCounter = 0;

    std::queue<std::vector<unsigned char>> frameQueue;
    std::mutex queueMutex;
    std::condition_variable queueCV;
    std::thread writerThread;

    // Start recording
    void start(int fps_ = 60)
    {
        fps = fps_;
        frameCounter = 0;
        recording = true;
        done = false;

        writerThread = std::thread([this]() { this->threadedWriter(); });
        std::cout << "\nRecording started.\n";
    }

    // Capture frame (non-blocking)
    void captureFrame()
    {
        if (!recording)
            return;

        std::vector<unsigned char> rgbBuffer(width * height * 3);
        std::vector<unsigned char> flippedBuffer(width * height * 3);

        glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, rgbBuffer.data());

        for (int y = 0; y < height; y++) {
            memcpy(&flippedBuffer[y * width * 3],
                &rgbBuffer[(height - 1 - y) * width * 3],
                width * 3);
        }

        // Push to queue
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            frameQueue.push(std::move(flippedBuffer));
        }
        queueCV.notify_one();
    }

    // Stop recording
    void stop()
    {
        recording = false;
        done = true;
        queueCV.notify_all();
        if (writerThread.joinable())
            writerThread.join();
        std::cout << "Recording stopped. Total frames: " << frameCounter << "\n";
    }

    // Thread function
    void threadedWriter()
    {
        while (!done || !frameQueue.empty()) {
            std::vector<unsigned char> frame;
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                queueCV.wait(lock, [this] { return !frameQueue.empty() || done; });
                if (!frameQueue.empty()) {
                    frame = std::move(frameQueue.front());
                    frameQueue.pop();
                } else
                    continue;
            }

            // Build filename
            std::ostringstream name;
            name << "images/frame_" << std::setw(5) << std::setfill('0') << frameCounter++ << ".png";

            stbi_write_png(name.str().c_str(), width, height, 3, frame.data(), width * 3);
        }
    }

    // Export MP4 via ffmpeg
    void exportMP4(const std::string& outputFile = "gif-directory/simulation.mp4")
    {
        stop(); // ensure recording stopped

        std::ostringstream cmd;
        cmd << "ffmpeg -y -framerate " << fps
            << " -i images/frame_%05d.png -pix_fmt yuv420p "
            << outputFile;

        std::cout << "Exporting MP4... this may take a moment.\n";
        std::system(cmd.str().c_str());
        std::cout << "Export complete: " << outputFile << "\n";
    }

    void exportGIF(const std::string& outputFile = "gif-directory/simulation.gif")
    {
        stop(); // make sure recording fully stopped + writer thread finished

        std::ostringstream cmd;

        // High-quality GIF via palette generation (much better colours)
        cmd << "ffmpeg -y -framerate " << fps << " -i images/frame_%05d.png "
                                                 " -vf \"fps="
            << fps << ",scale=800:-1:flags=lanczos,palettegen\" palette.png";

        std::cout << "Generating GIF palette...\n";
        std::system(cmd.str().c_str());

        std::ostringstream cmd2;
        cmd2 << "ffmpeg -y -framerate " << fps << " -i images/frame_%05d.png -i palette.png "
                                                  " -lavfi \"fps="
             << fps << ",scale=800:-1:flags=lanczos[x];[x][1:v]paletteuse\" "
             << outputFile;

        std::cout << "Exporting GIF...\n";
        std::system(cmd2.str().c_str());

        std::cout << "GIF saved as: " << outputFile << "\n";
    }
};

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

    ParticleSystem_V2 particleSystem = ParticleSystem_V2();

    ThreadedPNGRecorder recorder;
    recorder.start(60);

    float lastTime = (float)glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        float currentTime = (float)glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        particleSystem.update(deltaTime);
        particleSystem.render();

        // displayFPS(deltaTime);

        // Capture frame every render
        if (recorder.recording) {
            recorder.captureFrame();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

        if (currentTime > RECORDING_DURATION) {
            glfwSetWindowShouldClose(window, true);
        }
    }
    recorder.stop();
    recorder.exportMP4();

    glfwTerminate();
    return 0;
}
