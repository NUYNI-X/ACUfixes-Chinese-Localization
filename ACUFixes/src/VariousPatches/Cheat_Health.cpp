#include "pch.h"

#include "Cheat_Health.h"

#include "ACU/ACUGetSingletons.h"
#include "ACU/CSrvPlayerHealth.h"

void Cheat_Health_DrawImGui()
{
    CSrvPlayerHealth* health = ACU::GetPlayerHealth();
    bool isDesyncNow = false;
    bool isGodmode = false;
    if (health)
    {
        isDesyncNow = health->isDesynchronizationNow & 1;
        isGodmode = health->bGodmode & 1;
        if (ImGui::Checkbox("无敌模式", &isGodmode))
        {
            health->bGodmode = isGodmode;
        }
        if (ImGui::Checkbox("立即失去同步（死亡）", &isDesyncNow))
        {
            health->isDesynchronizationNow = isDesyncNow;
        }
    }
}
