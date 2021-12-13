#include <iostream>
#include <uwebsockets/App.h>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "parsing_csv.h"

int main()
{
    /* Instance of App Class with an handler type Post
    the notation [...](...) represents a lambda expression:
        - in the capture group [...] we put variables OUTSIDE the scope of the lambda expression
        - the params (...) are passed to the handler */
    uWS::App().post("/*", [](auto* res, auto* req)
        {
            // the datas received are text-based, we already know that (JSON)
            std::string buffer;

            // res->onData attaches a read handler (that is also a function) for datas sent
            // e.g.: in this case, the capture group is calling res and buffer that are situated inside the enclosing and anonymous function [](auto..., auto...)
            // std::string_view sets the variable 'data' as immutable (data becomes const-type)
            // the variable "bool last" indicates if the chunk readed is the last
            // "mutable" is a key-word to indicate that the variables inside the capture group can be modified
            res->onData([res, buffer](std::string_view data, bool last) mutable
            {
                // the datas are added into the buffer
                buffer.append(data.data(), data.length());

                // after adding the last chunk inside the buffer, last is set to true
                if (last) {
                    // c_str: conversion of the string to an array of const char* with NULL byte termination
                    vector<bet_data> betting_data = bet_data::from_buffer_to_list(buffer.c_str());
                    parsing_csv csv_parser = parsing_csv({ "web_site", "date", "match", "one", "ics", "two",
                        "gol", "over", "under" }, betting_data);
                    auto filename = csv_parser.save_into_file();
                    // writing of an HTTP header
                    res->writeHeader("Content-Type", "text/html; charset=utf-8")->end(filename.c_str());
                }
            });
            res->onAborted([res]() {
                perror("Error!");
                res->writeHeader("Content-Type", "text/html; charset=utf-8")->writeStatus("400 Bad Request")->end();
                });
        }).listen(3000, [](auto* listen_socket)
            {
                if (listen_socket)
                {
                    std::cout << "Listening on port " << 3000 << std::endl;
                }
            }).run();

            std::cout << "Failed to listen on port 3000" << std::endl;
}
