#ifndef GENERAL_HPP
#define GENERAL_HPP

#include <iostream>
#include <string>
#include <stdexcept>
#include <chrono>
#include <vector>
#include <unordered_map>
#include <map>
#define FMT_HEADER_ONLY
#include <fmt/format.h>
#include <json/json.h>

namespace Kepler
{
    typedef std::unordered_map<std::string,std::string> DefaultDataType;
    typedef DefaultDataType HashMap;
    typedef std::map<std::string,std::string> Map;
}

#endif