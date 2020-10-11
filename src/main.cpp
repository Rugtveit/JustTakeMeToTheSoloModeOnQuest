#include "../include/main.hpp"

static ModInfo modInfo;
ModConfig::Config_t ModConfig::Config;


const Logger& getLogger() {
  static const Logger& logger(modInfo);
  return logger;
}


// Declaring CustomType codegen class
DECLARE_CLASS_CODEGEN(Il2CppNamespace, WaitUntilType, UnityEngine::MonoBehaviour,
    DECLARE_OVERRIDE_METHOD(void, Update, il2cpp_utils::FindMethodUnsafe("UnityEngine", "MonoBehaviour", "Update", 0));
    REGISTER_FUNCTION(WaitUntilType,
        REGISTER_METHOD(Update);
    )
)

DEFINE_CLASS(Il2CppNamespace::WaitUntilType);

ButtonFinder buttonFinder1; // Continue button
ButtonFinder buttonFinder2; // SoloMode button 
TabSelector tabSelector; 
void Initialize()
{
  buttonFinder1.buttonName = "Continue";
  buttonFinder1.waitingTime = ModConfig::Config.wait + 1.0f; // Adding 1 second delay because of other mods to make sure they get loaded

  buttonFinder2.buttonName = "SoloFreePlayButton";
  buttonFinder2.waitingTime = ModConfig::Config.wait;

  tabSelector.tab = ModConfig::Config.selectTab;
  tabSelector.waitTime = ModConfig::Config.wait;
}

std::string GetSceneName(Scene scene);

UnityEngine::GameObject* waitUntilGO = nullptr;

MAKE_HOOK_OFFSETLESS(ActiveSceneChanged, void, Scene prevScene, Scene nextScene)
{
  ActiveSceneChanged(prevScene, nextScene);
  std::string nextSceneName = GetSceneName(nextScene);
  if(nextSceneName == "") return; 
  
  if(nextSceneName == "ShaderWarmup")
  {
    waitUntilGO =  UnityEngine::GameObject::New_ctor(il2cpp_utils::createcsstr("WaitUntilTypeGO"));
    waitUntilGO->AddComponent<Il2CppNamespace::WaitUntilType*>();
    UnityEngine::Object::DontDestroyOnLoad(waitUntilGO);
  }

  if(nextSceneName == "HealthWarning")
  {
    std::thread findContinueBtnThread(buttonFinder1.FindButton, buttonFinder1.waitingTime, buttonFinder1.foundButtonPtr, buttonFinder1.buttonName);
    findContinueBtnThread.detach();
  }

  if(nextSceneName == "MenuViewControllers")
  {
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
 
    if(tabSelector.IsValidTab())
    {
      std::thread waitThread(StartTimer, tabSelector.waitTime, tabSelector.waitDonePtr);
      waitThread.detach();
    } else UnityEngine::GameObject::Destroy(waitUntilGO);
    
    buttonFinder2.pressedButton = true;
  }

  if(tabSelector.ReadyToSelect())
  {
    tabSelector.SelectTab();
    tabSelector.tabSelected = true;
    UnityEngine::GameObject::Destroy(waitUntilGO);
  }
}

extern "C" void setup(ModInfo& info) 
{
    info.id = "JustTakeMeToTheSoloMode";
    info.version = "0.1.0";
    modInfo = info;
    ModConfig::modInfo = modInfo;
    getLogger().info(info.version);
    getLogger().info(info.id);
}

extern "C" void load() 
{
  Initialize();
  if(!ModConfig::loadConfig())ModConfig::saveConfig();
  custom_types::Register::RegisterType<Il2CppNamespace::WaitUntilType>();
  INSTALL_HOOK_OFFSETLESS(ActiveSceneChanged, il2cpp_utils::FindMethodUnsafe("UnityEngine.SceneManagement", "SceneManager", "Internal_ActiveSceneChanged", 2));
}

std::string GetSceneName(Scene scene)
{
    Il2CppString* csString = RET_UNLESS("", il2cpp_utils::RunMethod<Il2CppString*>("UnityEngine.SceneManagement", "Scene", "GetNameInternal", scene.m_Handle));
    RET_UNLESS("", csString);
    return to_utf8(csstrtostr(csString));
}