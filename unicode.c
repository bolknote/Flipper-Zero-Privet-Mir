#include "helpers/unicode.h"
#include <u8g2/u8g2_fonts.c>

void unicode_draw_utf8_str(Canvas* canvas, uint8_t x, uint8_t y, char* str) {
    FuriStringUTF8State state = FuriStringUTF8StateStarting;
    FuriStringUnicodeValue value = 0;

    for(; *str; str++) {
        furi_string_utf8_decode(*str, &state, &value);
        if(state == FuriStringUTF8StateError) furi_crash(NULL);

        if(state == FuriStringUTF8StateStarting) {
            canvas_draw_glyph(canvas, x, y, value);
            x += unicode_GetGlyphWidth(&canvas->fb, value);
        }
    }
}

void unicode_render_callback(Canvas* canvas, void* ctx) {
    UNUSED(ctx);

    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    canvas_set_custom_u8g2_font(canvas, u8g2_font_haxrcorp4089_t_cyrillic);

    uint8_t h = canvas_current_font_height(canvas);
    unicode_draw_utf8_str(canvas, 0, h, "Привет, мир!");
    unicode_draw_utf8_str(canvas, 0, 2 * h, "Пишем по-русски");
    unicode_draw_utf8_str(canvas, 0, 3 * h, "на Флиппере Зеро.");

    canvas_set_custom_u8g2_font(canvas, u8g2_font_5x7_t_cyrillic);
    unicode_draw_utf8_str(canvas, 0, 5 * h, "Евгений Степанищев, 2023");
}

int32_t unicode_main(void* p) {
    UNUSED(p);
    __attribute__((__cleanup__(unicode_app_free))) UnicodeApp* app = unicode_app_alloc();
    unicode_wait_a_key(app);
    return 0;
}
