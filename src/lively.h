
/*
 * @name Lively
 * @type Watchface
 * @author Edward Hotchkiss <edwardhotchkiss@me.com>
 * @description Utilitarian Pebble Watchface
 * @license MIT
 */

#include "pebble_os.h"

// main window
Window main_window;

// date and time layer
TextLayer time_text_layer;
TextLayer date_text_layer;

// image layer
Layer background_layer;
BmpContainer background_image;

// definitions
#define SCREEN_WIDWTH 144
#define SCREEN_HEIGHT 168

// custom fonts
GFont avenir_font;
GFont silkscreen_font;

// date/time format chars
static char time_text[] = "00:00";
static char date_text[] = "Xxx 00";

/* EOF */