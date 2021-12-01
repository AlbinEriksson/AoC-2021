#include "stdio.h"
#include "stdlib.h"

#if _WIN32
#include <Windows.h>
#else
#error "Target operating system not supported"
#endif

#include "solution.hpp"
#include "solutions/day1.hpp"
// #include "solutions/day2.hpp"
// #include "solutions/day3.hpp"
// #include "solutions/day4.hpp"
// #include "solutions/day5.hpp"
// #include "solutions/day6.hpp"
// #include "solutions/day7.hpp"
// #include "solutions/day8.hpp"
// #include "solutions/day9.hpp"
// #include "solutions/day10.hpp"
// #include "solutions/day11.hpp"
// #include "solutions/day12.hpp"
// #include "solutions/day13.hpp"
// #include "solutions/day14.hpp"
// #include "solutions/day15.hpp"
// #include "solutions/day16.hpp"
// #include "solutions/day17.hpp"
// #include "solutions/day18.hpp"
// #include "solutions/day19.hpp"
// #include "solutions/day20.hpp"
// #include "solutions/day21.hpp"
// #include "solutions/day22.hpp"
// #include "solutions/day23.hpp"
// #include "solutions/day24.hpp"
// #include "solutions/day25.hpp"

Solution get_solution(int day)
{
    Solution solution;
    switch(day)
    {
        case 1: solution.parse = day1_parse; solution.solve_part1 = day1_solve_part1; solution.solve_part2 = day1_solve_part2; break;
        // case 2: solution.parse = day2_parse; solution.solve_part1 = day2_solve_part1; solution.solve_part2 = day2_solve_part2; break;
        // case 3: solution.parse = day3_parse; solution.solve_part1 = day3_solve_part1; solution.solve_part2 = day3_solve_part2; break;
        // case 4: solution.parse = day4_parse; solution.solve_part1 = day4_solve_part1; solution.solve_part2 = day4_solve_part2; break;
        // case 5: solution.parse = day5_parse; solution.solve_part1 = day5_solve_part1; solution.solve_part2 = day5_solve_part2; break;
        // case 6: solution.parse = day6_parse; solution.solve_part1 = day6_solve_part1; solution.solve_part2 = day6_solve_part2; break;
        // case 7: solution.parse = day7_parse; solution.solve_part1 = day7_solve_part1; solution.solve_part2 = day7_solve_part2; break;
        // case 8: solution.parse = day8_parse; solution.solve_part1 = day8_solve_part1; solution.solve_part2 = day8_solve_part2; break;
        // case 9: solution.parse = day9_parse; solution.solve_part1 = day9_solve_part1; solution.solve_part2 = day9_solve_part2; break;
        // case 10: solution.parse = day10_parse; solution.solve_part1 = day10_solve_part1; solution.solve_part2 = day10_solve_part2; break;
        // case 11: solution.parse = day11_parse; solution.solve_part1 = day11_solve_part1; solution.solve_part2 = day11_solve_part2; break;
        // case 12: solution.parse = day12_parse; solution.solve_part1 = day12_solve_part1; solution.solve_part2 = day12_solve_part2; break;
        // case 13: solution.parse = day13_parse; solution.solve_part1 = day13_solve_part1; solution.solve_part2 = day13_solve_part2; break;
        // case 14: solution.parse = day14_parse; solution.solve_part1 = day14_solve_part1; solution.solve_part2 = day14_solve_part2; break;
        // case 15: solution.parse = day15_parse; solution.solve_part1 = day15_solve_part1; solution.solve_part2 = day15_solve_part2; break;
        // case 16: solution.parse = day16_parse; solution.solve_part1 = day16_solve_part1; solution.solve_part2 = day16_solve_part2; break;
        // case 17: solution.parse = day17_parse; solution.solve_part1 = day17_solve_part1; solution.solve_part2 = day17_solve_part2; break;
        // case 18: solution.parse = day18_parse; solution.solve_part1 = day18_solve_part1; solution.solve_part2 = day18_solve_part2; break;
        // case 19: solution.parse = day19_parse; solution.solve_part1 = day19_solve_part1; solution.solve_part2 = day19_solve_part2; break;
        // case 20: solution.parse = day20_parse; solution.solve_part1 = day20_solve_part1; solution.solve_part2 = day20_solve_part2; break;
        // case 21: solution.parse = day21_parse; solution.solve_part1 = day21_solve_part1; solution.solve_part2 = day21_solve_part2; break;
        // case 22: solution.parse = day22_parse; solution.solve_part1 = day22_solve_part1; solution.solve_part2 = day22_solve_part2; break;
        // case 23: solution.parse = day23_parse; solution.solve_part1 = day23_solve_part1; solution.solve_part2 = day23_solve_part2; break;
        // case 24: solution.parse = day24_parse; solution.solve_part1 = day24_solve_part1; solution.solve_part2 = day24_solve_part2; break;
        // case 25: solution.parse = day25_parse; solution.solve_part1 = day25_solve_part1; solution.solve_part2 = day25_solve_part2; break;
    }
    return solution;
}

int main(int argc, char *argv[])
{
    int day;
    if(argc == 2)
    {
        if(sscanf(argv[1], "%d", &day) == 0)
        {
            printf("Error: Expected number as argument\n");
            return EXIT_FAILURE;
        }
    }
    else
    {
        printf("Usage: aoc_2021 <day>\n");
    }

    char filename[256];
    sprintf_s(filename, "input/%d", day);

    Solution solution = get_solution(day);

    #if _WIN32
    LARGE_INTEGER perf_freq, parse_start, parse_end, solve_part1_start, solve_part1_end, solve_part2_start, solve_part2_end;
    QueryPerformanceFrequency(&perf_freq);

    QueryPerformanceCounter(&parse_start);
    #endif

    solution.parse(filename);

    #if _WIN32
    QueryPerformanceCounter(&parse_end);
    QueryPerformanceCounter(&solve_part1_start);
    #endif

    solution.solve_part1();

    #if _WIN32
    QueryPerformanceCounter(&solve_part1_end);
    QueryPerformanceCounter(&solve_part2_start);
    #endif

    solution.solve_part2();

    #if _WIN32
    QueryPerformanceCounter(&solve_part2_end);

    // float parse_time = (parse_end - parse_start) / (float)(perf_freq);
    // float solve_part1_time = (solve_part1_end - solve_part1_start) / (float)(perf_freq);
    // float solve_part2_time = (solve_part2_end - solve_part2_start) / (float)(perf_freq);
    #endif

    // printf("Parse time:        %4.6f seconds\n", parse_time);
    // printf("Part 1 solve time: %4.6f seconds\n", solve_part1_time);
    // printf("Part 2 solve time: %4.6f seconds\n", solve_part2_time);
}
