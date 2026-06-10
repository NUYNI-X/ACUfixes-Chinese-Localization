#include "pch.h"

#include "AutoAssemblerKinda/AutoAssemblerKinda.h"

#include "ImGuiCTX.h"
#include "MainConfig.h"
#include "ImGuiConfigUtils.h"

#include "ParkourDebugging/ParkourDebuggingPatch.h"
#include "ParkourDebugging/LoggingTheHumanStates.h"
#include "ParkourDebugging/Hack_PickEntityToShoot.h"
#include "AnimationTools/AnimationsLog/AnimationGraphEvaluationLog.h"


class MyHacksContainer_DevExtras
{
public:
    AutoAssembleWrapper<ParkourDebuggingPatch> parkourDebugging;
    AutoAssembleWrapper<EnterHumanStateHook> humanStatesHook;
    AutoAssembleWrapper<AnimGraphEvaluationLogging> animGraphDebugging;
    AutoAssembleWrapper<PickEntityToShoot> pickEntityToShoot;

    void DrawControls()
    {
        ImGui::DrawCheckboxForHack(animGraphDebugging, "动画图表调试");
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip(
                "每当玩家角色开始播放动画时，"
                "\nImGui 控制台中会添加一条日志，包含"
                "\n对应 AtomAnimationDataBaseNode 的地址、动画的地址、动画的句柄"
                "\n和文件名（如果已加载句柄字典"
                "\n且句柄可识别——详见「句柄」标签页）。"
                "\n这有助于找出动画状态转换在何时以及如何发生。"
                "\n动画可能在动画图表中被引用多次，"
                "\n但如果你在同一游戏会话中使用「将动画图表导出为文本文件」功能，"
                "\n并搜索 AtomAnimationDataBaseNode 的地址，"
                "\n就能准确看到涉及哪些状态节点。"
            );
        }
        ImGui::Separator();
        ImGui::DrawCheckboxForHack(humanStatesHook, "记录玩家的「人体状态」");
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip(
                "记录玩家「人体状态」的树状结构。\n"
                "不同状态包含不同的回调，这些"
                "\n是使 ACUFixes 的许多功能得以实现的关键。"
                "\n我认为实时观察状态树如何响应"
                "\n玩家的操作会很有启发。"
            );
        }
        if (humanStatesHook.IsActive())
        {
            ImGui::SameLine();
            DrawHumanStatesDebugControls();
        }
        ImGui::Separator();
        ImGui::DrawCheckboxForHack(parkourDebugging, "跑酷调试");
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip(
                "将识别到的「跑酷动作」选择信息"
                "\n记录到 ImGui 控制台。"
                "\n如果启用了 3D 标记功能，还会可视化"
                "\n被考虑的跑酷动作目标位置。"
            );
        }
        ImGui::Separator();
        ImGui::DrawCheckboxForHack(pickEntityToShoot, "手动选择快速射击目标");
        if (pickEntityToShoot.IsActive())
        {
            RaycastPicker_PickRangedWeaponTarget();
        }
    }
};
std::optional<MyHacksContainer_DevExtras> g_MyHacks_DevExtras;

void DrawHacksControls_DevExtras()
{
    if (g_MyHacks_DevExtras)
    {
        g_MyHacks_DevExtras->DrawControls();
    }
}

void MyVariousHacks_DevExtras_Start()
{
    g_MyHacks_DevExtras.emplace();
}
