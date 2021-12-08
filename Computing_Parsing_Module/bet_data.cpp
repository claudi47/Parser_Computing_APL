#include "bet_data.h"


vector<bet_data> bet_data::from_buffer_to_list(const char* buffer)
{
    // instantiation of a object Document with enconding UTF-8
    rapidjson::Document doc;
    // Parsing of the json in a document
    doc.Parse(buffer);

    vector<bet_data> value_list;

    for (auto it = doc.Begin(); it != doc.End(); it++) {
        bet_data row((*it)["web_site"].GetString(), (*it)["date"].GetString(), (*it)["match"].GetString(), (*it)["one"].GetString(), 
            (*it)["ics"].GetString(), (*it)["two"].GetString(), (*it)["gol"].GetString(), (*it)["over"].GetString(), (*it)["under"].GetString());

        value_list.push_back(row);
    }
    
    return value_list;
}

string bet_data::to_string() {
    return web_site + ", " + date + ", " + match + ", " + one + ", " + ics + ", " + two + ", " + gol + ", " + over + ", " + under;
}
