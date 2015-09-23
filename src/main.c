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

#include "effect_layer.h"


static BitmapLayer *s_background_layer=NULL;  /* NOTE s_bg_image is NOT defined */

static uint32_t s_bg_image=RESOURCE_ID_IMAGE_RENEGADE;  // or RESOURCE_ID_IMAGE_PARAGON
static GBitmap     *s_background_bitmap_renegade=NULL;
static GBitmap     *s_background_bitmap_paragon=NULL;

static EffectLayer* effect_layer=NULL;
static EffectOffset effect_offset;

void setup_effects(Window *window)
{
    //effect_layer = effect_layer_create(GRect(0, 0, 144, 168)); // will cover entire screen
    effect_layer = effect_layer_create(CLOCK_POS);

    effect_offset.orig_color = time_color;
    effect_offset.offset_color = background_color;
    effect_offset.offset_y = 2;
    effect_offset.offset_x = 2;

    //effect_layer_add_effect(effect_layer, effect_shadow, &effect_offset);
    effect_layer_add_effect(effect_layer, effect_outline, &effect_offset);
    //effect_layer_add_effect(effect_layer, effect_invert, NULL);
    //effect_layer_add_effect(effect_layer, effect_invert_bw_only, NULL);
    //effect_layer_add_effect(effect_layer, effect_rotate_90_degrees, (void *)true); // rotates 90 degrees counterclockwise

    layer_add_child(window_get_root_layer(window), effect_layer_get_layer(effect_layer));
}

void cleanup_effects()
{
    if (effect_layer)
    {
        effect_layer_destroy(effect_layer);
    }
}

void custom_main_window_load(Window *window) {
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);

    // Create GBitmap, then set to created BitmapLayer
    s_background_bitmap_renegade = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_RENEGADE);
    s_background_bitmap_paragon = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_PARAGON);
    
    s_background_layer = bitmap_layer_create(bounds);
    if (s_bg_image == RESOURCE_ID_IMAGE_RENEGADE)
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

    window_set_background_color(main_window, background_color);

    layer_add_child(window_layer, bitmap_layer_get_layer(s_background_layer));

    /* End of custom code, call generic code */
    main_window_load(window);

    /* Custom code */
    setup_effects(window); // move me?
}

void custom_main_window_unload(Window *window) {
    cleanup_effects();

    /* Call generic code */
    main_window_unload(window);

    /* Start of custom code */
    //Destroy GBitmap
    gbitmap_destroy(s_background_bitmap_renegade);
    gbitmap_destroy(s_background_bitmap_paragon);

    //Destroy BitmapLayer
    bitmap_layer_destroy(s_background_layer);
}

void custom_tick_handler(struct tm *tick_time, TimeUnits units_changed) {
    /* Test, this changes the image once a minute */
    s_bg_image = s_bg_image == RESOURCE_ID_IMAGE_RENEGADE ? RESOURCE_ID_IMAGE_PARAGON : RESOURCE_ID_IMAGE_RENEGADE;
    if (s_bg_image == RESOURCE_ID_IMAGE_RENEGADE)
    {
        bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap_renegade);
    }
    else
    {
        bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap_paragon);
    }

    update_time();
}
