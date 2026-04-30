#pragma once
#include <GL/gl.h>
#include <random>
#include <string>
#include <unordered_map>

struct Colour {
    float r, g, b;
};

extern const std::unordered_map<std::string, Colour> COLOURS;
extern const std::unordered_map<bool, std::string> BALL_COLOURS;
extern std::string BACKGROUND_COLOUR;

class RandomGen {
public:
    static float get(float min, float max);
};

void drawCircle(float cx, float cy, float r);
void clearScreen();
