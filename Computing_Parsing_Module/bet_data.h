#pragma once
#include <iostream>
#include <utility>
#include <vector>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace std;

struct bet_data
{
	/* This is a structure that contains the field passed through the request sent by the web server */
	string web_site, date, match, one, ics, two, gol, over, under;

	// constructor, the ":" is the initializer list
	// using move to prevent the second copy of the variable in the field
	bet_data(string web_site, string date, string match, string one, string ics, string two, 
		string gol, string over, string under) : web_site(move(web_site)), date(move(date)),
		match(move(match)), one(move(one)), ics(move(ics)), two(move(two)), gol(move(gol)),
		over(move(over)), under(move(under)) {}

	// conversion of the data from json, contained in the buffer, to object
	static vector<bet_data> from_buffer_to_list(const char* buffer);

	string to_string();
};

