#include <cstdlib>
#include <cstdio>
#include <intrin.h>
#include <algorithm>

#include "common.hpp"

#define MAX_BIT_COUNT 16
#define BIT_COUNT 12

struct ReportEntry
{
    u16 bits[MAX_BIT_COUNT];
};

struct Day3Input
{
    ReportEntry *entries;
    u16 *packed_entries;
    u32 entry_count;  
    u32 max_entry_count;
    
    void add_entry(__m256i bits)
    {
        u16 packed_bits =
            (bits.m256i_i16[0] << 11) |
            (bits.m256i_i16[1] << 10) |
            (bits.m256i_i16[2] << 9) |
            (bits.m256i_i16[3] << 8) |
            (bits.m256i_i16[4] << 7) |
            (bits.m256i_i16[5] << 6) |
            (bits.m256i_i16[6] << 5) |
            (bits.m256i_i16[7] << 4) |
            (bits.m256i_i16[8] << 3) |
            (bits.m256i_i16[9] << 2) |
            (bits.m256i_i16[10] << 1) |
            (bits.m256i_i16[11]);

        packed_entries[entry_count] = packed_bits;
        _mm256_storeu_epi16(&entries[entry_count].bits, bits);
        entry_count++;
        if(entry_count >= max_entry_count)
        {
            max_entry_count *= 2;
            entries = (ReportEntry*)std::realloc(entries, sizeof(ReportEntry) * max_entry_count);
            packed_entries = (u16*)std::realloc(packed_entries, sizeof(u16) * max_entry_count);
        }
    }
};

struct Day3Output
{
    u32 gamma_rate;
    u32 epsilon_rate;
    u32 power_consumption;

    u32 oxygen_generator_rating;
    u32 co2_scrubber_rating;
    u32 life_support_rating;
};

const Day3Input* day3_parse(const char *filename)
{
    Day3Input input;

    input.entry_count = 0;
    input.max_entry_count = 1;
    input.entries = (ReportEntry*)std::malloc(sizeof(ReportEntry) * input.max_entry_count);
    input.packed_entries = (u16*)std::malloc(sizeof(u16) * input.max_entry_count);

    #if _WIN32
    {
        HANDLE file = CreateFileA(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        HANDLE mapping = CreateFileMapping(file, NULL, PAGE_READONLY | SEC_COMMIT, 0, 0, NULL);
        const char *contents = (const char*)MapViewOfFile(mapping, FILE_MAP_READ, 0, 0, 0);
        DWORD size = GetFileSize(file, NULL);

        __m128i char_zero = _mm_set1_epi8('0');
        for(u32 i = 0; i < size; i += 13)
        {
            __m128i bit_chars = _mm_loadu_epi8(&contents[i]);
            bit_chars = _mm_sub_epi8(bit_chars, char_zero);
            __m256i bits = _mm256_cvtepu8_epi16(bit_chars);
            input.add_entry(bits);
        }

        UnmapViewOfFile(contents);
        CloseHandle(mapping);
        CloseHandle(file);
    }
    #endif

    return new Day3Input(input);
}

Day3Output* day3_alloc_output()
{
    return new Day3Output;
}

void day3_solve_part1(const Day3Input *input, Day3Output *output)
{
    __m256i sum_bits = _mm256_setzero_si256();
    for(u32 i = 0; i < input->entry_count; ++i)
    {
        __m256i bits = _mm256_loadu_epi16(&input->entries[i].bits);
        sum_bits = _mm256_add_epi16(sum_bits, bits);
    }
    __m256i half_count = _mm256_set1_epi16((u16)input->entry_count / 2 - 1);
    __m256i comparison = _mm256_cmpgt_epi16(sum_bits, half_count);
    comparison = _mm256_abs_epi16(comparison);
    u32 gamma_rate =
        (comparison.m256i_i16[0] << 11) |
        (comparison.m256i_i16[1] << 10) |
        (comparison.m256i_i16[2] << 9) |
        (comparison.m256i_i16[3] << 8) |
        (comparison.m256i_i16[4] << 7) |
        (comparison.m256i_i16[5] << 6) |
        (comparison.m256i_i16[6] << 5) |
        (comparison.m256i_i16[7] << 4) |
        (comparison.m256i_i16[8] << 3) |
        (comparison.m256i_i16[9] << 2) |
        (comparison.m256i_i16[10] << 1) |
        (comparison.m256i_i16[11]);
    
    u32 epsilon_rate = 0b111111111111 ^ gamma_rate;

    output->gamma_rate = gamma_rate;
    output->epsilon_rate = epsilon_rate;
    output->power_consumption = gamma_rate * epsilon_rate;
}

void sort_bit(u16 *array, u32 start, u32 end, u32 mask)
{
    u32 left = start;
    u32 right = end - 1;

    while(left < right)
    {
        if(!(array[left] & mask))
            left++;
        else if(array[right] & mask)
            right--;
        else
        {
            std::swap(array[left], array[right]);
            left++;
            right--;
        }
    }
}

void day3_solve_part2(const Day3Input *input, Day3Output *output)
{
    u16 *entries = new u16[input->entry_count];
    
    memcpy(entries, input->packed_entries, sizeof(u16) * input->entry_count);
    u32 start = 0, end = input->entry_count;
    u32 mask = 0b100000000000;
    for(u32 i = 0; i < 12 && end - start > 1; ++i)
    {
        sort_bit(entries, start, end, mask);
        
        u32 middle = (start + end) / 2;
        if(entries[middle] & mask)
        {
            s32 j;
            for(j = middle - 1; (entries[j] & mask) && j >= start; --j);
            start = j + 1;
        }
        else
        {
            s32 j;
            for(j = middle + 1; !(entries[j] & mask) && j < end; ++j);
            end = j;
        }
        mask >>= 1;
    }

    u32 oxygen_generator_rating = entries[start];
    
    start = 0, end = input->entry_count;
    mask = 0b100000000000;
    for(u32 i = 0; i < 12 && end - start > 1; ++i)
    {
        sort_bit(entries, start, end, mask);
        
        u32 middle = (start + end) / 2;
        if(entries[middle] & mask)
        {
            s32 j;
            for(j = middle - 1; (entries[j] & mask) && j >= start; --j);

            end = j + 1;
        }
        else
        {
            s32 j;
            for(j = middle + 1; !(entries[j] & mask) && j < end; ++j);

            start = j;
        }
        mask >>= 1;
    }

    u32 co2_scrubber_rating = entries[start];

    delete[] entries;

    output->oxygen_generator_rating = oxygen_generator_rating;
    output->co2_scrubber_rating = co2_scrubber_rating;
    output->life_support_rating = oxygen_generator_rating * co2_scrubber_rating;
}

void day3_print_answer(const Day3Output *output)
{
    printf("Gamma rate:        %d\n", output->gamma_rate);
    printf("Epsilon rate:      %d\n", output->epsilon_rate);
    printf("Power consumption: %d\n\n", output->power_consumption);

    printf("Oxygen generator rating: %d\n", output->oxygen_generator_rating);
    printf("CO2 scrubber rating:     %d\n", output->co2_scrubber_rating);
    printf("Life support rating:     %d\n\n", output->life_support_rating);
}

void day3_free(const Day3Input *input, const Day3Output *output)
{
    std::free(input->entries);
    delete input;
    delete output;
}
