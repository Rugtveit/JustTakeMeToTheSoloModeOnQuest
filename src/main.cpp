#include "../include/main.hpp"

static ModInfo modInfo;

const Logger& getLogger() {
  static const Logger& logger(modInfo);
  return logger;
}

DECLARE_CLASS_CODEGEN(Il2CppNamespace, WaitUntilType, UnityEngine::MonoBehaviour,
    DECLARE_METHOD(void, Update);
    REGISTER_FUNCTION(WaitUntilType,
        REGISTER_METHOD(Update);
    )
)

ButtonFinder buttonFinder1; // Continue button finder
ButtonFinder buttonFinder2; // SoloMode button finder
TabSelector tabSelector; 
float waitingTime = 1.5f;

MAKE_HOOK_OFFSETLESS(ActiveSceneChanged, void, Scene prevScene, Scene nextScene)
{
  ActiveSceneChanged(prevScene, nextScene);
  std::string nextSceneName = Utils::GetSceneName(nextScene);
  if(nextSceneName == "") return; 
  
  if(nextSceneName == "ShaderWarmup")
  {
    auto* gameObject =  UnityEngine::GameObject::New_ctor(il2cpp_utils::createcsstr("WaitUntilTypeGO"));
    auto* component =   gameObject->AddComponent<Il2CppNamespace::WaitUntilType*>();
    UnityEngine::Object::DontDestroyOnLoad(gameObject);
  }

  if(nextSceneName == "HealthWarning")
  {
    buttonFinder1.buttonName = "Continue";
    buttonFinder1.waitingTime = waitingTime;
    std::thread findContinueBtnThread(buttonFinder1.FindButton, buttonFinder1.waitingTime, buttonFinder1.foundButtonPtr, buttonFinder1.buttonName);
    findContinueBtnThread.detach();
  }

  if(nextSceneName == "MenuViewControllers")
  {
    buttonFinder2.buttonName = "SoloFreePlayButton";
    buttonFinder2.waitingTime = waitingTime;
    std::thread findSoloBtnThread(buttonFinder2.FindButton, buttonFinder2.waitingTime, buttonFinder2.foundButtonPtr, buttonFinder2.buttonName);
    findSoloBtnThread.detach();
  }
   
}

void StartTimer(float time, bool* finishedTimer)
{
  std::this_thread::sleep_for(std::chrono::duration<float>(time));
  *finishedTimer = true; 
}

void Il2CppNamespace::WaitUntilType::Update() 
{
  if(buttonFinder1.ReadyToPress())
  {
    UnityEngine::UI::Button* continueBtn = buttonFinder1.GetButton();
    continueBtn->Press();
    buttonFinder1.pressedButton = true;
  }

  if(buttonFinder2.ReadyToPress())
  {
    UnityEngine::UI::Button* soloModeBtn = buttonFinder2.GetButton();
    soloModeBtn->Press();
    tabSelector.tab = 1;
    if(tabSelector.IsValidTab())
    {
      tabSelector.waitTime = waitingTime;
      std::thread waitThread(StartTimer, tabSelector.waitTime, tabSelector.waitDonePtr);
      waitThread.detach();
    }
    buttonFinder2.pressedButton = true;
  }

  if(tabSelector.ReadyToSelect())
  {
    tabSelector.SelectTab();
    tabSelector.tabSelected = true;
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
  custom_types::Register::RegisterType<Il2CppNamespace::WaitUntilType>();
  INSTALL_HOOK_OFFSETLESS(ActiveSceneChanged, il2cpp_utils::FindMethodUnsafe("UnityEngine.SceneManagement", "SceneManager", "Internal_ActiveSceneChanged", 2));
}
