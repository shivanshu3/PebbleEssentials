#include <pebble.h>

//Static variables:
static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *am_pm_layer;
static GFont s_ubuntu_font;
static GFont s_ubuntu_really_small_font;
static GFont s_ubuntu_date_font;
static TextLayer *am_pm_m_layer;
static TextLayer *s_month_date_layer;
static TextLayer *s_day_layer;
static TextLayer *s_year_layer;
static TextLayer *s_battery_percentage_layer;

//Time buffers:
static char *monthDateBuffer;
static char *dayBuffer;
static char *yearBuffer;
static char *batteryBuffer;

//Functions:
static void tick_handler(struct tm *tick_time, TimeUnits units_changed);
static void update_time(void);
static void main_window_load(Window *);
static void main_window_unload(Window *);
static void init(void);
static void deinit(void);

//Non static functions:
void getMonthDateString(int month, int date, char *buffer);
void getYearString(int year, char *buffer);
void getDayString(int day, char *buffer);

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
	update_time();
}

static void update_time(){
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

	//Get month, date, year, and day:
	int month = tick_time->tm_mon;
	int date = tick_time->tm_mday;
	int year = tick_time->tm_year;
	int day = tick_time->tm_wday;

	//Convert month, date, year, and day into strings:
	getMonthDateString(month, date, monthDateBuffer);
	getYearString(year, yearBuffer);
	getDayString(day, dayBuffer);

	//Display these: (These calls should only be made once, but w/e):
	text_layer_set_text(s_month_date_layer, monthDateBuffer);
	text_layer_set_text(s_year_layer, yearBuffer);
	text_layer_set_text(s_day_layer, dayBuffer);

	// Display this time on the TextLayer
	text_layer_set_text(s_time_layer, buffer);
}

static void main_window_load(Window *window) {
	// Create GFont
	s_ubuntu_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_UBUNTU_FONT_66));
	s_ubuntu_really_small_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_UBUNTU_FONT_15));
	s_ubuntu_date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_UBUNTU_FONT_25));

	// Create time TextLayer
	s_time_layer = text_layer_create(GRect(0, -17, 133, 70));
	text_layer_set_background_color(s_time_layer, GColorClear);
	text_layer_set_text_color(s_time_layer, GColorWhite);
	text_layer_set_text(s_time_layer, "88:88");
	text_layer_set_font(s_time_layer, s_ubuntu_font);

	//Create the 'M' layer in AM/PM:
	am_pm_m_layer = text_layer_create(GRect(128, 25, 14, 22));
	text_layer_set_background_color(am_pm_m_layer, GColorClear);
	text_layer_set_text_color(am_pm_m_layer, GColorWhite);
	text_layer_set_text(am_pm_m_layer, "M");
	text_layer_set_font(am_pm_m_layer, s_ubuntu_really_small_font);
	text_layer_set_text_alignment(am_pm_m_layer, GTextAlignmentCenter);

	//Create the 'A/P' layer in AM/PM:
	am_pm_layer = text_layer_create(GRect(128, 11, 14, 22));
	text_layer_set_background_color(am_pm_layer, GColorClear);
	text_layer_set_text_color(am_pm_layer, GColorWhite);
	text_layer_set_text(am_pm_layer, "Z");//Z is an arbitrary choice
	text_layer_set_font(am_pm_layer, s_ubuntu_really_small_font);
	text_layer_set_text_alignment(am_pm_layer, GTextAlignmentCenter);

	s_month_date_layer = text_layer_create(GRect(0, 54, 144, 40));
	text_layer_set_background_color(s_month_date_layer, GColorClear);
	text_layer_set_text_color(s_month_date_layer, GColorWhite);
	text_layer_set_text(s_month_date_layer, "Nomonth 00");
	text_layer_set_font(s_month_date_layer, s_ubuntu_date_font);
	text_layer_set_text_alignment(s_month_date_layer, GTextAlignmentCenter);

	s_day_layer = text_layer_create(GRect(0, 102, 144, 40));
	text_layer_set_background_color(s_day_layer, GColorClear);
	text_layer_set_text_color(s_day_layer, GColorWhite);
	text_layer_set_text(s_day_layer, "Noday");
	text_layer_set_font(s_day_layer, s_ubuntu_date_font);
	text_layer_set_text_alignment(s_day_layer, GTextAlignmentCenter);

	s_year_layer = text_layer_create(GRect(0, 79, 144, 40));
	text_layer_set_background_color(s_year_layer, GColorClear);
	text_layer_set_text_color(s_year_layer, GColorWhite);
	text_layer_set_text(s_year_layer, "8952");
	text_layer_set_font(s_year_layer, s_ubuntu_date_font);
	text_layer_set_text_alignment(s_year_layer, GTextAlignmentCenter);

	s_battery_percentage_layer = text_layer_create(GRect(0, 130, 144, 40));
	text_layer_set_background_color(s_battery_percentage_layer, GColorClear);
	text_layer_set_text_color(s_battery_percentage_layer, GColorWhite);
	text_layer_set_text(s_battery_percentage_layer, "Battery : 125%");
	text_layer_set_font(s_battery_percentage_layer, s_ubuntu_date_font);
	text_layer_set_text_alignment(s_battery_percentage_layer, GTextAlignmentCenter);

	// Add it as a child layer to the Window's root layer
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_battery_percentage_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_year_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_day_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_month_date_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(am_pm_m_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(am_pm_layer));
}

static void main_window_unload(Window *window) {
	// Destroy TextLayers:
	text_layer_destroy(s_time_layer);
	text_layer_destroy(am_pm_layer);
	text_layer_destroy(s_month_date_layer);
	text_layer_destroy(s_day_layer);
	text_layer_destroy(s_year_layer);
	text_layer_destroy(s_battery_percentage_layer);
	text_layer_destroy(am_pm_m_layer);

	// Unload GFonts:
	fonts_unload_custom_font(s_ubuntu_font);
	fonts_unload_custom_font(s_ubuntu_really_small_font);
	fonts_unload_custom_font(s_ubuntu_date_font);
}

static void init() {
	//Initialize the time buffers:
	monthDateBuffer = malloc(15);
	dayBuffer = malloc(12);
	yearBuffer = malloc(10);
	batteryBuffer = malloc(15);

	// Create main Window element and assign to pointer
	s_main_window = window_create();
	window_set_background_color(s_main_window, GColorBlack);

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
	//Free the time buffers:
	free(monthDateBuffer);
	free(dayBuffer);
	free(yearBuffer);
	free(batteryBuffer);

	window_destroy(s_main_window);
}

int main(void) {
	init();
	app_event_loop();
	deinit();
	return 0;
}
