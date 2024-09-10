#ifndef H_HTTP_WS
#define H_HTTP_WS

#include <string>
#include <unordered_map>
#include <iostream>

#include <tegia/core/json.h>
#include <tegia/tegia.h>
#include <tegia/db/mysql/mysql.h>

#include "connection.h"


struct workspace_t
{
	std::string wsid;
	std::string actor;
	std::string domain;
	std::string t_create;
};


class workspaces_t
{
	private:
		std::unordered_map<std::string, workspace_t> * ws_map = nullptr;

	public:
		workspaces_t()
		{
			this->ws_map = new std::unordered_map<std::string, workspace_t>;
		};

		~workspaces_t() = default;

		std::tuple<int,workspace_t *> find(const std::string &wsid)
		{
			auto pos = this->ws_map->find(wsid);
			if(pos != this->ws_map->end())
			{
				std::cout << _OK_TEXT_ << "find ws" << std::endl;
				return std::move(std::make_tuple(200,&pos->second));
			}

			std::cout << _ERR_TEXT_ << "load ws" << std::endl;
			auto res = tegia::mysql::query("http_ws","SELECT * FROM `ws` WHERE `uuid` = '" + wsid + "'");

			if(res->code != 200)
			{
				delete res;
				return std::move(std::make_tuple(500,nullptr));
			}

			if(res->count() == 0)
			{
				delete res;
				return std::move(std::make_tuple(404,nullptr));
			}

			if(res->count() == 1)
			{
				workspace_t ws;

				ws.wsid = wsid;
				ws.actor = res->get("ws::actor",0);
				ws.domain = res->get("ws::domain",0);
				ws.t_create = res->get("ws::t_create",0);
				delete res;

				auto [iter,code] = this->ws_map->insert({wsid,ws});
				return std::make_tuple(200,&iter->second);
			}

			return std::make_tuple(500,nullptr);
		};
};


#endif