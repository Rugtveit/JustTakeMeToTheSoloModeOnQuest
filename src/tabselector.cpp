#include "../include/tabselector.hpp"
void TabSelector::SelectTab()
{
    Array<GlobalNamespace::LevelFilteringNavigationController*>* levelFilterNavController = UnityEngine::Resources::FindObjectsOfTypeAll<GlobalNamespace::LevelFilteringNavigationController*>();
    GlobalNamespace::LevelFilteringNavigationController* levelFilter = levelFilterNavController->values[0];
    GlobalNamespace::TabBarViewController* tabBar = levelFilter->tabBarViewController;
    if(tabBar != nullptr)
    {
        tabBar->SelectItem(tab);
        levelFilter->SwitchToPlaylists();
    }
}