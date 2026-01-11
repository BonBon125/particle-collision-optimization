#!/bin/bash
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
	g++ main.cpp -lglfw -lGL -o bouncing_balls
elif [[ "$OSTYPE" == "darwin"* ]]; then
	clang++ main.cpp -std=c++17 -lglfw -framework OpenGL -o bouncing_balls

	echo "This is macOS"
else
	echo "Unknown OS: $OSTYPE"
	exit 1
fi

./bouncing_balls
