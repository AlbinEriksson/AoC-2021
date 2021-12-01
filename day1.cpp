#include <cstdlib>
#include <cstdio>
#include <intrin.h>

#include "common.hpp"

struct Day1Input
{
    u16 *depths;
    u32 max_depth_count;
    u32 depth_count;
    
    void add_depth(u16 depth)
    {
        depths[depth_count] = depth;
        depth_count++;
        if(depth_count >= max_depth_count)
        {
            max_depth_count *= 2;
            depths = (u16*)_aligned_realloc(depths, sizeof(u16) * max_depth_count, 32);
        }
    }
};

struct Day1Output
{
    u32 increase_count;
    u32 sliding_increase_count;
};

const Day1Input* day1_parse(const char *filename)
{
    Day1Input input;

    input.depth_count = 0;
    input.max_depth_count = 1;
    input.depths = (u16*)_aligned_malloc(sizeof(u16) * input.max_depth_count, 32);

    #if _WIN32
    {
        OFSTRUCT ofs;
        HANDLE file = CreateFileA(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        HANDLE mapping = CreateFileMapping(file, NULL, PAGE_READONLY | SEC_COMMIT, 0, 0, NULL);
        const char *contents = (const char*)MapViewOfFile(mapping, FILE_MAP_READ, 0, 0, 0);
        DWORD size = GetFileSize(file, NULL);

        u16 depth = 0;
        for(s32 i = 0; i < size; ++i)
        {
            char c = contents[i];
            if(c >= '0')
            {
                depth = depth * 10 + c - '0';
            }
            else
            {
                input.add_depth(depth);
                depth = 0;
            }
        }

        UnmapViewOfFile(contents);
        CloseHandle(mapping);
        CloseHandle(file);
    }
    #else
    {
        FILE *fp = std::fopen(filename, "r");

        u16 depth;
        s32 read = 0;
        while(read != EOF)
        {
            depth = 0;
            char c;
            
            read = std::fgetc(fp);
            while(read >= '0')
            {
                depth = depth * 10 + read - '0';
                read = std::fgetc(fp);
            }

            input.add_depth(depth);
        }

        std::fclose(fp);
    }
    #endif

    return new Day1Input(input);
}

Day1Output* day1_alloc_output()
{
    return new Day1Output;
}

void day1_solve_part1(const Day1Input *input, Day1Output *output)
{
    u32 increase_count = 0;
    u16 prev_depth = input->depths[0];
    for(u32 i = 1; i < input->depth_count; ++i)
    {
        u16 depth = input->depths[i];
        increase_count += depth > prev_depth;
        prev_depth = depth;
    }

    output->increase_count = increase_count;
}

void day1_solve_part2(const Day1Input *input, Day1Output *output)
{
    u32 increase_count = 0;
    u32 sliding_sum = input->depths[1] + input->depths[2];
    u32 prev_sum = input->depths[0] + sliding_sum;
    for(int i = 3; i < input->depth_count; ++i)
    {
        sliding_sum += input->depths[i];
        increase_count += sliding_sum > prev_sum;
        prev_sum = sliding_sum;
        sliding_sum -= input->depths[i - 2];
    }

    output->sliding_increase_count = increase_count;
}

void day1_print_answer(const Day1Output *output)
{
    printf("Depth increase count: %d\n", output->increase_count);
    printf("Sliding increase count: %d\n", output->sliding_increase_count);
}

void day1_free(const Day1Input *input, const Day1Output *output)
{
    _aligned_free(input->depths);
    delete input;
    delete output;
}
