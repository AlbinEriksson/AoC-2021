#include <cstdlib>
#include <cstdio>
#include <intrin.h>

#include "common.hpp"

struct Board
{
    u8 numbers[5 * 5];
};

struct Day4Input
{
    u8 *numbers_to_draw;
    u32 draw_count;
    u32 max_draw_count;

    Board *boards;
    u32 board_count;
    u32 max_board_count;
    
    void add_number_to_draw(u8 number_to_draw)
    {
        numbers_to_draw[draw_count] = number_to_draw;
        draw_count++;
        if(draw_count >= max_draw_count)
        {
            max_draw_count *= 2;
            numbers_to_draw = (u8*)std::realloc(numbers_to_draw, sizeof(u8) * max_draw_count);
        }
    }
    
    void add_board(const Board &board)
    {
        boards[board_count] = board;
        board_count++;
        if(board_count >= max_board_count)
        {
            max_board_count *= 2;
            boards = (Board*)std::realloc(boards, sizeof(Board) * max_board_count);
        }
    }
};

struct Day4Output
{
    u32 unmarked_sum;
    u32 last_draw;
    u32 product;

    u32 worst_unmarked_sum;
    u32 worst_last_draw;
    u32 worst_product;
};

const Day4Input* day4_parse(const char *filename)
{
    Day4Input input;

    input.draw_count = 0;
    input.max_draw_count = 1;
    input.numbers_to_draw = (u8*)std::malloc(sizeof(u8) * input.max_draw_count);
    
    input.board_count = 0;
    input.max_board_count = 1;
    input.boards = (Board*)std::malloc(sizeof(Board) * input.max_board_count);

    #if _WIN32
    {
        HANDLE file = CreateFileA(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        HANDLE mapping = CreateFileMapping(file, NULL, PAGE_READONLY | SEC_COMMIT, 0, 0, NULL);
        const char *contents = (const char*)MapViewOfFile(mapping, FILE_MAP_READ, 0, 0, 0);
        DWORD size = GetFileSize(file, NULL);

        u8 number = 0;
        char c = '\0';
        u32 i;
        for(i = 0; c != '\n'; ++i)
        {
            c = contents[i];
            if(c >= '0')
            {
                number = number * 10 + c - '0';
            }
            else
            {
                input.add_number_to_draw(number);
                number = 0;
            }
        }

        ++i;

        while(i < size)
        {
            Board board;
            for(u32 j = 0; j < 25; ++j, i += 3)
            {
                c = contents[i];
                board.numbers[j] = (c != ' ') * (c - '0') * 10 + contents[i + 1] - '0';
            }
            input.add_board(board);
            ++i;
        }

        UnmapViewOfFile(contents);
        CloseHandle(mapping);
        CloseHandle(file);
    }
    #endif

    return new Day4Input(input);
}

Day4Output* day4_alloc_output()
{
    return new Day4Output;
}

#define CHECK_BINGO(x,y) (((x) & (y)) == (y))

bool mark_board_and_check_bingo(Board &board, u32 *marked_numbers, __m256i draw_vector, __m256i zero, __m256i index_mask)
{
    __m256i search = _mm256_loadu_epi8(&board.numbers);
    search = _mm256_cmpeq_epi8(search, draw_vector);
    search = _mm256_blendv_epi8(zero, index_mask, search);
    __m128i low = _mm256_extracti128_si256(search, 0); // Can't do epi8 horizontal add on __m256i, so I split into two __m128i instead
    __m128i high = _mm256_extracti128_si256(search, 1);
    __m128i sum = _mm_add_epi8(low, high);
    search = _mm256_cvtepu8_epi16(sum);
    search = _mm256_hadd_epi16(search, zero);
    search = _mm256_hadd_epi16(search, zero);
    search = _mm256_hadd_epi16(search, zero);

    u32 mark_index = search.m256i_i16[0] + search.m256i_i16[8];
    *marked_numbers |= 1 << mark_index;

    u32 marked = *marked_numbers;
    bool has_bingo = false;
    has_bingo |= CHECK_BINGO(marked, 0b11111'00000'00000'00000'00000'0);
    has_bingo |= CHECK_BINGO(marked, 0b00000'11111'00000'00000'00000'0);
    has_bingo |= CHECK_BINGO(marked, 0b00000'00000'11111'00000'00000'0);
    has_bingo |= CHECK_BINGO(marked, 0b00000'00000'00000'11111'00000'0);
    has_bingo |= CHECK_BINGO(marked, 0b00000'00000'00000'00000'11111'0);
    has_bingo |= CHECK_BINGO(marked, 0b10000'10000'10000'10000'10000'0);
    has_bingo |= CHECK_BINGO(marked, 0b01000'01000'01000'01000'01000'0);
    has_bingo |= CHECK_BINGO(marked, 0b00100'00100'00100'00100'00100'0);
    has_bingo |= CHECK_BINGO(marked, 0b00010'00010'00010'00010'00010'0);
    has_bingo |= CHECK_BINGO(marked, 0b00001'00001'00001'00001'00001'0);
    return has_bingo;
}

void day4_solve_part1(const Day4Input *input, Day4Output *output)
{
    u32 *marked_numbers = new u32[input->board_count];
    memset(marked_numbers, 0, sizeof(u32) * input->board_count);

    __m256i zero = _mm256_setzero_si256();
    __m256i index_mask = _mm256_set_epi8(0, 0, 0, 0, 0, 0, 0, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1);

    u32 draw_index = 0;
    while(draw_index < input->draw_count)
    {
        u8 draw = input->numbers_to_draw[draw_index];
        __m256i draw_vector = _mm256_set1_epi8(draw);

        for(u32 i = 0; i < input->board_count; ++i)
        {
            bool has_bingo = mark_board_and_check_bingo(input->boards[i], &marked_numbers[i], draw_vector, zero, index_mask);
            if(has_bingo)
            {
                u32 mask = 0b10;
                u32 unmarked_sum = 0;
                u32 marked = marked_numbers[i];
                for(u32 j = 0; j < 25; ++j)
                {
                    unmarked_sum += input->boards[i].numbers[j] * ((marked & mask) == 0);
                    mask <<= 1;
                }

                output->last_draw = draw;
                output->unmarked_sum = unmarked_sum;
                output->product = draw * unmarked_sum;

                delete[] marked_numbers;

                return;
            }
        }

        ++draw_index;
    }

    delete[] marked_numbers;
}

void day4_solve_part2(const Day4Input *input, Day4Output *output)
{
    u32 *marked_numbers = new u32[input->board_count];
    u32 win_array_size = (input->board_count + 63) / 64;
    u64 *not_won_bits = new u64[(input->board_count + 63) / 64]; // Contains a 1 if the corresponding board has not gotten a bingo yet
    memset(marked_numbers, 0, sizeof(u32) * input->board_count);
    memset(not_won_bits, U64_MAX, sizeof(u64) * win_array_size);

    u32 not_won_boards = input->board_count;

    __m256i zero = _mm256_setzero_si256();
    __m256i index_mask = _mm256_set_epi8(0, 0, 0, 0, 0, 0, 0, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1);

    u32 draw_index = 0;
    u32 win_array_index = 0;
    u8 draw;
    while(not_won_boards > 1)
    {
        draw = input->numbers_to_draw[draw_index];
        __m256i draw_vector = _mm256_set1_epi8(draw);
        
        unsigned long board_index = -1;
        for(win_array_index = 0; win_array_index < win_array_size; ++win_array_index)
            if(not_won_bits[win_array_index])
            {
                _BitScanForward64(&board_index, not_won_bits[win_array_index]);
                board_index |= win_array_index * 64;
                break;
            }
        
        while(board_index < input->board_count)
        {
            u64 board_win_mask = 1ull << (board_index % 64);

            bool has_bingo = mark_board_and_check_bingo(input->boards[board_index], &marked_numbers[board_index], draw_vector, zero, index_mask);
            if(has_bingo)
            {
                not_won_bits[board_index / 64] &= ~board_win_mask;
                --not_won_boards;
            }

            board_win_mask |= board_win_mask - 1;
            u64 win_bits_above = not_won_bits[win_array_index] & (~board_win_mask);
            if(win_bits_above)
            {
                _BitScanForward64(&board_index, win_bits_above);
                board_index |= win_array_index * 64;
            }
            else
            {
                ++win_array_index;
                for(; win_array_index < win_array_size; ++win_array_index)
                {
                    if(not_won_bits[win_array_index])
                    {
                        _BitScanForward64(&board_index, not_won_bits[win_array_index]);
                        board_index |= win_array_index * 64;
                        break;
                    }
                }
            }
        }

        ++draw_index;
    }

    unsigned long worst_board_index = -1;
    for(win_array_index = 0; win_array_index < win_array_size; ++win_array_index)
        if(not_won_bits[win_array_index])
        {
            _BitScanForward64(&worst_board_index, not_won_bits[win_array_index]);
            worst_board_index |= win_array_index * 64;
            break;
        }
    
    Board &worst_board = input->boards[worst_board_index];
    u32 *worst_marked_numbers = &marked_numbers[worst_board_index];
    while(draw_index < input->draw_count)
    {
        draw = input->numbers_to_draw[draw_index];
        __m256i draw_vector = _mm256_set1_epi8(draw);

        bool has_bingo = mark_board_and_check_bingo(worst_board, worst_marked_numbers, draw_vector, zero, index_mask);
        if(has_bingo)
            break;
        
        ++draw_index;
    }

    u32 mask = 0b10;
    u32 unmarked_sum = 0;
    u32 marked = *worst_marked_numbers;
    for(u32 j = 0; j < 25; ++j)
    {
        unmarked_sum += worst_board.numbers[j] * ((marked & mask) == 0);
        mask <<= 1;
    }

    output->worst_unmarked_sum = unmarked_sum;
    output->worst_last_draw = draw;
    output->worst_product = draw * unmarked_sum;

    delete[] marked_numbers;
    delete[] not_won_bits;
}

void day4_print_answer(const Day4Output *output)
{
    printf("Sum of unmarked numbers: %d\n", output->unmarked_sum);
    printf("Last number drawn:       %d\n", output->last_draw);
    printf("Product:                 %d\n\n", output->product);

    printf("Worst sum:       %d\n", output->worst_unmarked_sum);
    printf("Worst last draw: %d\n", output->worst_last_draw);
    printf("Worst product:   %d\n\n", output->worst_product);
}

void day4_free(const Day4Input *input, const Day4Output *output)
{
    std::free(input->numbers_to_draw);
    std::free(input->boards);
    delete input;
    delete output;
}
