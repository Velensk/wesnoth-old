/*
   Copyright (C) 2003 - 2014 by David White <dave@whitevine.net>
   Part of the Battle for Wesnoth Project http://www.wesnoth.org/

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include "format_time_summary.hpp"

#include "gettext.hpp"
#include "preferences.hpp"
#include "strftime.hpp"

namespace util {

std::string format_time_summary(time_t t) {
	time_t curtime = time(NULL);
	const struct tm* timeptr = localtime(&curtime);
	if(timeptr == NULL) {
		return "";
	}

	const struct tm current_time = *timeptr;

	timeptr = localtime(&t);
	if(timeptr == NULL) {
		return "";
	}

	const struct tm save_time = *timeptr;

	const char* format_string = NULL;

	if(current_time.tm_year == save_time.tm_year) {
		const int days_apart = current_time.tm_yday - save_time.tm_yday;
		if(days_apart == 0) {
			// save is from today
			if(preferences::use_twelve_hour_clock_format() == false) {
				format_string = _("%H:%M");
			}
			else {
				format_string = _("%I:%M %p");
			}
		} else if(days_apart > 0 && days_apart <= current_time.tm_wday) {
			// save is from this week
			if(preferences::use_twelve_hour_clock_format() == false) {
				format_string = _("%A, %H:%M");
			}
			else {
				format_string = _("%A, %I:%M %p");
			}
		} else {
			// save is from current year
			format_string = _("%b %d");
		}
	} else {
		// save is from a different year
		format_string = _("%b %d %y");
	}
	assert(format_string);

	char buf[40];
	const size_t res = util::strftime(buf,sizeof(buf),format_string,&save_time);
	if(res == 0) {
		buf[0] = 0;
	}

	return buf;
}

}
