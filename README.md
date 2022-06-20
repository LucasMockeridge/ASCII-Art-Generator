# ASCII-Art-Generator
A C++ program that generates ASCII art from 24-bit bitmap images.

## Compilation
    g++ main.cpp -o main -std=c++17 -Wall
    gcc main.cpp -o main -std=c++17 -Wall -lstdc++ -lm

## Usage
    usage: ./main image_path [ansi_code]
           image_path: path to a 24-bit bitmap image
           ansi_code: ansi code between 0 and 255
