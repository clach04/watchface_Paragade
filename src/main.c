/*
** Watchface Framework - Color or Mono
**
**    https://github.com/clach04/watchface_framework
**
** This file may not be needed. This is only needed if additional C code is required.
** This file implements two images, switched once per minute.
*/

#include <pebble.h>

#include "watchface.h"


static BitmapLayer *s_background_layer=NULL;  /* NOTE s_bg_image is NOT defined */

static uint32_t s_bg_image=RESOURCE_ID_IMAGE_ALT;  // or RESOURCE_ID_IMAGE_MAIN
static GBitmap     *s_background_bitmap_main=NULL;
static GBitmap     *s_background_bitmap_alt=NULL;


void custom_main_window_load(Window *window) {
    Layer *window_layer=window_get_root_layer(window);
#ifdef BG_IMAGE_GRECT
    GRect bounds=BG_IMAGE_GRECT;  /* Hand crafted */
#else /* BG_IMAGE_GRECT */
    GRect bounds=layer_get_bounds(window_layer);  /* screen size, center image */
#endif /* BG_IMAGE_GRECT */


    // Create GBitmap, then set to created BitmapLayer
    s_background_bitmap_main = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_MAIN);
    s_background_bitmap_alt = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ALT);
    
    s_background_layer = bitmap_layer_create(bounds);

#ifdef FAST_SWITCH
    /* show s_bg_image and then within 1 second show the next image */
    if (s_bg_image == RESOURCE_ID_IMAGE_MAIN)
#else /* not FAST_SWITCH */
    /*
    ** set reverse image,  TICK_HANDLER()/custom_tick_handler() is called
    ** almost immediately which will toggle the image
    */
    if (s_bg_image != RESOURCE_ID_IMAGE_MAIN)
#endif /* FAST_SWITCH/KEEP_IMAGE */
    {
        bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap_main);
    }
    else
    {
        bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap_alt);
    }

#ifdef PBL_COLOR
     bitmap_layer_set_compositing_mode(s_background_layer, GCompOpSet);
#else /* PBL_BW MONO */
     bitmap_layer_set_compositing_mode(s_background_layer, GCompOpAssign);
#endif

    window_set_background_color(main_window, background_color);

    layer_add_child(window_layer, bitmap_layer_get_layer(s_background_layer));

    /* End of custom code, call generic code */
    main_window_load(window);
}

void custom_main_window_unload(Window *window) {
    /* Call generic code */
    main_window_unload(window);

    /* Start of custom code */
    //Destroy GBitmap
    gbitmap_destroy(s_background_bitmap_main);
    gbitmap_destroy(s_background_bitmap_alt);

    //Destroy BitmapLayer
    bitmap_layer_destroy(s_background_layer);
}

void custom_tick_handler(struct tm *tick_time, TimeUnits units_changed) {
    /* Test, this changes the image once a minute */
    s_bg_image = s_bg_image == RESOURCE_ID_IMAGE_MAIN ? RESOURCE_ID_IMAGE_ALT : RESOURCE_ID_IMAGE_MAIN;
    if (s_bg_image == RESOURCE_ID_IMAGE_MAIN)
    {
        bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap_main);
    }
    else
    {
        bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap_alt);
    }

    update_time();
}
