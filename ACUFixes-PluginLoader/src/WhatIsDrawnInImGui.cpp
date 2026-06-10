#include "pch.h"

#include "base.h"
#include "ImGuiCTX.h"

#include "PluginLoaderConfig.h"
#include "PluginLoader.h"
#include "ImGuiConfigUtils.h"

#include "Common_Plugins/ACUPlugin.h"

void DrawSuccessfulInjectionIndicatorOverlay()
{
    ImGui::SetNextWindowBgAlpha(0.3f); // Transparent background
    ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_Always);
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoDecoration;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
    window_flags |= ImGuiWindowFlags_NoFocusOnAppearing;
    window_flags |= ImGuiWindowFlags_NoNav;
    window_flags |= ImGuiWindowFlags_NoInputs;

    if (ImGui::Begin("始终显示的叠加层", nullptr, window_flags))
    {
        ImGui::Text(
            "按 %s 打开 ImGui 菜单。"
            "\n热键可以在菜单中或配置文件中更改。"
            "\n此文本可以在菜单的「额外」标签页中禁用。详见 `ACUFixesAndPluginLoader-readme.txt`。"
            , enum_reflection<VirtualKeys>::GetString(g_PluginLoaderConfig.hotkey_ToggleMenu)
            );
    }
    ImGui::End();
}
std::filesystem::path& GetThisDLLAbsolutePath();
void DrawModMenuControls()
{
    ImGui::Text(
        "模组菜单热键。"
        "\n你还可以通过手动编辑"
        "\n`%s` 文件（用文本编辑器）来更改热键。"
        , g_PluginLoaderConfig_Filename
    );
    if (ImGui::Button("在文件资源管理器中打开 DLL 文件夹（含配置文件）"))
    {
        system(("explorer \"" + GetThisDLLAbsolutePath().parent_path().string() + "\"").c_str());
    }
    ImGui::DrawEnumPicker("模组菜单热键", g_PluginLoaderConfig.hotkey_ToggleMenu.get(), ImGuiComboFlags_HeightLarge);
}
void DrawFontControls()
{
    if (ImGui::SliderFloat("字体大小", &g_PluginLoaderConfig.fontSize.get(), 9.0f, 75.0f, "%.3f",
        ImGuiSliderFlags_AlwaysClamp))
    {
        Base::Fonts::SetFontSize(g_PluginLoaderConfig.fontSize);
    }
}

void DrawPluginListControls();
void DrawPluginsWhenMenuOpen();
void DrawPluginsEvenWhenMenuIsClosed();
void CrashLog_CodePatches_DrawControls();
void Base::ImGuiLayer_WhenMenuIsOpen()
{
    static bool enableDemoWindow = false;
    if (enableDemoWindow) {
        ImGui::ShowDemoWindow(&enableDemoWindow);
    }
    DrawPluginsWhenMenuOpen();
    ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_FirstUseEver);
    if (ImGuiCTX::Window _mainWindow{ "ACU 插件加载器", 0, ImGuiWindowFlags_NoFocusOnAppearing })
    {
        if (ImGuiCTX::TabBar _tabbar{ "主窗口标签" })
        {
            if (ImGuiCTX::Tab _mainTab{ "主菜单" })
            {
                DrawPluginListControls();
            }
            if (ImGuiCTX::Tab _extraoptions{ "额外" })
            {
#define PLUGIN_API_VERSION_GET_MAJOR(version) ((version >> 24) & 0xFF)
#define PLUGIN_API_VERSION_GET_MINOR(version) ((version >> 16) & 0xFF)
#define PLUGIN_API_VERSION_GET_MINORER(version) ((version >> 8) & 0xFF)
#define PLUGIN_API_VERSION_GET_MINOREST(version) (version & 0xFF)
                ImGui::Separator();
                ImGui::Text(THIS_DLL_PROJECT_NAME " v%d.%d.%d.%d"
                    , PLUGIN_API_VERSION_GET_MAJOR(g_CurrentPluginAPIversion)
                    , PLUGIN_API_VERSION_GET_MINOR(g_CurrentPluginAPIversion)
                    , PLUGIN_API_VERSION_GET_MINORER(g_CurrentPluginAPIversion)
                    , PLUGIN_API_VERSION_GET_MINOREST(g_CurrentPluginAPIversion)
                );
                ImGui::Separator();
#undef PLUGIN_API_VERSION_GET_MAJOR
#undef PLUGIN_API_VERSION_GET_MINOR
#undef PLUGIN_API_VERSION_GET_MINORER
#undef PLUGIN_API_VERSION_GET_MINOREST
                if (ImGui::Button("保存插件加载器配置"))
                {
                    PluginLoaderConfig::WriteToFile();
                }
                DrawModMenuControls();
                ImGui::Separator();
                ImGui::Checkbox("在左上角显示提示文字", &g_PluginLoaderConfig.imgui_showSuccessfulInjectionIndicator.get());
                ImGui::DrawEnumPicker(
                    "热键：控制台"
                    , g_PluginLoaderConfig.hotkey_ToggleConsole.get()
                    , ImGuiComboFlags_HeightLarge);
                ImGui::Separator();
                DrawFontControls();
                ImGui::Separator();
                ImGui::Checkbox("显示 ImGui 演示窗口", &enableDemoWindow);
                ImGui::Separator();
                ImGui::Checkbox("显示开发者选项", &g_PluginLoaderConfig.developerOptions->isActive.get());
                if (g_PluginLoaderConfig.developerOptions->isActive)
                {
                    ImGui::Checkbox("游戏失去焦点时继续加载（用于无边框窗口，非全屏）", &g_PluginLoaderConfig.developerOptions->continueLoadingGameWhileItIsNotFocused.get());
                    if (ImGui::IsItemHovered())
                        ImGui::SetTooltip("开发时很有用。");
                    CrashLog_CodePatches_DrawControls();
                    ImGui::Checkbox("在游戏主线程开始时显示消息框", &g_PluginLoaderConfig.developerOptions->showMessageBoxAtStartOfMainThread.get());
                    if (ImGui::IsItemHovered())
                        ImGui::SetTooltip(
                            "你可能不需要这个。\n"
                            "如果你正在开发 PluginLoader 本身，并且对\n"
                            "「早期钩子」（在游戏窗口打开之前，\n"
                            "在代码可以安全打补丁之前）特别感兴趣，那么你可能想要\n"
                            "尽早暂停游戏进程。但这么早连接普通调试器\n"
                            "可能会因游戏的代码保护而困难。\n"
                            "如果我在游戏主线程开始时立即显示一个消息框，\n"
                            "那么你至少可以连接 Cheat Engine 的 VEH 调试器。"
                        );
                    ImGui::Separator();
                    ImVec4 color_TextDangerZone(1.0f, 0.4f, 0.4f, 1.0f);
                    const bool isInDangerZone = g_PluginLoaderConfig.developerOptions->canUninjectPluginLoader->isActive;
                    if (isInDangerZone)
                        ImGui::PushStyleColor(ImGuiCol_Text, color_TextDangerZone);
                    ImGui::Checkbox("允许卸载 PluginLoader", &g_PluginLoaderConfig.developerOptions->canUninjectPluginLoader->isActive.get());
                    if (ImGui::IsItemHovered())
                    {
                        ImGui::SetTooltip(
                            "警告：我强烈建议保持此选项关闭。\n"
                            "你不应该卸载 PluginLoader。\n"
                            "这样做很可能会导致游戏崩溃。\n"
                            "此外，由各个插件自行决定\n"
                            "它们是否可以安全卸载。\n"
                            "如果你好奇：目前这种崩溃不可避免的原因是\n"
                            "我的动画可脱下兜帽切换功能。详见代码注释。\n"
                            "如果需要，你仍然可以尝试卸载。"
                        );
                        /*
                        Currently the only reason why I cannot safely uninject the PluginLoader
                        is my Animated Removable Hood Toggle. It works by extending the player's
                        (and all NPCs') Animation Graph and due to certain implementation details
                        I need to maintain the patches that at runtime adjust the indices of the last two
                        RTCP variables of the AtomGraph, the only two of the `entityref` type:
                            539. entityref SyncParticipant1; // 0x324db80/52747136
                            540. entityref SyncSubject; // 0xe35d816d/3814556013
                        Basically, I insert a new RTCP variable `int HoodControlValue` at index 539
                        and displace the two vars mentioned above: SyncParticipant1->540; SyncSubject->541
                        (because "entityrefs" are supposed to be the last ones).
                        So whenever a user of the patched AtomGraph (player or NPC)
                        refers to an "entityref" at index 539, it should be referring to index 540,
                        and this adjustment of the index is what I need to maintain at runtime.
                        A way to regain uninjectability would be to look through the AtomGraph dump
                        and patch every single reference to SyncSubject and SyncParticipant1.
                        A quick look at the dump shows 173 references to SyncParticipant1 and 129
                        to SyncSubject, and there most likely are more that I missed.
                        Yet another way would be to change the "adjust entityref index" patches
                        from C++ to assembly, and not deallocate them upon uninjection,
                        with some way to find "rediscover" these patches upon reinjection.
                        I'm going to declare this a very low priority feature and just accept that
                        the PluginLoader isn't supposed to be uninjected.
                        */
                    }
                    if (g_PluginLoaderConfig.developerOptions->canUninjectPluginLoader->isActive)
                    {
                        ImGuiCTX::Indent _ind;
                        ImGui::DrawEnumPicker(
                            "热键：卸载所有"
                            , g_PluginLoaderConfig.developerOptions->canUninjectPluginLoader->hotkey_UninjectPluginLoader.get()
                            , ImGuiComboFlags_HeightLarge);
                        if (ImGui::IsItemHovered(0))
                        {
                            ImGui::SetTooltip("可用于卸载所有插件和插件加载器本身。");
                        }
                    }
                    if (isInDangerZone)
                        ImGui::PopStyleColor();
                }
            }
            if (g_PluginLoaderConfig.developerOptions->isActive)
            {
                if (ImGuiCTX::Tab _typeInfosTab{ "DX11-BaseHook 变量" })
                {
                    Base::ImGuiDrawBasehookDebug();
                }
            }
        }
    }
}
void EveryFrameBeforeGraphicsUpdate();
#include "ImGuiConsole.h"
void Base::ImGuiLayer_EvenWhenMenuIsClosed()
{
    EveryFrameBeforeGraphicsUpdate();
    if (g_PluginLoaderConfig.imgui_showSuccessfulInjectionIndicator
        && g_ConsoleMode == ConsoleMode::Hidden
        )
        DrawSuccessfulInjectionIndicatorOverlay();
    DrawConsoleIfVisible();
    DrawPluginsEvenWhenMenuIsClosed();
}
