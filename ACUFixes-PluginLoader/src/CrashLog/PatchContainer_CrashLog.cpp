#include "pch.h"

#include "Hook_ZwRaiseException.h"
#include "MyLog.h"

#include "AutoAssemblerKinda/AutoAssemblerKinda.h"
#include "PluginLoaderConfig.h"
#include "ImGuiConfigUtils.h"

void CrashLog_TestVariousWaysOfCrashing_DrawImGui()
{
    if (ImGui::Button("崩溃测试: 写入空指针"))
    {
        // Depending on the method that Present() was hooked,
        // the UnhandledExceptionFilter() might not catch this,
        // but ZwRaiseException() hook can.
        volatile int* x = nullptr;
        *x = 2;
    }
    if (ImGui::Button("崩溃测试: 抛出 C++ 异常"))
    {
        // Depending on the method that Present() was hooked,
        // the UnhandledExceptionFilter() might not catch this,
        // but ZwRaiseException() hook can.
        std::optional<int> x;
        int y = x.value();
    }
    if (ImGui::Button("崩溃测试(游戏线程): 写入空指针到游戏全局"))
    {
        // Caught by UnhandledExceptionFilter() only if NOT under a debugger.
        // Caught by ZwRaiseException() only if under a debugger.
        *(uintptr_t*)0x14521CA40 = 0;
    }
    if (ImGui::Button("崩溃测试(游戏线程): 设置UEF并写入空指针"))
    {
        SetUnhandledExceptionFilter([](::_EXCEPTION_POINTERS* exc) -> LONG
            {
                MessageBoxA(NULL, "Just set the UEF; crash from gamethread", "崩溃日志", MB_OK | MB_ICONSTOP);
                return EXCEPTION_CONTINUE_SEARCH;
            });
        *(uintptr_t*)0x14521CA40 = 0;
    }
    if (ImGui::Button("崩溃测试(游戏线程): 设置UEF并写入空指针(调用上一个处理器)"))
    {
        static LPTOP_LEVEL_EXCEPTION_FILTER prevHandler = nullptr;
        prevHandler = SetUnhandledExceptionFilter([](::_EXCEPTION_POINTERS* exc) -> LONG
            {
                MessageBoxA(NULL, "Just set the UEF; crash from gamethread", "崩溃日志", MB_OK | MB_ICONSTOP);
                return prevHandler(exc);
            });
        *(uintptr_t*)0x14521CA40 = 0;
    }
    //if (ImGui::Button("CRASH ACUFIXES SPINTHREAD: Write to nullptr"))
    //{
    //    // In the PluginLoader thread that sleeps in a loop, write to nullptr if
    //    // if this bool is true.
    //    extern bool g_DoCrashSpinthread; g_DoCrashSpinthread = true;
    //}
    if (ImGui::Button("不崩溃: 抛出并捕获 C++ 异常"))
    {
        try
        {
            int y = std::optional<int>().value();
        }
        catch (const std::bad_optional_access&)
        {
            LOG_DEBUG(DefaultLogger,
                "[CrashLog_TestVariousWaysOfCrashing] Exception manually thrown and safely caught using C++."
            );
        }
    }
    if (ImGui::Button("不崩溃: 抛出并使用SEH捕获"))
    {
        auto SEHTest = []()
            {
                __try
                {
                    int* x = 0;
                    *x = 42;
                }
                __except (EXCEPTION_EXECUTE_HANDLER)
                {
                    LOG_DEBUG(DefaultLogger,
                        "[CrashLog_TestVariousWaysOfCrashing] Exception manually thrown and safely caught using SEH."
                    );
                }
            };
        SEHTest();
    }
}

class PatchContainer_CrashLog
{
public:
    AutoAssembleWrapper<Hook_ZwRaiseException> zwExcHook;
};
std::optional<PatchContainer_CrashLog> g_PatchContainer_CrashLog;
void CrashLog_CodePatches_Start()
{
    g_PatchContainer_CrashLog.emplace();
    if (g_PluginLoaderConfig.developerOptions->hungrierCrashLog_hookZwRaiseException)
        g_PatchContainer_CrashLog->zwExcHook.Activate();
}
void CrashLog_CodePatches_DrawControls()
{
    if (!g_PatchContainer_CrashLog) return;
    ImGui::DrawCheckboxForHack(g_PatchContainer_CrashLog->zwExcHook, "更强的崩溃日志");
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(
            "额外挂钩 ntdll.ZwRaiseException().\n"
            "它允许崩溃日志捕获更多种类的崩溃,\n"
            "但附加调试器时可能不兼容.\n"
            "如果试图追踪某个静默崩溃的原因，可临时启用\n"
            "此功能."
        );
    }
}
