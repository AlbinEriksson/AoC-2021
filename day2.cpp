#include <cstdlib>
#include <cstdio>

#include "common.hpp"

enum class Dir : u8
{
    Forward,
    Down,
    Up
};

struct Move
{
    Dir dir;
    u8 dist;
};

struct Day2Input
{
    Move *moves;
    u32 max_move_count;
    u32 move_count;
    
    void add_move(Dir dir, u8 dist)
    {
        moves[move_count] = {dir, dist};
        move_count++;
        if(move_count >= max_move_count)
        {
            max_move_count *= 2;
            moves = (Move*)std::realloc(moves, sizeof(Move) * max_move_count);
        }
    }
    
};

struct Day2Output
{
    s32 x;
    s32 y;
    s32 product;

    s32 x2;
    s32 y2;
    s32 product2;
};

const Day2Input* day2_parse(const char *filename)
{
    Day2Input input;

    input.move_count = 0;
    input.max_move_count = 1;
    input.moves = (Move*)std::malloc(sizeof(Move) * input.max_move_count);

    #if _WIN32
    {
        OFSTRUCT ofs;
        HANDLE file = CreateFileA(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        HANDLE mapping = CreateFileMapping(file, NULL, PAGE_READONLY | SEC_COMMIT, 0, 0, NULL);
        const char *contents = (const char*)MapViewOfFile(mapping, FILE_MAP_READ, 0, 0, 0);
        DWORD size = GetFileSize(file, NULL);

        for(s32 i = 0; i < size;)
        {
            char c = contents[i];
            
            Dir dir;
            switch(c)
            {
                case 'f': dir = Dir::Forward; i += 8; break;
                case 'd': dir = Dir::Down; i += 5; break;
                case 'u': dir = Dir::Up; i += 3; break;
            }

            c = contents[i];
            u8 dist = c - '0';

            input.add_move(dir, dist);

            i += 2;
        }

        UnmapViewOfFile(contents);
        CloseHandle(mapping);
        CloseHandle(file);
    }
    #endif

    return new Day2Input(input);
}

Day2Output* day2_alloc_output()
{
    return new Day2Output();
}

void day2_solve_part1(const Day2Input *input, Day2Output *output)
{
    s32 x = 0;
    s32 y = 0;

    for(u32 i = 0; i < input->move_count; ++i)
    {
        Move &move = input->moves[i];
        x += (move.dir == Dir::Forward) * move.dist;
        y += ((move.dir == Dir::Down) - (move.dir == Dir::Up)) * move.dist;
    }

    output->x = x;
    output->y = y;
    output->product = x * y;
}

void day2_solve_part2(const Day2Input *input, Day2Output *output)
{
    s32 x = 0;
    s32 y = 0;
    s32 aim = 0;

    for(u32 i = 0; i < input->move_count; ++i)
    {
        Move &move = input->moves[i];
        x += (move.dir == Dir::Forward) * move.dist;
        y += (move.dir == Dir::Forward) * aim * move.dist;
        aim += ((move.dir == Dir::Down) - (move.dir == Dir::Up)) * move.dist;
    }

    output->x2 = x;
    output->y2 = y;
    output->product2 = x * y;
}

void day2_print_answer(const Day2Output *output)
{
    printf("Part 1\n");
    printf("Position: %d, %d\n", output->x, output->y);
    printf("Product:  %d\n\n", output->product);
    printf("Part 2\n");
    printf("Position: %d, %d\n", output->x2, output->y2);
    printf("Product:  %d\n\n", output->product2);
}

void day2_free(const Day2Input *input, const Day2Output *output)
{
    std::free(input->moves);
    delete input;
    delete output;
}
