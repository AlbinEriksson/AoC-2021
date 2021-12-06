#pragma once

#if _WIN32
#include <Windows.h>
#endif

#define DAY_INPUT_STRUCT(day_num) Day ## day_num ## Input
#define DAY_OUTPUT_STRUCT(day_num) Day ## day_num ## Output

#define MAKE_SOLUTION_FUNCS(day_num) \
    struct DAY_INPUT_STRUCT(day_num); \
    struct DAY_OUTPUT_STRUCT(day_num); \
    const DAY_INPUT_STRUCT(day_num)* day ## day_num ## _parse(const char *filename); \
    DAY_OUTPUT_STRUCT(day_num)* day ## day_num ## _alloc_output(); \
    void day ## day_num ## _solve_part1(const DAY_INPUT_STRUCT(day_num) *input, DAY_OUTPUT_STRUCT(day_num) *output); \
    void day ## day_num ## _solve_part2(const DAY_INPUT_STRUCT(day_num) *input, DAY_OUTPUT_STRUCT(day_num) *output); \
    void day ## day_num ## _print_answer(const DAY_OUTPUT_STRUCT(day_num) *output); \
    void day ## day_num ## _free(const DAY_INPUT_STRUCT(day_num) *input, const DAY_OUTPUT_STRUCT(day_num) *output);

MAKE_SOLUTION_FUNCS(1)
MAKE_SOLUTION_FUNCS(2)
MAKE_SOLUTION_FUNCS(3)
MAKE_SOLUTION_FUNCS(4)
MAKE_SOLUTION_FUNCS(5)
MAKE_SOLUTION_FUNCS(6)
MAKE_SOLUTION_FUNCS(7)
MAKE_SOLUTION_FUNCS(8)
MAKE_SOLUTION_FUNCS(9)
MAKE_SOLUTION_FUNCS(10)
MAKE_SOLUTION_FUNCS(11)
MAKE_SOLUTION_FUNCS(12)
MAKE_SOLUTION_FUNCS(13)
MAKE_SOLUTION_FUNCS(14)
MAKE_SOLUTION_FUNCS(15)
MAKE_SOLUTION_FUNCS(16)
MAKE_SOLUTION_FUNCS(17)
MAKE_SOLUTION_FUNCS(18)
MAKE_SOLUTION_FUNCS(19)
MAKE_SOLUTION_FUNCS(20)
MAKE_SOLUTION_FUNCS(21)
MAKE_SOLUTION_FUNCS(22)
MAKE_SOLUTION_FUNCS(23)
MAKE_SOLUTION_FUNCS(24)
MAKE_SOLUTION_FUNCS(25)

using ParseFunc = const void* (*)(const char *filename);
using AllocOutputFunc = void* (*)();
using SolveFunc = void (*)(const void *input, void *output);
using PrintAnswerFunc = void (*)(const void *output);
using FreeFunc = void (*)(const void *input, const void *output);

struct Solution
{
    ParseFunc parse;
    AllocOutputFunc alloc_output;
    SolveFunc solve_part1;
    SolveFunc solve_part2;
    PrintAnswerFunc print_answer;
    FreeFunc free;
};

// case 1: solution.parse = (ParseFunc)day1_parse; solution.solve_part1 = (SolveFunc)day1_solve_part1; solution.solve_part2 = (SolveFunc)day1_solve_part2; solution.free = (FreeFunc)day1_free; break;
#define GET_SOLUTION_CASE(day_num) \
    case day_num ## : \
    { \
        solution.parse = (ParseFunc)day ## day_num ## _parse; \
        solution.alloc_output = (AllocOutputFunc)day ## day_num ## _alloc_output; \
        solution.solve_part1 = (SolveFunc)day ## day_num ## _solve_part1; \
        solution.solve_part2 = (SolveFunc)day ## day_num ## _solve_part2; \
        solution.print_answer = (PrintAnswerFunc)day ## day_num ## _print_answer; \
        solution.free = (FreeFunc)day ## day_num ## _free; \
        break; \
    }

inline Solution get_solution(int day)
{
    Solution solution;
    switch(day)
    {
        GET_SOLUTION_CASE(1)
        GET_SOLUTION_CASE(2)
        GET_SOLUTION_CASE(3)
        GET_SOLUTION_CASE(4)
        // GET_SOLUTION_CASE(5)
        GET_SOLUTION_CASE(6)
        // GET_SOLUTION_CASE(7)
        // GET_SOLUTION_CASE(8)
        // GET_SOLUTION_CASE(9)
        // GET_SOLUTION_CASE(10)
        // GET_SOLUTION_CASE(11)
        // GET_SOLUTION_CASE(12)
        // GET_SOLUTION_CASE(13)
        // GET_SOLUTION_CASE(14)
        // GET_SOLUTION_CASE(15)
        // GET_SOLUTION_CASE(16)
        // GET_SOLUTION_CASE(17)
        // GET_SOLUTION_CASE(18)
        // GET_SOLUTION_CASE(19)
        // GET_SOLUTION_CASE(20)
        // GET_SOLUTION_CASE(21)
        // GET_SOLUTION_CASE(22)
        // GET_SOLUTION_CASE(23)
        // GET_SOLUTION_CASE(24)
        // GET_SOLUTION_CASE(25)
    }
    return solution;
}

using u64 = unsigned long long;
using u32 = unsigned int;
using u16 = unsigned short;
using u8 = unsigned char;

using s64 = long long;
using s32 = int;
using s16 = short;
using s8 = char;

#define U64_MAX ((u64)0xFFFFFFFFFFFFFFFF)
#define U32_MAX ((u32)0xFFFFFFFF)
#define U16_MAX ((u16)0xFFFF)
#define U8_MAX ((u8)0xFF)

#define S64_MIN ((s64)0x8000000000000000)
#define S32_MIN ((s32)0x80000000)
#define S16_MIN ((s16)0x8000)
#define S8_MIN ((s8)0x80)

#define S64_MAX ((s64)0x7FFFFFFFFFFFFFFF)
#define S32_MAX ((s32)0x7FFFFFFF)
#define S16_MAX ((s16)0x7FFF)
#define S8_MAX ((s8)0x7F)
