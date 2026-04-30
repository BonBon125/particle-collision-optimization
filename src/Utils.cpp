#include "Utils.h"
#include "Constants.h"
#include <cmath>
#include <iostream>

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
    { "Light Gray", { 0.8f, 0.8f, 0.8f } }, { "Gray", { 0.5f, 0.5f, 0.5f } },
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

const std::unordered_map<bool, std::string> BALL_COLOURS = { { true, "Red" }, { false, "Teal" } };
std::string BACKGROUND_COLOUR = "Black";

float RandomGen::get(float min, float max)
{
    static std::mt19937 gen(std::random_device {}());
    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}

void drawCircle(float cx, float cy, float r)
{
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= NUM_CIRCLE_SEGMENTS; i++) {
        float angle = 2.0f * 3.1415926f * i / NUM_CIRCLE_SEGMENTS;
        glVertex2f(cx + cos(angle) * r, cy + sin(angle) * r);
    }
    glEnd();
}

void clearScreen()
{
    auto& c = COLOURS.at(BACKGROUND_COLOUR);
    glClearColor(c.r, c.g, c.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}
