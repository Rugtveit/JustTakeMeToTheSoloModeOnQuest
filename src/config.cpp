#include "../include/config.hpp"
#include <string_view>


ModInfo ModConfig::modInfo;

#define GET(obj, fieldName, method) auto itr = obj.FindMember(fieldName.data()); \
if (itr == obj.MemberEnd()) { \
    return std::nullopt; \
} \
return itr->value.method(); \

std::optional<float> getFloat(rapidjson::Value& obj, std::string_view fieldName) {
    GET(obj, fieldName, GetFloat);
}

std::optional<int> getInt(rapidjson::Value& obj, std::string_view fieldName) {
    GET(obj, fieldName, GetInt);
}




bool ModConfig::loadConfig()
{
    getConfig().Load();	
    ConfigDocument& configDoc = getConfig().config;
    if(auto selectTab = getInt(configDoc, "selectTab")) Config.selectTab = *selectTab; else return false;
    if(auto wait = getFloat(configDoc, "wait")) Config.wait = *wait; else return false;
	return true;
};

void ModConfig::saveConfig()
{
    ConfigDocument& configDoc = getConfig().config;
    configDoc.RemoveAllMembers();
	configDoc.SetObject();
	auto &allocator = configDoc.GetAllocator();
    configDoc.AddMember("selectTab", Config.selectTab, allocator);
    configDoc.AddMember("wait", Config.wait, allocator);
	getConfig().Write();
}

