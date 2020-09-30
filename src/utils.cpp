#include "../include/utils.hpp"

std::string Utils::GetSceneName(Scene scene)
{
    Il2CppString* csString = RET_UNLESS("", il2cpp_utils::RunMethod<Il2CppString*>("UnityEngine.SceneManagement", "Scene", "GetNameInternal", scene.m_Handle));
    RET_UNLESS("", csString);
    return to_utf8(csstrtostr(csString));
}