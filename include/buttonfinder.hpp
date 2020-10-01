#include <string>
#include "beatsaber-hook/shared/utils/typedefs.h"
#include "codegen/include/UnityEngine/Resources.hpp"
#include "codegen/include/UnityEngine/UI/Button.hpp"

class ButtonFinder 
{
    public: 
        std::string buttonName  = "";
        bool foundButton        = false;
        bool pressedButton      = false;
        bool* foundButtonPtr    = &foundButton;
        float waitingTime       = 1.0f;

        bool                        ReadyToPress(){ return (!pressedButton && foundButton); } 
        static bool                 FindButton(float time, bool* foundButtonPtr, std::string buttonName);
        UnityEngine::UI::Button*    GetButton();
        
        
};