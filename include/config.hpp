#include "modloader/shared/modloader.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"

class ModConfig 
{
    public:
        static ModInfo modInfo;

        static Configuration& getConfig() {
            static Configuration config(modInfo);
            return config;
        }

        static struct Config_t
        {
            int     selectTab = -1; 
            float   wait = 1.0f;
        } Config;

        static void saveConfig();
        static bool loadConfig();
};