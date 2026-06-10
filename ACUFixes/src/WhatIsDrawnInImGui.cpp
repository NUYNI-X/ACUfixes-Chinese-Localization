#include "pch.h"

#include "vmath/vmath.h"
#include <vmath/vmath_extra.h>

#include "ImGuiCTX.h"
#include "ImGui3D/ImGui3DRenderer.h"

#include "ACU/ACUGetSingletons.h"
#include "ACU/Entity.h"
#include "ACU/RenderValuesHolder.h"
#include "ACU/ManagedPtrs/ManagedPtrs.h"

Vector3f g_VisualizedDebugDirection;
void VisualizeLocationFromClipboard()
{
    Vector3f visualizedLoc = ParseVector3fFromClipboard().value_or(Vector3f());
    ImGui3D::DrawLocationNamed(visualizedLoc, "Vizualized Loc");
}


namespace ImGui
{
inline void CopyToClipboardOnClick(const char* s, const char* fmtTooltip = "点击复制到剪贴板", ...)
{
    va_list args;
    va_start(args, fmtTooltip);
    if (ImGui::IsItemHovered()) ImGui::SetTooltipV(fmtTooltip, args);
    va_end(args);
    if (ImGui::IsItemClicked()) ImGui::SetClipboardText(s);
}
}
void VisualizeCurrentPlayerLocation()
{
    Entity* player = ACU::GetPlayer();
    Vector3f loc = player ? player->GetPosition() : Vector3f();
    ImGui3D::DrawLocationNamed(loc, "Player");
}
void VisualizeDirectionFromClipboard()
{
    g_VisualizedDebugDirection = ParseVector3fFromClipboard().value_or(g_VisualizedDebugDirection);
}
void DrawHacksControls();
void WriteAppliedHacksStateToConfig();
void TypeInfoSystemTests();



void DrawBuiltinDebugCommands();
void DrawPlayerVisualsControls();
void DrawWeatherControls();
void DoKeepCursorInsideGameWindow();
#include "Common_Plugins/Common_PluginSide.h"
void DrawModMenuControls()
{
    if (ImGui::Button("在文件资源管理器中打开 DLL 文件夹（含配置文件）"))
    {
        system(("explorer \"" + GetThisDLLAbsoluteFilepath().parent_path().string() + "\"").c_str());
    }
}

void DrawImGui3DMatricesDebug();
void RequestUnloadThisPlugin();
void DrawAnimationExperiments();
void DrawHacksControls_DevExtras();
#include "MainConfig.h"
#include "ImGuiConfigUtils.h"
#include "Handles.h"
void ImGuiLayer_WhenMenuIsOpen()
{
    DoKeepCursorInsideGameWindow();
#ifdef _DEBUG
    void RequestUnloadThisPlugin(); if (ImGui::Button("卸载此插件")) RequestUnloadThisPlugin();
    ImGui::Separator();
#endif // _DEBUG
        if (ImGuiCTX::TabBar _tabbar{ "主窗口标签" })
        {
            if (ImGuiCTX::Tab _mainTab{ "主菜单" })
            {
                DrawHacksControls();
            }
            if (ImGuiCTX::Tab _mainTab{ "天气" })
            {
                DrawWeatherControls();
            }
            if (ImGuiCTX::Tab _extraoptions{ "额外" })
            {
                if (ImGui::Button("保存配置文件"))
                {
                    WriteAppliedHacksStateToConfig();
                    MainConfig::WriteToFile();
                }
                DrawModMenuControls();
                ImGui::Separator();
                if (ImGui::Button("卸载此插件"))
                {
                    RequestUnloadThisPlugin();
                }
                ImGui::Separator();
                ImGui::Checkbox("显示开发实验功能", &g_Config.developmentExtras->showDevelopmentExtras.get());
                if (ImGui::IsItemHovered(0))
                {
                    ImGui::SetTooltip("这些是我的实验功能，对游戏体验没有任何改善。");
                }
                if (g_Config.developmentExtras->showDevelopmentExtras)
                {
                    ImGui::Checkbox("绘制 3D 标记", &g_Config.developmentExtras->show3DMarkersIfDevelopmentExtrasAreEnabled.get());
                }
            }
            if (!g_Config.developmentExtras->showDevelopmentExtras) return;
            auto DrawDevExtrasTabs = []()
                {
                    if (ImGuiCTX::Tab _3dMarkersTab{ "3D 标记" })
                    {
                        ImGui::Checkbox("绘制 3D 标记", &g_Config.developmentExtras->show3DMarkersIfDevelopmentExtrasAreEnabled.get());
                        ImGui3D::DrawPersistent3DMarkersControls();
                        if (ImGui::Button("可视化剪贴板中的位置"))
                        {
                            VisualizeLocationFromClipboard();
                        }
                        if (ImGui::Button("可视化剪贴板中的方向"))
                        {
                            VisualizeDirectionFromClipboard();
                        }
                        if (ImGui::Button("可视化当前玩家位置"))
                        {
                            VisualizeCurrentPlayerLocation();
                        }
                        if (ImGui::CollapsingHeader("视图投影矩阵调试"))
                        {
                            DrawImGui3DMatricesDebug();
                        }
                    }
                    if (ImGuiCTX::Tab _tab_parkourDebug{ "移动调试" })
                    {
                        DrawHacksControls_DevExtras();
                    }
                    if (ImGuiCTX::Tab _{ "玩家外观" })
                    {
                        DrawPlayerVisualsControls();
                    }
                    if (ImGuiCTX::Tab _mainTab{ "动画工具" })
                    {
                        if (ImGui::Button("传送到巴士底狱顶部"))
                        {
                            if (Entity* player = ACU::GetPlayer())
                            {
                                player->GetPosition() = Vector3f(1200.23f, 150.99f, 39.00f);
                            }
                        }
                        DrawAnimationExperiments();
                    }
                    if (ImGuiCTX::Tab _typeInfosTab{ "类型信息" })
                    {
                        TypeInfoSystemTests();
                    }
                    if (ImGuiCTX::Tab _typeInfosTab{ "内置命令" })
                    {
                        if (ImGuiCTX::WindowChild _{ "调试命令" })
                        {
                            DrawBuiltinDebugCommands();
                        }
                    }
                    if (ImGuiCTX::Tab _tab_handles{ "句柄" })
                    {
                        extern const char* g_HandlesMapFilename;
                        ImGui::Text(
                            "部分开发额外功能会显示文件的句柄（Handle，即内部编号）\n"
                            "例如：动画日志、动画图表导出、实体视觉组件等。\n"
                            "如果你在插件目录中放了 \"%s\" 文件\n"
                            "（可在 ACUFixes GitHub 页面的 Releases 部分获取），\n"
                            "那么与这些句柄对应的文件名\n"
                            "也会显示（如果可识别的话）。\n"
                            "例如，如果有上述文件，你应该看到\n"
                            "64839213519 => \n"
                            "变成\n"
                            "64839213519 => ACU_Paris\\ACU_Paris.World"
                            , g_HandlesMapFilename
                        );
                        static uint64 handleToSearch = 64839213519; // => ACU_Paris\ACU_Paris.World
                        ImGui::InputScalar("搜索句柄（十进制）", ImGuiDataType_U64, &handleToSearch, 0, 0, "%llu", ImGuiInputTextFlags_CharsDecimal);
                        ImGui::InputScalar("搜索句柄（十六进制）", ImGuiDataType_U64, &handleToSearch, 0, 0, "%llX", ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase);
                        static ImGuiTextBuffer buf;
                        buf.clear();
                        buf.appendf(
                            "%llu => %s"
                            , handleToSearch
                            , ACU::Handles::HandleToText(handleToSearch)
                        );
                        ImGui::Text(buf.c_str());
                        ImGui::CopyToClipboardOnClick(buf.c_str());
                        ACU::WeakRef<ManagedObject> weakRef(handleToSearch);
                        buf.clear();
                        buf.appendf(
                            "地址:     %llX\n"
                            "共享块:   %llX\n"
                            "强引用:   %u\n"
                            "弱引用:   %u"
                            , weakRef.GetSharedBlock().GetPtr()
                            , &weakRef.GetSharedBlock()
                            , weakRef.GetSharedBlock().GetRefcountStrong()
                            , weakRef.GetSharedBlock().weakRefCount
                        );
                        ImGui::Text(buf.c_str());
                        ImGui::CopyToClipboardOnClick(buf.c_str());
                    }
                };
            if (ImGuiCTX::Tab _tab_devExtras{ "开发额外" })
            {
                if (ImGuiCTX::TabBar _tabbar_withinDevExtrasTab{ "DevExtrasTabs" })
                {
                    DrawDevExtrasTabs();
                }
            }
        }
}

ImGui3D::World2ScreenParams CalculateWorld2ScreenParametersForCurrentFrame();
void DoSlowMotionTrick();
void DoManualHoodControls();
void ShowHumanStatesLogIfNeeded();
#include "AnimationTools/MyAnimationPlayer.h"
void ImGuiLayer_EvenWhenMenuIsClosed()
{
    DoKeepCursorInsideGameWindow();
    g_MyAnimationPlayer.UpdateAnimations();
    ShowHumanStatesLogIfNeeded();
    DoSlowMotionTrick();
    DoManualHoodControls();
    bool drawImGui3D =
        g_Config.developmentExtras->showDevelopmentExtras
        && g_Config.developmentExtras->show3DMarkersIfDevelopmentExtrasAreEnabled;
    if (drawImGui3D)
        ImGui3D::Draw3DLayer(CalculateWorld2ScreenParametersForCurrentFrame());
}
