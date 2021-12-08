#pragma once
#include <iostream>
#include <vector>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace std;

struct bet_data
{
	/* This is a structure that contains the field passed through the request sended by the web server */
	string web_site, date, match, one, ics, two, gol, over, under;

	// constructor, the ":" is the initializer list
	bet_data(string web_site, string date, string match, string one, string ics, string two, 
		string gol, string over, string under) : web_site(web_site), date(date),
		match(match), one(one), ics(ics), two(two), gol(gol), over(over), under(under) {}

	// conversion of the data from json, contained in the buffer, to object
	static vector<bet_data> from_buffer_to_list(const char* buffer);

	string to_string();
};

