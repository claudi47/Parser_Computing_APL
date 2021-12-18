#pragma once
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>


class MongoDB
{
	mongocxx::instance inst{};
	mongocxx::client conn{
		mongocxx::uri{
	  "mongodb+srv://claudi47:Abcd1900@@apldb.hhz9g.mongodb.net/db_apl?retryWrites=true&w=majority"
		}
	};
	mongocxx::database db = conn["db_apl"];
};

