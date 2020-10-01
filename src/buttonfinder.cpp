#include "../include/buttonfinder.hpp"
UnityEngine::UI::Button* ButtonFinder::GetButton()
{
    UnityEngine::UI::Button* tempButton;
    Array<UnityEngine::UI::Button*>* buttons = UnityEngine::Resources::FindObjectsOfTypeAll<UnityEngine::UI::Button*>();
    for(int i = 0; i < buttons->Length(); i++)
    {
        std::string foundBtn = to_utf8(csstrtostr(buttons->values[i]->get_name()));
        if(foundBtn == buttonName)
        {
            return buttons->values[i];
        }
    }
    return nullptr;
}

bool ButtonFinder::FindButton(float time, bool* boolPtr, std::string buttonName)
{
    bool found = false;
    while(!found)
    {
       Array<UnityEngine::UI::Button*>* buttons = UnityEngine::Resources::FindObjectsOfTypeAll<UnityEngine::UI::Button*>();
        for(int i = 0; i < buttons->Length(); i++)
        {
            std::string foundBtn = to_utf8(csstrtostr(buttons->values[i]->get_name()));
            if(foundBtn == buttonName)
            {
                found = true;
                break;
            }
        } 
    }
    if(found)
    {
        std::this_thread::sleep_for(std::chrono::duration<float>(time));
        *boolPtr = true; 
    }

    return false;
}

