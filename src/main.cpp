#include "../include/main.hpp"



static ModInfo modInfo;

const Logger& getLogger() {
  static const Logger& logger(modInfo);
  return logger;
}

std::string scenesNames[3] = {"ShaderWarmup", "HealthWarning", "MenuViewControllers"};


ButtonFinder continueButton;

DECLARE_CLASS_CODEGEN(Il2CppNamespace, ButtonChecker, UnityEngine::MonoBehaviour,
    DECLARE_METHOD(void, Update);
    REGISTER_FUNCTION(ButtonChecker,
        REGISTER_METHOD(Update);
    )
)


void Il2CppNamespace::ButtonChecker::Update() 
{
  if(!continueButton.pressedButton && continueButton.foundButton)
  {
    UnityEngine::UI::Button* continueBtn = continueButton.GetButton();
    continueBtn->Press();
    continueButton.pressedButton = true;
  }
}


MAKE_HOOK_OFFSETLESS(ActiveSceneChanged, void, Scene prevScene, Scene nextScene)
{
   ActiveSceneChanged(prevScene, nextScene);
   std::string sceneName = Utils::GetSceneName(nextScene);
   if(sceneName == "") return; 
   
   // Scene ShaderWarmup 
   // Adding customtype CoroutineType to recreate the coroutine.
   if(sceneName == scenesNames[0])
   {
     auto* gameObject =     UnityEngine::GameObject::New_ctor(il2cpp_utils::createcsstr("ButtonCheckerGO"));
     auto* component =      RET_V_UNLESS(il2cpp_utils::RunMethod(gameObject, "AddComponent", typeof(Il2CppNamespace::ButtonChecker*)));
     UnityEngine::Object::DontDestroyOnLoad(gameObject);
   }

   if(sceneName == scenesNames[1])
   {
     continueButton.buttonName = "Continue";
     std::thread buttonFind1(continueButton.FindButton, continueButton.waitingTime, continueButton.foundButtonPtr, continueButton.buttonName);
     buttonFind1.detach();
   }
   


}

extern "C" void setup(ModInfo& info) 
{
    info.id = "JustTakeMeToTheSoloMode";
    info.version = "0.1.0";
    modInfo = info;
    getLogger().info(info.version);
    getLogger().info(info.id);
}

extern "C" void load() 
{
  custom_types::Register::RegisterType<Il2CppNamespace::ButtonChecker>();
  INSTALL_HOOK_OFFSETLESS(ActiveSceneChanged, il2cpp_utils::FindMethodUnsafe("UnityEngine.SceneManagement", "SceneManager", "Internal_ActiveSceneChanged", 2));
}
