#pragma once
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <cstdlib>
#include <format>


struct MongoDB
{
	static inline mongocxx::instance inst{};
	static inline mongocxx::client conn{
		mongocxx::uri{
			std::format("mongodb+srv://claudi47:{}@apldb.hhz9g.mongodb.net/db_apl?retryWrites=true&w=majority", getenv("mongodb_Password"))
		}
	};
	static inline mongocxx::database db = conn["db_apl"];
};

