#include <pebble.h>

//Static variables:
static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *am_pm_layer;
static GFont s_ubuntu_font;
static GFont s_ubuntu_really_small_font;
static TextLayer *s_month_date_layer;

//Functions:
static void tick_handler(struct tm *tick_time, TimeUnits units_changed);
static void update_time(void);
static void main_window_load(Window *);
static void main_window_unload(Window *);
static void init(void);
static void deinit(void);
int main(void);

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
	APP_LOG(APP_LOG_LEVEL_INFO, "Timer Fired");
	update_time();
}

static void update_time(){
	APP_LOG(APP_LOG_LEVEL_INFO, "Updating Time");
	
	// Get a tm structure
	time_t temp = time(NULL); 
	struct tm *tick_time = localtime(&temp);

	// Create a long-lived buffer
	static char buffer[] = "00:00";

	// Write the current hours and minutes into the buffer (Always 12 hour format):
	strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);

	//AM or PM?
	if(tick_time->tm_hour < 12){//AM
		text_layer_set_text(am_pm_layer, "A");
	}
	else{//PM
		text_layer_set_text(am_pm_layer, "P");
	}

	// Display this time on the TextLayer
	text_layer_set_text(s_time_layer, buffer);
}

static void main_window_load(Window *window) {
	// Create GFont
	s_ubuntu_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_UBUNTU_FONT_67));
	s_ubuntu_really_small_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_UBUNTU_FONT_13));

	// Create time TextLayer
	s_time_layer = text_layer_create(GRect(0, -17, 133, 70));
	text_layer_set_background_color(s_time_layer, GColorBlack);
	text_layer_set_text_color(s_time_layer, GColorWhite);
	text_layer_set_text(s_time_layer, "88:88");
	text_layer_set_font(s_time_layer, s_ubuntu_font);

	//Create the 'M' layer in AM/PM:
	TextLayer *am_pm_m_layer = text_layer_create(GRect(133, 24, 14, 22));
	text_layer_set_background_color(am_pm_m_layer, GColorBlack);
	text_layer_set_text_color(am_pm_m_layer, GColorWhite);
	text_layer_set_text(am_pm_m_layer, "M");
	text_layer_set_font(am_pm_m_layer, s_ubuntu_really_small_font);

	//Create the 'A/P' layer in AM/PM:
	am_pm_layer = text_layer_create(GRect(133, 6, 14, 20));
	text_layer_set_background_color(am_pm_layer, GColorBlack);
	text_layer_set_text_color(am_pm_layer, GColorWhite);
	text_layer_set_text(am_pm_layer, "Z");//Z is an arbitrary choice
	text_layer_set_font(am_pm_layer, s_ubuntu_really_small_font);

	s_month_date_layer = text_layer_create(GRect(0, 78, 133, 20));
	text_layer_set_background_color(s_month_date_layer, GColorBlack);
	text_layer_set_text_color(s_month_date_layer, GColorWhite);
	text_layer_set_text(s_month_date_layer, "September 21");
	text_layer_set_font(s_month_date_layer, s_ubuntu_really_small_font);

	// Add it as a child layer to the Window's root layer
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(am_pm_m_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(am_pm_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_month_date_layer));
}

static void main_window_unload(Window *window) {
	// Destroy TextLayer
	text_layer_destroy(s_time_layer);

	// Unload GFont
	fonts_unload_custom_font(s_ubuntu_font);
}

static void init() {
	// Create main Window element and assign to pointer
	s_main_window = window_create();
	//window_set_background_color(s_main_window, GColorBlack);

	// Set handlers to manage the elements inside the Window
	window_set_window_handlers(s_main_window, (WindowHandlers) {
		.load = main_window_load,
		.unload = main_window_unload
	});

	// Show the Window on the watch, with animated=true
	window_stack_push(s_main_window, true);
	
	// Register with TickTimerService
	tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

	//Initially update time:
	update_time();
}

static void deinit() {
	window_destroy(s_main_window);
}

int main(void) {
	init();
	app_event_loop();
	deinit();
	return 0;
}
