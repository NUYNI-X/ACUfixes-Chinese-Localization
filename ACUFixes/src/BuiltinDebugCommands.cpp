#include "pch.h"

#include "ACU/CLAssassin.h"
#include "ACU/DebugCommandsHolder.h"
#include "ACU/Memory/ACUAllocs.h"
#include "ACU/World.h"
#include "ACU_DefineNativeFunction.h"
#include "MyLog.h"

static DEFINE_LOGGER_CONSOLE_AND_FILE(BuiltinsLog, "[" THIS_DLL_PROJECT_NAME "][BuiltinCommands]");

DebugCommandsHolder* GetCheatsHolder()
{
    CLAssassin* cla = CLAssassin::GetSingleton();
    if (!cla) { return nullptr; }
    return cla->cheatCodes;
}
void DrawBuiltinDebugCommands()
{
    ImGui::Text(
        "大多数命令不起作用。\n"
        "警告！\"消灭敌人\"/\"消灭盟友\"会导致游戏崩溃。"
    );
    DebugCommandsHolder* cheatsHolder = GetCheatsHolder();
    if (!cheatsHolder) { return; }
    static std::string buf;
    buf.reserve(200);
    for (int i = 0; i < cheatsHolder->arrDebugCommands.size; i++)
    {
        DebugCommand* cheat = cheatsHolder->arrDebugCommands[i];
        buf.clear();
        buf.append(std::to_string(i));
        if (cheat->textDesc)
        {
            buf += ": ";
            buf += cheat->textDesc;
        }
        if (ImGui::Button(buf.c_str()))
        {
            // "Nuke Enemies"/"Nuke Allies" want a1==2. (But they crash anyways because of a nullptr read).
            // Using a1==0 doesn't seems to not have any effect except a string output written to the 3rd argument (sometimes).
            constexpr int mostCommandsSeemToRequireTheFirstParameterToBe1 = 1;
            char* strOut = nullptr;
            cheat->fnExecute(mostCommandsSeemToRequireTheFirstParameterToBe1, cheatsHolder, &strOut);
            if (strOut)
            {
                LOG_DEBUG(BuiltinsLog,
                    "Output from \"%s\": %s"
                    , buf.c_str()
                    , strOut
                );
                ACU::Memory::ACUDeallocateBytes((byte*)strOut);
            }
        }
    }
}
