/*
** Renegade watch face - Color or Mono
**
** Acknowledgements
**   * This is only a slightly modified version of Pebble Watchface tutorial
**       * http://developer.getpebble.com/getting-started/watchface-tutorial/part2/
**   * Font ISL Jupiter by Isurus Labs
**       * http://www.dafont.com/isl-jupiter.font
**   * Logo Vector by Karlika
**       * http://karlika.deviantart.com/art/Mass-Effect-Vector-Pack-178422616
*/

#include <pebble.h>

#define FONT_NAME RESOURCE_ID_FONT_JUPITER_60

//#define CLOCK_POS GRect(5, 52, 139, 50)
#define CLOCK_POS GRect(0, 52, 144, 168) /* probably taller than really needed */

/* PebbleKit JS, Message Keys, Pebble config keys */
// FIXME why can't this be generated from the json settings file into a header?
#define KEY_TIME_COLOR 0


static Window    *s_main_window;
static TextLayer *s_time_layer;

static GFont       s_time_font;
static BitmapLayer *s_background_layer;
static GBitmap     *s_background_bitmap_renegade;
static GBitmap     *s_background_bitmap_paragon;

static uint32_t bg_image=RESOURCE_ID_IMAGE_RENEGADE;  // or RESOURCE_ID_IMAGE_PARAGON

/* For colors, see http://developer.getpebble.com/tools/color-picker/#0000FF */
static GColor       time_color;
static int          config_time_color;


static void in_recv_handler(DictionaryIterator *iterator, void *context)
{
    Tuple *t=NULL;

    APP_LOG(APP_LOG_LEVEL_DEBUG, "in_recv_handler() called");
    t = dict_read_first(iterator);
    while(t != NULL)
    {
        switch(t->key)
        {
            case KEY_TIME_COLOR:
                APP_LOG(APP_LOG_LEVEL_DEBUG, "got KEY_TIME_COLOR");
                config_time_color = (int)t->value->int32;
                APP_LOG(APP_LOG_LEVEL_DEBUG, "Persisting time color: 0x%06x", config_time_color);
                persist_write_int(KEY_TIME_COLOR, config_time_color);
                time_color = COLOR_FALLBACK(GColorFromHEX(config_time_color), GColorWhite);
                text_layer_set_text_color(s_time_layer, time_color);
                break;
            default:
                APP_LOG(APP_LOG_LEVEL_ERROR, "Unknown key %d! :-(", (int) t->key);
                break;
        }
        t = dict_read_next(iterator);
    }
}

static void update_time() {
    // Get a tm structure
    time_t    temp = time(NULL);
    struct tm *tick_time = localtime(&temp);

    // Create a long-lived buffer
    static char buffer[] = "00:00";

    // Write the current hours and minutes into the buffer
    if(clock_is_24h_style() == true) {
        // 2h hour format
        strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
    } else {
        // 12 hour format
        strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
    }

    // Display this time on the TextLayer
    text_layer_set_text(s_time_layer, buffer);
}


static void main_window_load(Window *window) {
    // Create GBitmap, then set to created BitmapLayer
    s_background_bitmap_renegade = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_RENEGADE);
    s_background_bitmap_paragon = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_PARAGON);
    
    s_background_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
    if (bg_image == RESOURCE_ID_IMAGE_RENEGADE)
    {
        bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap_renegade);
    }
    else
    {
        bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap_paragon);
    }
    layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layer));

    // Create time TextLayer
    s_time_layer = text_layer_create(CLOCK_POS);
    text_layer_set_background_color(s_time_layer, GColorClear);
    text_layer_set_text_color(s_time_layer, time_color);
    text_layer_set_text(s_time_layer, "00:00");

    // Create GFont
    s_time_font = fonts_load_custom_font(resource_get_handle(FONT_NAME));

    // Apply to TextLayer
    text_layer_set_font(s_time_layer, s_time_font);
    text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

    // Add it as a child layer to the Window's root layer
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));

    // Make sure the time is displayed from the start
    update_time();
}

static void main_window_unload(Window *window) {
    //Unload GFont
    fonts_unload_custom_font(s_time_font);

    //Destroy GBitmap
    gbitmap_destroy(s_background_bitmap_renegade);
    gbitmap_destroy(s_background_bitmap_paragon);

    //Destroy BitmapLayer
    bitmap_layer_destroy(s_background_layer);

    // Destroy TextLayer
    text_layer_destroy(s_time_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
    /* Test, this changes the image once a minute */
    bg_image = bg_image == RESOURCE_ID_IMAGE_RENEGADE ? RESOURCE_ID_IMAGE_PARAGON : RESOURCE_ID_IMAGE_RENEGADE;
    if (bg_image == RESOURCE_ID_IMAGE_RENEGADE)
    {
        bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap_renegade);
    }
    else
    {
        bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap_paragon);
    }

    update_time();
}

static void init() {
    time_color = GColorWhite;

#ifdef PBL_PLATFORM_BASALT
    /* TODO refactor */
    if (persist_exists(KEY_TIME_COLOR))
    {
        config_time_color = persist_read_int(KEY_TIME_COLOR);
        APP_LOG(APP_LOG_LEVEL_INFO, "Read time color: %x", config_time_color);
        time_color = COLOR_FALLBACK(GColorFromHEX(config_time_color), GColorWhite);
    }
#endif /* PBL_PLATFORM_BASALT */

    // Create main Window element and assign to pointer
    s_main_window = window_create();
    window_set_background_color(s_main_window, GColorBlack); // TODO move me into handler?

    // Set handlers to manage the elements inside the Window
    window_set_window_handlers(s_main_window, (WindowHandlers) {
                                   .load = main_window_load,
                                   .unload = main_window_unload
                               });

    // Show the Window on the watch, with animated=true
    window_stack_push(s_main_window, true);

    // Register with TickTimerService
    tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

    /* Register config event */
    /* TODO use AppSync instead? */
    app_message_register_inbox_received(in_recv_handler);
    app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum()); 

}

static void deinit() {
    // Destroy Window
    window_destroy(s_main_window);
}

int main(void) {
    init();
    app_event_loop();
    deinit();
}
