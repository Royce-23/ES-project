#pragma once
#include <cassert>
#include <stdint.h>
#include <string>
class LiquidCrystal {
public:
    std::string rows[2] = {std::string(16, ' '), std::string(16, ' ')};
    unsigned writes = 0;
    uint8_t x = 0, y = 0;
    LiquidCrystal(int, int, int, int, int, int) {}
    void begin(int cols, int lines) { assert(cols == 16 && lines == 2); }
    void setCursor(uint8_t col, uint8_t row) {
        assert(col < 16 && row < 2);
        x = col;
        y = row;
    }
    void write(uint8_t c) {
        assert(x < 16 && y < 2);
        rows[y][x++] = static_cast<char>(c);
        ++writes;
    }
};

