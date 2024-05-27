#include "buffer.h"

#include "array.h"
#include "charbuffer.h"

Buffer* buffer_create (char* file) {
    Buffer* buffer = malloc(sizeof(Buffer));

    buffer->lines = array_create();

    buffer->cursor = buffer->selection = (Point) {0,0};

    buffer->filename = file;

    buffer->tab_width = 4;
    buffer->hard_tabs = false;

    // Load contents.
    FILE* f = fopen(file, "r");
    for (;;) {
        CharBuffer* line = charbuffer_create();
        bool b = charbuffer_read_line(line, f);
        array_add (buffer->lines, line);
        if (!b) break;
    }
    fclose(f);

    return buffer;
}

void buffer_destroy (Buffer* buffer) {
    for (int i = 0; i < buffer->lines->size; ++i) {
        charbuffer_destroy(buffer->lines->data[i]);
    }
    array_destroy(buffer->lines);
    free(buffer);
}

void buffer_save (Buffer* buffer) {

}

void buffer_draw (Buffer* buffer, Box window) {
    for (int i = 0; i < window.height; ++i) {
        output_cup(window.y + i, window.x);
        CharBuffer* line = array_get(buffer->lines, buffer->view_line + i);
        for (int j = 0; j < window.width; ++j) {
            if (line != NULL && j < line->size && line->buffer[j] > 32) {
                output_char(line->buffer[j]);
            } else {
                output_char(' ');
            }
        }
    }

    int cx = buffer->cursor.col;
    int cy = buffer->cursor.line - buffer->view_line;
    if (cx >= window.width || cy >= window.height) {
        output_civis();
    } else {
        output_cvvis();
        output_cup(window.y + cy, window.x + cx);
    }

}


void buffer_edit_char (Buffer* buffer, uint32_t ch, int32_t i) {
    while (i-- > 0) {
        CharBuffer* line = buffer->lines->data[buffer->cursor.line];
        charbuffer_ichar(line, (char) ch, buffer->cursor.col);
        buffer->cursor.col++;
    }
}

void buffer_edit_text (Buffer* buffer, CharBuffer* text, int32_t i) {

    // ...
}

void buffer_edit_line (Buffer* buffer, int32_t i) {
    while (i-- > 0) {
        CharBuffer* line = buffer->lines->data[buffer->cursor.line];
        CharBuffer* newline = charbuffer_create();
        charbuffer_get_suffix(line, newline, buffer->cursor.col);
        charbuffer_rm_suffix(line, buffer->cursor.col);
        array_insert(buffer->lines, buffer->cursor.line + 1, newline);
        buffer->cursor.line++;
        buffer->cursor.col = 0;
    }
}

void buffer_edit_tab (Buffer* buffer, int32_t i) {

    // ...
}

void buffer_edit_indent (Buffer* buffer, int32_t i) {

    // ...
}

void buffer_edit_delete (Buffer* buffer, int32_t i) {
    while (i-- > 0) {
        CharBuffer* line = buffer->lines->data[buffer->cursor.line];
        if (buffer->cursor.col >= line->size) {
            if (buffer->cursor.line < buffer->lines->size - 1) {
                CharBuffer* nextline = array_remove(buffer->lines, buffer->cursor.line + 1);
                charbuffer_achars(line, nextline);
                charbuffer_destroy(nextline);
            }
        } else {
            charbuffer_rm_char(line, buffer->cursor.col);
        }
    }
}

void buffer_edit_backspace (Buffer* buffer, int32_t i) {
    while (i-- > 0) {
        CharBuffer* line = buffer->lines->data[buffer->cursor.line];
        if (buffer->cursor.col <= 0) {
            if (buffer->cursor.line > 0) {
                CharBuffer* prevline = buffer->lines->data[buffer->cursor.line - 1];
                buffer->cursor.col = prevline->size;
                array_remove(buffer->lines, buffer->cursor.line);
                charbuffer_achars(prevline, line);
                charbuffer_destroy(line);
                buffer->cursor.line--;
            }
        } else {
            charbuffer_rm_char(line, buffer->cursor.col - 1);
            buffer->cursor.col--;
        }
    }
}



void buffer_cursor_goto (Buffer* buffer, int32_t row, int32_t col, bool sel) {
    if (row >= buffer->lines->size) row = buffer->lines->size - 1;

    CharBuffer* line = buffer->lines->data[row];
    if (col > line->size) col = line->size;

    buffer->cursor.line = row;
    buffer->cursor.col = col;

    if (!sel) {
        buffer->selection = buffer->cursor;
    }
}

void buffer_cursor_line (Buffer* buffer, int32_t i, bool sel) {
    buffer->cursor.line += i;
    if (buffer->cursor.line < 0) buffer->cursor.line = 0;
    if (buffer->cursor.line >= buffer->lines->size) buffer->cursor.line = buffer->lines->size - 1;

    // Line Wrapping logic goes here...
    CharBuffer* line = buffer->lines->data[buffer->cursor.line];
    if (buffer->cursor.col > line->size) buffer->cursor.col = line->size;

    if (!sel) {
        buffer->selection = buffer->cursor;
    }
}

void buffer_cursor_char (Buffer* buffer, int32_t i, bool sel) {
    int d = i > 0 ? 1 : -1;
    if (i < 0) i *= -1;

    while (i-- > 0) {
        int c = (int) buffer->cursor.col + d;

        if (c < 0) {
            if (buffer->cursor.line <= 0) {
                break;
            }
            buffer->cursor.line--;
            CharBuffer* line = buffer->lines->data[buffer->cursor.line];
            buffer->cursor.col = line->size;
            continue;
        }

        CharBuffer* line = buffer->lines->data[buffer->cursor.line];
        if (c > line->size) {
            if (buffer->cursor.line >= buffer->lines->size - 1) {
                break;
            }
            buffer->cursor.line++;
            buffer->cursor.col = 0;
            continue;
        }

        buffer->cursor.col = c;
    }

    if (!sel) {
        buffer->selection = buffer->cursor;
    }
}

void buffer_cursor_word (Buffer* buffer, int32_t i, bool sel) {

    // ...
}

void buffer_cursor_home (Buffer* buffer, bool sel) {
    buffer->cursor.line = 0;
    buffer->cursor.col = 0;

    if (!sel) {
        buffer->selection = buffer->cursor;
    }
}

void buffer_cursor_end (Buffer* buffer, bool sel) {
    buffer->cursor.line = buffer->lines->size - 1;
    CharBuffer* line = buffer->lines->data[buffer->cursor.line];
    buffer->cursor.col = line->size;

    if (!sel) {
        buffer->selection = buffer->cursor;
    }
}

void buffer_cursor_line_begin (Buffer* buffer, bool sel) {
    buffer->cursor.col = 0;

    if (!sel) {
        buffer->selection = buffer->cursor;
    }
}

void buffer_cursor_line_end (Buffer* buffer, bool sel) {
    CharBuffer* line = buffer->lines->data[buffer->cursor.line];
    buffer->cursor.col = line->size;

    if (!sel) {
        buffer->selection = buffer->cursor;
    }
}
