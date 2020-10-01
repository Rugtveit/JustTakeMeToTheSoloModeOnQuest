#include "../extern/codegen/include/GlobalNamespace/LevelFilteringNavigationController.hpp"
#include "../extern/codegen/include/GlobalNamespace/TabBarViewController.hpp"
#include "codegen/include/UnityEngine/Resources.hpp"

class TabSelector 
{
    public:
        int     tab = 0;
        bool    waitDone    = false;
        bool*   waitDonePtr = &waitDone;
        float   waitTime    = 0.0f;
        bool    tabSelected = false;

        void    SelectTab();
        bool    ReadyToSelect(){ return (!tabSelected && waitDone); }
        bool    IsValidTab(){ return (tab >= 0 && tab <= 2); }
};