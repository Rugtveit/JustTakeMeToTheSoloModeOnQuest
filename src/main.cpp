#include "../include/main.hpp"
#include "../include/utils.hpp"
#include "../extern/custom-types/shared/register.hpp"
#include "../extern/custom-types/shared/macros.hpp"
#include "../extern/custom-types/shared/logging.hpp"
#include "../extern/custom-types/shared/types.hpp"
#include "../extern/codegen/include/UnityEngine/GameObject.hpp"
#include "../extern/codegen/include/UnityEngine/MonoBehaviour.hpp"
#include <thread>
#include <chrono>


static ModInfo modInfo;

const Logger& getLogger() {
  static const Logger& logger(modInfo);
  return logger;
}
std::string buttonTextContinue = "Contine";
std::string buttonTextSolo = "SoloFreePlayButton";
std::string scenesNames[3] = {"ShaderWarmup", "HealthWarning", "MenuViewControllers"};
bool timerOne = false;
bool timerTwo = false;
bool* timerOnePtr = &timerOne;
bool* timerTwoPtr = &timerTwo;

DECLARE_CLASS_CODEGEN(Il2CppNamespace, Timer, UnityEngine::MonoBehaviour,
    DECLARE_METHOD(void, Update);
    REGISTER_FUNCTION(Timer,
        REGISTER_METHOD(Update);
    )
)


void Il2CppNamespace::Timer::Update() 
{
  
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
     auto* gameObject =     UnityEngine::GameObject::New_ctor(il2cpp_utils::createcsstr("TimerGO"));
     auto* component =      RET_V_UNLESS(il2cpp_utils::RunMethod(gameObject, "AddComponent", typeof(Il2CppNamespace::Timer*)));
     UnityEngine::Object::DontDestroyOnLoad(gameObject);
   }


}

MAKE_HOOK_OFFSETLESS(HealthWarningSceneStart_Start, void, Il2CppObject*self )
{
  HealthWarningSceneStart_Start(self);
  std::thread buttonClick2(clickButton, "SoloFreePlayButton", foundButton2Ptr);
	buttonClick2.detach();
}

MAKE_HOOK_OFFSETLESS(MenuSceneSetup_Start, void, Il2CppObject*self )
{
  MenuSceneSetup_Start(self);
}

void StartTimer(float seconds, bool* timerDone);


extern "C" void setup(ModInfo& info) 
{
    info.id = "JustTakeMeToTheSoloMode";
    info.version = "0.0.1";
    modInfo = info;
    getLogger().info(info.version);
    getLogger().info(info.id);
}

extern "C" void load() 
{
  custom_types::Register::RegisterType<Il2CppNamespace::Timer>();
  INSTALL_HOOK_OFFSETLESS(ActiveSceneChanged, il2cpp_utils::FindMethodUnsafe("UnityEngine.SceneManagement", "SceneManager", "Internal_ActiveSceneChanged", 2));
  INSTALL_HOOK_OFFSETLESS(HealthWarningSceneStart_Start, il2cpp_utils::FindMethodUnsafe("", "HealthWarningSceneStart", "Start", 0));
  INSTALL_HOOK_OFFSETLESS(MenuSceneSetup_Start, il2cpp_utils::FindMethodUnsafe("", "MenuSceneSetup", "Start", 0));
}