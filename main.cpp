#include "stdio.h"
#include "stdlib.h"

#if _WIN32
#include <Windows.h>
#else
#error "Target operating system not supported"
#endif

#include "common.hpp"

int main(int argc, char *argv[])
{
    u32 day, loops = 1;
    if(argc >= 2 && argc <= 3)
    {
        if(sscanf(argv[1], "%d", &day) == 0)
        {
            printf("Error: Expected day number as first argument\n");
            return EXIT_FAILURE;
        }

        if(argc == 3 && sscanf(argv[2], "%d", &loops) == 0)
        {
            printf("Error: Expected loop count as second argument\n");
            return EXIT_FAILURE;
        }
    }
    else
    {
        printf("Usage: aoc_2021 <day> [loops]\n");
    }

    char filename[256];
    sprintf_s(filename, "input/%d", day);

    Solution solution = get_solution(day);

    #if _WIN32
    u64 total_parse_ticks = 0, total_part1_ticks = 0, total_part2_ticks = 0, total_free_ticks = 0;
    LARGE_INTEGER perf_freq, parse_start, parse_end, solve_part1_start, solve_part1_end, solve_part2_start, solve_part2_end, free_start, free_end;
    QueryPerformanceFrequency(&perf_freq);
    #endif

    for(u32 i = 0; i < loops; ++i)
    {
        #if _WIN32
        QueryPerformanceCounter(&parse_start);
        #endif

        const void *input = solution.parse(filename);

        #if _WIN32
        QueryPerformanceCounter(&parse_end);
        #endif

        void *output = solution.alloc_output();

        #if _WIN32
        QueryPerformanceCounter(&solve_part1_start);
        #endif

        solution.solve_part1(input, output);

        #if _WIN32
        QueryPerformanceCounter(&solve_part1_end);
        QueryPerformanceCounter(&solve_part2_start);
        #endif

        solution.solve_part2(input, output);

        #if _WIN32
        QueryPerformanceCounter(&solve_part2_end);
        #endif

        if(i == 0)
        {
            solution.print_answer(output);
        }

        #if _WIN32
        QueryPerformanceCounter(&free_start);
        #endif

        solution.free(input, output);

        #if _WIN32
        QueryPerformanceCounter(&free_end);
        
        total_parse_ticks += parse_end.QuadPart - parse_start.QuadPart;
        total_part1_ticks += solve_part1_end.QuadPart - solve_part1_start.QuadPart;
        total_part2_ticks += solve_part2_end.QuadPart - solve_part2_start.QuadPart;
        total_free_ticks += free_end.QuadPart - free_start.QuadPart;
        #endif
    }

    #if _WIN32
    double parse_time = total_parse_ticks / (double)(perf_freq.QuadPart) / (double)(loops);
    double solve_part1_time = total_part1_ticks / (double)(perf_freq.QuadPart) / (double)(loops);
    double solve_part2_time = total_part2_ticks / (double)(perf_freq.QuadPart) / (double)(loops);
    double free_time = total_free_ticks / (double)(perf_freq.QuadPart) / (double)(loops);
    #endif

    printf("Parse time:        %10.6f seconds\n", parse_time);
    printf("Part 1 solve time: %10.6f seconds\n", solve_part1_time);
    printf("Part 2 solve time: %10.6f seconds\n", solve_part2_time);
    printf("Free time:         %10.6f seconds\n", free_time);
}
