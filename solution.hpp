#pragma once

struct Solution
{
    void (*parse)(const char *filename);
    void (*solve_part1)();
    void (*solve_part2)();
};
