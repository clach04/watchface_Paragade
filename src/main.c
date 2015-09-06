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

#include "watchface.h"

#ifdef CUSTOM    
static uint32_t bg_image=RESOURCE_ID_IMAGE_RENEGADE;  // or RESOURCE_ID_IMAGE_PARAGON


static void main_window_load(Window *window) {
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);

    // Create GBitmap, then set to created BitmapLayer
    s_background_bitmap_renegade = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_RENEGADE);
    s_background_bitmap_paragon = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_PARAGON);
    
    s_background_layer = bitmap_layer_create(bounds);
    if (bg_image == RESOURCE_ID_IMAGE_RENEGADE)
    {
        bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap_renegade);
    }
    else
    {
        bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap_paragon);
    }
#ifdef PBL_PLATFORM_APLITE
     bitmap_layer_set_compositing_mode(s_background_layer, GCompOpAssign);
#elif PBL_PLATFORM_BASALT
     bitmap_layer_set_compositing_mode(s_background_layer, GCompOpSet);
#endif

    window_set_background_color(s_main_window, background_color);

    layer_add_child(window_layer, bitmap_layer_get_layer(s_background_layer));

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


/*
int main(void) {
    init();
    app_event_loop();
    deinit();
}
*/
#endif /* CUSTOM */
