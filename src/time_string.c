#include <pebble.h>

/**
 * Give a month from [0-11]
 */
char * getMonthString(int month){
	switch(month){
		case 0:
			return "January";
		case 1:
			return "February";
		case 2:
			return "March";
		case 3:
			return "April";
		case 4:
			return "May";
		case 5:
			return "June";
		case 6:
			return "July";
		case 7:
			return "August";
		case 8:
			return "September";
		case 9:
			return "October";
		case 10:
			return "November";
		case 11:
			return "December";
		default:
			return "Error";
	}
}

/**
 * Give a date from [1-31]
 * and a big enough buffer
 */
void getDateString(int date, char *buffer){
	int firstDigit = date/10;
	int secondDigit = date - firstDigit*10;
	buffer[0] = firstDigit + '0';
	buffer[1] = secondDigit + '0';
	buffer[2] = 0;
}

/**
 * Give a month from [0-11]
 * date from [1-31]
 * a big enough buffer
 */
void getMonthDateString(int month, int date, char *buffer){
	char *monthString = getMonthString(month);
	strcpy(buffer, monthString);
	int monthLength = strlen(monthString);
	buffer[monthLength] = ' ';
	getDateString(date, &buffer[monthLength+1]);
}
