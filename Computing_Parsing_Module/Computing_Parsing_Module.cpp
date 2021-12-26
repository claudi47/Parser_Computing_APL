#include <iostream>
#include <uwebsockets/App.h>
#include "parsing_csv.h"
#include "MongoDB.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/builder/stream/document.hpp>

using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;

int main()
{
	/* Instance of App Class with an handler type Post
	the notation [...](...) represents a lambda expression:
	    - in the capture group [...] we put variables OUTSIDE the scope of the lambda expression
	    - the params (...) are passed to the handler */
	uWS::App().post("/*", [](auto* res, auto* req)
	          {
		          res->onAborted([res]()
		          {
			          perror("Error!");
			          res->writeHeader("Content-Type", "text/html; charset=utf-8")->writeStatus("400 Bad Request")->
			               end();
		          });

		          // the data received are text-based, we already know that (JSON)
		          std::string buffer;

		          // res->onData attaches a read handler (that is also a function) for datas sent
		          // e.g.: in this case, the capture group is calling res and buffer that are situated inside the enclosing and anonymous function [](auto..., auto...)
		          // std::string_view sets the variable 'data' as immutable (data becomes const-type)
		          // the variable "bool last" indicates if the chunk readed is the last
		          // "mutable" is a key-word to indicate that the variables inside the capture group can be modified
		          res->onData([res, buffer](std::string_view data, bool last) mutable
		          {
			          // the data are added into the buffer
			          buffer.append(data.data(), data.length());

			          // after adding the last chunk inside the buffer, last is set to true
			          if (last)
			          {
				          // c_str: conversion of the string to an array of const char* with NULL byte termination
				          vector<bet_data> betting_data = bet_data::from_buffer_to_list(buffer.c_str());
				          parsing_csv csv_parser = parsing_csv({
					                                               "date", "match", "one", "ics", "two",
					                                               "gol", "over", "under"
				                                               }, betting_data);
				          auto filename = csv_parser.save_into_file();
				          // writing of an HTTP header
				          res->writeHeader("Content-Type", "text/html; charset=utf-8")->end(filename.c_str());
			          }
		          });
	          }).get("/stats", [](auto* res, auto* req)
	          {
		          res->onAborted([res]()
		          {
			          perror("Error!");
			          res->writeHeader("Content-Type", "text/html; charset=utf-8")->writeStatus("400 Bad Request")->
			               end();
		          });

				  rapidjson::Document doc; // this is the DOM (Document Object Model) end from him we use the Allocator
				  rapidjson::StringBuffer buffer;
				  rapidjson::Writer writer(buffer);

		          if (req->getQuery("stat") == "1") // case stat_1
		          {
					  // retrieving the collection and the data from it	
			          auto users = MongoDB::db["web_server_user"]; //collection
			          auto count = users.count_documents({}); //count of the users
			          auto cursor = users.find({}); // getting all the elements of the collection
			          vector<string> usernames;
			          for (auto doc : cursor) // for each
			          {
						// push back means "append into the vector" the value (a string), in utf-8 standard
				          usernames.push_back(doc["username"].get_utf8().value.to_string());
			          }
			          rapidjson::Value responseUsers(rapidjson::kObjectType); // Generic Object value
			          rapidjson::Value responseUsernames(rapidjson::kArrayType); // generic Array value
			          responseUsers.AddMember("total_users", count, doc.GetAllocator());
			          for (const auto& username : usernames) // the reference prevents the copy of the element in the vector into the username var
				          responseUsernames.PushBack(rapidjson::Value(username.c_str(), doc.GetAllocator()).Move(), // with Move we avoid the copy of Value, instead Value will put directly inside the array
				                                     doc.GetAllocator());
			          responseUsers.AddMember("usernames", responseUsernames, doc.GetAllocator()); // appending the array of Value inside responseUsers

					  // Implementation of Visitor pattern, where the Writer, with this method, can access into responseUsers and can copy his content inside the buffer in JSON format
			          responseUsers.Accept(writer);
			          auto jsonValue = buffer.GetString();

			          res->writeHeader("Content-Type", "application/json; charset=utf-8")->end(jsonValue);
		          }
				  else if(req->getQuery("stat") == "2")
				  {
					  auto searches = MongoDB::db["web_server_search"];
					  auto count = searches.count_documents({});
					  rapidjson::Value responseSearches(rapidjson::kObjectType);
					  responseSearches.AddMember("total_csv_files", count, doc.GetAllocator());
					  responseSearches.Accept(writer);
					  auto jsonValue = buffer.GetString();
					  res->writeHeader("Content-Type", "application/json; charset=utf-8")->end(jsonValue);
				  }
				  else if (req->getQuery("stat") == "3")
				  {
					  auto searches_collection = MongoDB::db["web_server_search"];
					  auto searches_bwin = searches_collection.count_documents(document{} << "web_site" << "bwin" << finalize);
					  auto searches_goldbet = searches_collection.count_documents(document{} << "web_site" << "goldbet" << finalize);
					  rapidjson::Value responseSearches(rapidjson::kObjectType);
					  responseSearches.AddMember("bwin_total", searches_bwin, doc.GetAllocator());
					  responseSearches.AddMember("goldbet_total", searches_goldbet, doc.GetAllocator());
					  responseSearches.Accept(writer);
					  auto jsonValue = buffer.GetString();
					  res->writeHeader("Content-Type", "application/json; charset=utf-8")->end(jsonValue);
				  }
				  else if (req->getQuery("stat") == "4")
				  {
					  auto searches_collection = MongoDB::db["web_server_search"];
					  auto users_collection = MongoDB::db["web_server_user"];
					  auto users = users_collection.count_documents({});
					  auto searches = searches_collection.count_documents({});
					  if (users == 0) {
						  res->writeHeader("Content-Type", "text/html; charset=utf-8")->writeStatus("400 Bad Request")->end();
						  return;
					  }
					  double average = static_cast<double>(searches) / users;
					  rapidjson::Value responseSearches(rapidjson::kObjectType);
					  responseSearches.AddMember("average_researches", average, doc.GetAllocator());
					  responseSearches.Accept(writer);
					  auto jsonValue = buffer.GetString();
					  res->writeHeader("Content-Type", "application/json; charset=utf-8")->end(jsonValue);
				  }
	          })
	          .listen(3000, [](auto* listen_socket)
	          {
		          if (listen_socket)
		          {
			          std::cout << "Listening on port " << 3000 << std::endl;
		          }
	          }).run();


	std::cout << "Failed to listen on port 3000" << std::endl;
}
