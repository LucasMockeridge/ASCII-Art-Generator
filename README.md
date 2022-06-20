# ASCII-Art-Generator
A C++ program that generates ASCII art from 24-bit bitmap images.

## Compilation
    gcc main.cpp -o main -lstdc++ -lm -Wall -std=c++17	

## Usage
    usage: ./main image_path [ansi_code]
           image_path: path to a 24-bit bitmap image
           ansi_code: ansi code between 0 and 255
