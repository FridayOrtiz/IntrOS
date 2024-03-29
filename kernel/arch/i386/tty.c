#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>

#include "vga.h"

// size_t strlen(const char* str)
// {
//     size_t len = 0;
//     while ( str[len] )
//     {
//         len++;
//     }
//     return len;
// }

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void terminal_initialize(void)
{
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminal_buffer = VGA_MEMORY;
    for (size_t y = 0; y < VGA_HEIGHT; y++)
    {
        for (size_t x = 0; x < VGA_WIDTH; x++)
        {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

void terminal_setcolor(uint8_t color)
{
    terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}


void terminal_scroll(void)
{
    // Move rows up
    for (size_t row = 0; row < VGA_HEIGHT - 1; row++)
    {
        for (size_t col = 0; col < VGA_WIDTH; col++)
        {
            const size_t first_index = row * VGA_WIDTH + col;
            const size_t second_index = (row + 1) * VGA_WIDTH + col;

            terminal_buffer[first_index] = terminal_buffer[second_index];
        }
    }

    // Clear bottom row
    for (size_t col = 0; col < VGA_WIDTH; col++ )
    {
        const size_t index = (VGA_HEIGHT - 1) * VGA_WIDTH + col;
        terminal_buffer[index] = vga_entry(' ', terminal_color);
    }
}

void terminal_putchar(char c)
{
    unsigned char uc = c;
    bool newline = uc == '\n';

    if (!newline)
    {
        terminal_putentryat(uc, terminal_color, terminal_column, terminal_row);
    }
    if (++terminal_column == VGA_WIDTH || newline)
    {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT)
        {
            terminal_row = VGA_HEIGHT - 2;
            terminal_scroll();
        }
    }       
}

void terminal_write(const char* data, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        terminal_putchar(data[i]);
    }
}

void terminal_writestring(const char* data)
{
    terminal_write(data, strlen(data));
}