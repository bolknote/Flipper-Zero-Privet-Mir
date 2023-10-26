#pragma once

#include <gui/gui.h>
#include <gui/canvas_i.h>
#include <u8g2/u8g2.h>

typedef struct {
    Gui* gui;
    ViewPort* view_port;
    FuriMessageQueue* event_queue;
} UnicodeApp;

int8_t unicode_GetGlyphWidth(u8g2_t* u8g2, uint16_t requested_encoding);
void unicode_app_free(UnicodeApp** app);
void unicode_render_callback(Canvas* canvas, void* ctx);
UnicodeApp* unicode_app_alloc();
void unicode_wait_a_key(UnicodeApp* app);
