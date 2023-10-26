#include "unicode.h"

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

static void unicode_input_callback(InputEvent* input_event, void* ctx) {
    furi_assert(ctx);

    FuriMessageQueue* event_queue = ctx;
    furi_message_queue_put(event_queue, input_event, FuriWaitForever);
}

int32_t unicode_main(void* p) {
    UNUSED(p);

    ViewPort* view_port = view_port_alloc();
    view_port_draw_callback_set(view_port, unicode_render_callback, NULL);
    Gui* gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(gui, view_port, GuiLayerFullscreen);

    FuriMessageQueue* event_queue = furi_message_queue_alloc(1, sizeof(InputEvent));
    view_port_input_callback_set(view_port, unicode_input_callback, event_queue);

    for(InputEvent event;;) {
        if(furi_message_queue_get(event_queue, &event, 0) == FuriStatusOk) {
            if(event.type == InputTypePress) break;
        }
    }

    view_port_enabled_set(view_port, false);
    gui_remove_view_port(gui, view_port);
    view_port_free(view_port);
    furi_message_queue_free(event_queue);

    furi_record_close(RECORD_GUI);

    return 0;
}
