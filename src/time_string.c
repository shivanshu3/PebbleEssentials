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

/**
 * Give the year from [0, Infinity) where it is the number
 * of years after 1900.
 * Also give a big enough buffer.
 */
void getYearString(int year, char *buffer){
	year += 1900;
	int firstDigit = year/1000;
	int secondDigit = (year - firstDigit*1000)/100;
	int thirdDigit = (year - firstDigit*1000 - secondDigit*100)/10;
	int fourthDigit = year % 10;

	buffer[0] = firstDigit + '0';
	buffer[1] = secondDigit + '0';
	buffer[2] = thirdDigit + '0';
	buffer[3] = fourthDigit + '0';
	buffer[4] = 0;
}

/**
 * Give the day from [0-6] Where 0 is Sunday
 */
void getDayString(int day, char *buffer){
	char *dayString;
	switch(day){
		case 0:
			dayString = "Sunday";
			break;
		case 1:
			dayString = "Monday";
			break;
		case 2:
			dayString = "Tuesday";
			break;
		case 3:
			dayString = "Wednesday";
			break;
		case 4:
			dayString = "Thursday";
			break;
		case 5:
			dayString = "Friday";
			break;
		case 6:
			dayString = "Saturday";
			break;
		default:
			dayString = "Error";
			break;
	}

	strcpy(buffer, dayString);
}

/**
 * Charge percent should be between [10,100]
 */
void getBatteryString(int charge_percent, char *buffer){
	if(charge_percent == 100){
		buffer[0] = 1 + '0';
		buffer[1] = 0 + '0';
		buffer[2] = 0 + '0';
		buffer[3] = 0;
		return;
	}

	int firstDigit = charge_percent/10;
	int secondDigit = charge_percent - firstDigit*10;
	buffer[0] = firstDigit + '0';
	buffer[1] = secondDigit + '0';
	buffer[2] = 0;
}
