#include "unicode.h"

void unicode_input_callback(InputEvent* input_event, void* ctx) {
    furi_assert(ctx);

    FuriMessageQueue* event_queue = ctx;
    furi_message_queue_put(event_queue, input_event, FuriWaitForever);
}

UnicodeApp* unicode_app_alloc() {
    UnicodeApp* app = malloc(sizeof(UnicodeApp));

    app->view_port = view_port_alloc();
    view_port_draw_callback_set(app->view_port, unicode_render_callback, app);
    app->gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(app->gui, app->view_port, GuiLayerFullscreen);

    app->event_queue = furi_message_queue_alloc(1, sizeof(InputEvent));
    view_port_input_callback_set(app->view_port, unicode_input_callback, app->event_queue);

    return app;
}

void unicode_app_free(UnicodeApp** app) {
    furi_assert(*app);

    view_port_enabled_set((*app)->view_port, false);
    gui_remove_view_port((*app)->gui, (*app)->view_port);
    view_port_free((*app)->view_port);

    furi_record_close(RECORD_GUI);
    furi_message_queue_free((*app)->event_queue);

    free(*app);
}

void unicode_wait_a_key(UnicodeApp* app) {
    for(InputEvent event;;) {
        if(furi_message_queue_get(app->event_queue, &event, 0) == FuriStatusOk) {
            if(event.type == InputTypePress) break;
        }
    }
}
