#pragma once

#include "main.h"


struct point {
    int32_t line, col;
};

struct buffer {
    Array* lines;

    Point cursor;
    Point selection;

    int32_t col_mem;

    bool scroll_damage;
    int32_t scroll_line;
    int32_t scroll_offset;
    int32_t scroll_pages;

    char* title;
    char* filename;

    bool alt_mode;
    bool page_mode;
    bool block_mode;
    bool raise_mode;

    int32_t tab_width;
    bool hard_tabs;

    // Undo/Redo Stack.
    // ...

    // Buffer-Specific Clipboard(s).
    // ...
};


Buffer* buffer_create (const char* title);

void buffer_destroy (Buffer* buffer);

void buffer_load (Buffer* buffer, const char* file);

bool buffer_save (Buffer* buffer);

void buffer_draw (Buffer* buffer, Box window, uint32_t mstate, uint32_t mx, uint32_t my);


void buffer_edit_char (Buffer* buffer, uint32_t ch, int32_t i);

void buffer_edit_text (Buffer* buffer, CharBuffer* text, int32_t i);

void buffer_edit_line (Buffer* buffer, int32_t i);

void buffer_edit_tab (Buffer* buffer, int32_t i);

void buffer_edit_indent (Buffer* buffer, int32_t i);

void buffer_edit_delete (Buffer* buffer, int32_t i);

void buffer_edit_backspace (Buffer* buffer, int32_t i);

void buffer_edit_move_line (Buffer* buffer, int32_t i);


void buffer_cursor_goto (Buffer* buffer, int32_t row, int32_t col, bool sel);

void buffer_cursor_line (Buffer* buffer, int32_t i, bool sel);

void buffer_cursor_char (Buffer* buffer, int32_t i, bool sel);

void buffer_cursor_word (Buffer* buffer, int32_t lead, int32_t i, bool sel);

void buffer_cursor_paragraph (Buffer* buffer, int32_t i, bool sel);

void buffer_cursor_home (Buffer* buffer, bool sel);

void buffer_cursor_end (Buffer* buffer, bool sel);

void buffer_cursor_line_begin (Buffer* buffer, bool sel);

void buffer_cursor_line_end (Buffer* buffer, bool sel);


void buffer_select_all (Buffer* buffer);

void buffer_select_word (Buffer* buffer);

void buffer_select_line (Buffer* buffer);


bool buffer_selection_exist (Buffer* buffer);

void buffer_selection_swap (Buffer* buffer);

void buffer_selection_clear (Buffer* buffer);

void buffer_selection_cut (Buffer* buffer, CharBuffer* dst);

void buffer_selection_copy (Buffer* buffer, CharBuffer* dst);

void buffer_selection_duplicate (Buffer* buffer, int32_t i);

void buffer_selection_delete_whitespace (Buffer* buffer);


void buffer_undo (Buffer* buffer, int32_t i);

void buffer_redo (Buffer* buffer, int32_t i);
