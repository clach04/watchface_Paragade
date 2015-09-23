#pragma once

#define USE_GENERIC_MAIN
#undef REMOVE_LEADING_ZERO_FROM_TIME  /* 12 hour display will not have a leading "0" if this is defined */
#define FONT_NAME RESOURCE_ID_FONT_JUPITER_60

#undef DEBUG_TIME

#ifdef PBL_PLATFORM_BASALT
    /* Inverted looks better on Basalt, not so on Aplite */
    #define DEFAULT_TIME_COLOR GColorBlack
    #define DEFAULT_BACKGROUND_COLOR GColorWhite
#endif /* PBL_PLATFORM_BASALT */

#define USE_SHADOW_TIME_EFFECT

//#define CLOCK_POS GRect(0, 52, 144, 90) /* probably taller than really needed */
#define CLOCK_POS GRect(0, 52, 144, 120 - 52) /* probably taller than really needed */

#define FONT_BT_SYSTEM_NAME FONT_KEY_GOTHIC_18_BOLD
#define BT_POS GRect(0, 30, 144, 168) /* probably taller than really needed */

#define DATE_POS GRect(0, -5, 144 - 5, 168) /* probably taller than really needed */
#define BAT_POS GRect(5, 144, 144, 168) /* probably taller than really needed */

#define TICK_HANDLER custom_tick_handler
extern void custom_tick_handler(struct tm *tick_time, TimeUnits units_changed);

#define MAIN_WINDOW_LOAD custom_main_window_load
extern void custom_main_window_load(Window *window);

#define MAIN_WINDOW_UNLOAD custom_main_window_unload
extern void custom_main_window_unload(Window *window);
