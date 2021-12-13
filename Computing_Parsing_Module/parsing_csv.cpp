#include "parsing_csv.h"
#include "boost/lexical_cast.hpp"
#include "boost/uuid/random_generator.hpp"
#include "boost/uuid/uuid_io.hpp"
#include <fstream>

#pragma comment(lib, "Bcrypt.lib")


std::string parsing_csv::save_into_file()
{
	boost::uuids::random_generator generator;
	// lexical_cast is a template function that converts bytes into string
	// the param passed is a random generator that, with the extension of () operator, returns a sequence of random bytes
	string filename = boost::lexical_cast<string>(generator());

	ofstream csv_file; // ofstream prevede la chiusura automatica del file
	csv_file.open(filename + ".csv");
	
	for (int i = 0; i < header_csv.size(); i++) {
		csv_file << header_csv[i];
		if (i != header_csv.size() - 1)
			csv_file << ", ";
		else
			csv_file << endl;
	}
	
	for (auto it = row_list.begin(); it != row_list.end(); it++) {
		csv_file << (*it).to_string() << endl;
	}

	return filename + ".csv";
}
