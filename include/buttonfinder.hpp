#include <string>
#include "beatsaber-hook/shared/utils/typedefs.h"

class ButtonFinder 
{
    public: 
        std::string buttonName = "";
        Il2CppObject* FindButton();
        bool foundButton = false;
        bool pressedButton = false;
    private:
        bool* foundButtonPtr = &foundButton;
};