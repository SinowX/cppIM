#ifndef UTILS_H
#define UTILS_H

#include "json/json.h"

typedef Json::Value json_node;


std::string json2str(Json::Value root);

Json::Value str2json(std::string str);

#endif