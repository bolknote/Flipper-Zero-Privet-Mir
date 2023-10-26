#pragma once

#include <gui/gui.h>
#include <gui/canvas_i.h>
#include <u8g2/u8g2.h>
#include <u8g2/u8g2_fonts.c>

int8_t unicode_GetGlyphWidth(u8g2_t* u8g2, uint16_t requested_encoding);

typedef struct {
    Gui* gui;
    ViewPort* view_port;
} UnicodeApp;
