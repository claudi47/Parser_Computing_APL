#pragma once
#include <iostream>
#include <vector>
#include "bet_data.h"

using namespace std;


class parsing_csv
{
	vector<string> header_csv;
	vector<bet_data> row_list;

public:
	parsing_csv(vector<string> header) : header_csv(header) {}
	parsing_csv(vector<string> header, vector<bet_data> rows) : header_csv(header), row_list(rows) {}
	
	string save_into_file();
};

