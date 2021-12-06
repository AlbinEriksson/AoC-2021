#include <cstdlib>
#include <cstdio>
#include <intrin.h>

#include "common.hpp"

struct Day6Input
{
    u8 *timers;
    u32 timer_count;
    u32 max_timer_count;

    void add_timer(u8 timer)
    {
        timers[timer_count] = timer;
        ++timer_count;

        if(timer_count >= max_timer_count)
        {
            max_timer_count *= 2;
            timers = (u8*)std::realloc(timers, sizeof(u8) * max_timer_count);
        }
    }
};

struct Day6Output
{
    u64 lanternfish_amount_80;
    u64 lanternfish_amount_256;
};

const Day6Input* day6_parse(const char *filename)
{
    Day6Input input;
    input.max_timer_count = 1;
    input.timer_count = 0;
    input.timers = (u8*)std::malloc(sizeof(u8) * input.max_timer_count);

    #if _WIN32
    {
        HANDLE file = CreateFileA(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        HANDLE mapping = CreateFileMapping(file, NULL, PAGE_READONLY | SEC_COMMIT, 0, 0, NULL);
        const char *contents = (const char*)MapViewOfFile(mapping, FILE_MAP_READ, 0, 0, 0);
        DWORD size = GetFileSize(file, NULL);

        for(u32 i = 0; i < size; i += 2)
        {
            u8 timer = contents[i] - '0';
            input.add_timer(timer);
        }

        UnmapViewOfFile(contents);
        CloseHandle(mapping);
        CloseHandle(file);
    }
    #endif

    return new Day6Input(input);
}

Day6Output* day6_alloc_output()
{
    return new Day6Output;
}

u64 get_total_lanternfish_after(const Day6Input *input, u32 day_count)
{
    u64 timer_counts[9] = {};
    for(u32 i = 0; i < input->timer_count; ++i)
        ++timer_counts[input->timers[i]];
    
    for(u32 i = 0; i < day_count; ++i)
    {
        __m256i move1 = _mm256_loadu_epi64(&timer_counts[1]);
        __m256i move2 = _mm256_loadu_epi64(&timer_counts[5]);
        u64 spawn_count = timer_counts[0];
        _mm256_storeu_epi64(&timer_counts[0], move1);
        _mm256_storeu_epi64(&timer_counts[4], move2);

        timer_counts[6] += spawn_count;
        timer_counts[8] = spawn_count;
    }

    u64 total = timer_counts[0] + timer_counts[1] + timer_counts[2] + timer_counts[3] + timer_counts[4] + timer_counts[5] + timer_counts[6] + timer_counts[7] + timer_counts[8];
    return total;
}

void day6_solve_part1(const Day6Input *input, Day6Output *output)
{
    output->lanternfish_amount_80 = get_total_lanternfish_after(input, 80);
}

void day6_solve_part2(const Day6Input *input, Day6Output *output)
{
    output->lanternfish_amount_256 = get_total_lanternfish_after(input, 256);
}

void day6_print_answer(const Day6Output *output)
{
    printf("Population after 80 days:  %llu\n\n", output->lanternfish_amount_80);
    printf("Population after 256 days: %llu\n\n", output->lanternfish_amount_256);
}

void day6_free(const Day6Input *input, const Day6Output *output)
{
    std::free(input->timers);
    delete input;
    delete output;
}
