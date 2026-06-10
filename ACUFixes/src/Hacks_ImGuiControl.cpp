#include "pch.h"


#include "MyLog.h"
#include "MainConfig.h"
#include "ImGuiCTX.h"

#include "AutoAssemblerKinda/AutoAssemblerKinda.h"

#include "VariousPatches/Hack_EnterWindowsWhenRisPressed.h"
#include "VariousPatches/Hack_SlowMenacingWalkAndAutowalk.h"
#include "VariousPatches/Hack_CycleEquipmentWhenScrollingMousewheel.h"
#include "VariousPatches/Hack_ModifyAimingFOV.h"
#include "VariousPatches/Hack_DontForceUnsheatheWhenInDisguise.h"
#include "VariousPatches/Hack_CrouchFix.h"
#include "VariousPatches/Hack_ReworkedTakeCover.h"
#include "VariousPatches/Hack_BreakfallAndLedgeGrab.h"
#include "VariousPatches/Hack_EasierTurnWhenSwinging.h"
#include "VariousPatches/Hack_AimFromPeaks.h"
#include "VariousPatches/Hack_NoMoreFailedBombThrows.h"
#include "VariousPatches/Hack_MoreResponsiveBombQuickDrop.h"
#include "VariousPatches/Hack_ReloadRangedWeaponsWhenRefillAllInShop.h"
#include "VariousPatches/Hack_MoreReliableQuickshot.h"
#include "VariousPatches/Hack_DontRandomlyTurn180Degrees.h"
#include "VariousPatches/Hack_NoWaitForUnsafeEject.h"
#include "VariousPatches/Hack_AllowCustomizeEquipment.h"
#include "VariousPatches/Hack_GunRange.h"
#include "VariousPatches/Hack_UnequipPistol.h"
#include "VariousPatches/Hack_CorrectLandingHeightOutOfVault.h"

#include "VariousPatches/Hack_LookbehindButton.h"
#include "VariousPatches/Hack_WhistleAbility.h"
#include "VariousPatches/Hacks_VariousExperiments.h"

#include "VariousPatches/Cheat_BatlampOfFranciade.h"
#include "VariousPatches/Cheat_PretendYoureInFranciade.h"
#include "VariousPatches/Cheat_Health.h"
#include "VariousPatches/Cheat_Ammo.h"
#include "VariousPatches/Cheat_DisguiseUpgrades.h"

#include "VariousPatches/Request_Spindescent.h"
#include "VariousPatches/Request_FreezeFOV.h"

void DrawSlowMotionControls();
void DrawSlowMotionTrickControls();
void DrawHoodControls();
void DrawKeepCursorInsideGameWindowControls();

#include "ImGuiConfigUtils.h"
class MyHacks
{
public:
    AutoAssembleWrapper<EnterWindowWhenRisPressed> enterWindowsByPressingAButton;
    AutoAssembleWrapper<AllowSlowMenacingWalkAndAutowalk> menacingWalkAndAutowalk;
    AutoAssembleWrapper<ModifyConditionalFOVs> changeZoomLevelsWhenAimingBombs;
    AutoAssembleWrapper<InputInjection_CycleEquipmentWhenScrollingMousewheel> cycleEquipmentUsingMouseWheel;
    AutoAssembleWrapper<DontUnsheatheLikeAnIdiotWhileInDisguise> dontUnsheatheWhenInDisguise;
    AutoAssembleWrapper<CrouchFix> slightlyMoreResponsiveCrouch;
    AutoAssembleWrapper<ReworkedTakeCover> takingCoverIsLessSticky;
    AutoAssembleWrapper<LookbehindButton> lookbehindButton;
    AutoAssembleWrapper<BreakfallAndLedgeGrab> breakfallAndCatchLedgeByPressingE;
    AutoAssembleWrapper<EasierTurnWhenSwinging> easierTurnWhenSwingingOnAHorizontalBar;
    AutoAssembleWrapper<MoreSituationsToDropBomb> moreSituationsToDropBombs;
    AutoAssembleWrapper<AimFromPeaks> aimBombsFromPeaks;
    AutoAssembleWrapper<NoMoreFailedBombThrows> noMoreImaginaryBombThrows;
    AutoAssembleWrapper<DontRandomlyTurn180Degrees> dontRandomlyTurn180degrees;
    AutoAssembleWrapper<ReloadRangedWeaponsWhenRefillAllInShop> automaticallyReloadWeaponsWhenRefillAllInShops;
    AutoAssembleWrapper<MoreReliableQuickshot> moreReliableQuickshot;
    AutoAssembleWrapper<NoWaitForUnsafeEject> noWaitForUnsafeEject;
    AutoAssembleWrapper<AllowCustomizeEquipment> allowCustomizeEquipmentOnLedges;
    AutoAssembleWrapper<GuillotineGunRange> guillotineGunRange;
    AutoAssembleWrapper<UnequipPistol> allowUnequipPistol;
    AutoAssembleWrapper<CorrectLandingHeightOutOfVault> correctLandingHeightOutOfVault;

    // Unused and unfinished
    AutoAssembleWrapper<PlayWithFOV> fovGames;
    AutoAssembleWrapper<PlayWithBombAimCameraTracker2> bombAimExperiments2;
    AutoAssembleWrapper<WhistleAbility> whistleAbility;
    AutoAssembleWrapper<BetterAimingFromBehindCover> moreConsistentAimBombFromBehindCover;

    void DrawControls()
    {
        if (ImGui::Button("保存配置文件"))
        {
            WriteConfig(g_Config);
            MainConfig::WriteToFile();
        }
        //ImGui::DrawCheckboxForHack(whistleAbility, "Whistle ability");
        //if (ImGui::IsItemHovered())
        //{
        //    ImGui::SetTooltip("Press Y to _try_to_ attract nearby guards' attention. Very unfinished.");
        //}
        //WhistleAbilityAttempt_ImGuiControls(whistleAbility.IsActive());
        ImGui::DrawCheckboxForHack(enterWindowsByPressingAButton, "按下按钮进入附近窗户");
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip(
                "当在墙上攀爬时，按下指定按键（默认 'R'，与《枭雄》相同）\n"
                "即可进入附近窗户。\n"
                "此外，站在地面上时，也可进入附近的隐藏点\n"
                "（带帘子的衣柜）。"
            );
        }
        if (enterWindowsByPressingAButton.IsActive())
        {
            ImGuiCTX::Indent _indent;
            ImGui::Checkbox("同时进入附近的隐藏点衣柜", &g_Config.hacks->enterWindowsByPressingAButton->alsoEnterNearbyHidespotClosets.get());
            bool isHotkeyChanged = ImGui::DrawEnumPicker(
                "进入窗户按键",
                g_Config.hacks->enterWindowsByPressingAButton->enterWindowsButton.get(),
                ImGuiComboFlags_HeightLarge);
        }
        ImGui::DrawCheckboxForHack(menacingWalkAndAutowalk, "允许自动行走和缓慢霸气步态");
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip(
                "CapsLock 切换缓慢霸气步态；\n"
                "朝任意方向行走时，轻按自动行走键，然后松开\n"
                "方向键，亚诺将继续朝同一方向行走。\n"
                "或者：\n"
                " - 原地站立\n"
                " - 按下并松开自动行走键\n"
                " - 在接下来约一秒内，开始行走并松手。"
            );
        }
        if (menacingWalkAndAutowalk.IsActive())
        {
            ImGuiCTX::Indent _indent;
            bool isHotkeyChanged = ImGui::DrawEnumPicker(
                "自动行走按键",
                g_Config.hacks->menacingWalkAndAutowalk->autowalkButton.get(),
                ImGuiComboFlags_HeightLarge);
        }
        ImGui::DrawCheckboxForHack(moreSituationsToDropBombs, "可在更多情况下投掷炸弹，如跳跃时。请阅读说明。");
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip(
                "允许在更多情况下快速投掷炸弹：跳跃时、挂在墙上时、"
                "\n刺杀时、藏在衣柜中、在横杆上荡秋千时等等。"
                "\n因为这会启动投掷动画（至少涉及左臂），"
                "\n有时动画会看起来不协调"
                "\n（例如在刺杀过程中投掷炸弹，或单手悬挂时），"
                "\n我可能无法让所有动画都完美衔接。"
            );
        }
        if (moreSituationsToDropBombs.IsActive())
        {
            ImGuiCTX::Indent _ind;
            ImGui::Checkbox("战斗中反应更灵敏", &g_Config.hacks->moreSituationsToDropBombs->alsoMoreResponsiveInCombat.get());
            if (ImGui::IsItemHovered())
            {
                ImGui::SetTooltip(
                    "战斗中，允许在执行其他动作时投掷炸弹，"
                    "\n例如格挡时、翻滚时、终结技时。"
                );
            }
        }
        ImGui::DrawCheckboxForHack(moreReliableQuickshot, "更可靠的快速射击。请阅读说明。");
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip(
                "允许从尖顶和V形结构（如旗杆或树上）快速射击腕弩和手枪。"
                "\n同时修复了一个非常奇怪的情况：当你试图快速射击手枪"
                "\n_同时_收起近战武器时，亚诺会尝试装弹，"
                "\n即使弹匣是满的。"
                "\n此外，有望修复那个非常隐蔽、有时会破坏游戏的快速射击 bug："
                "\n当你快速射击后迅速进行跑酷时，"
                "\n你将无法切换远程武器，在边缘时"
                "\n无法行走或转身。"
                "\n建议同时开启「减少快速射击限制」选项。"
            );
        }
        if (moreReliableQuickshot.IsActive())
        {
            ImGuiCTX::Indent _indent;
            ImGui::Checkbox("减少快速射击限制。请阅读说明。", &g_Config.hacks->moreReliableQuickshot->lessQuickshotRestrictions.get());
            if (ImGui::IsItemHovered())
            {
                ImGui::SetTooltip(
                    "通常情况下，快速射击非常容易被中断："
                    "\n撞到桌子、从桌子上走下来、甚至只是撞到墙"
                    "\n（不攀爬）都可能取消动画。"
                    "\n此补丁移除了_大多数_中断，允许你例如"
                    "\n滑过桌子的同时射击，获得大量风格分数。"
                    "\n此外，如果启用了「更多情况下投掷炸弹」补丁，"
                    "\n也允许在大多数那些情况下进行快速射击。例如"
                    "\n跳跃时、翻滚时，甚至挂在墙上时。"
                    "\n这主要适用于手枪和腕弩，因为射击"
                    "\n像步枪这样的双手远程武器挂在墙上毫无意义。"
                    "\n遗憾的是，你仍然无法在边缘使用步枪射击。"
                    "\n请注意：与投掷炸弹动画主要只使用左臂不同，"
                    "\n快速射击动画也涉及角色的躯干。"
                    "\n这意味着动画看起来奇怪的机会更多，"
                    "\n例如下半身做翻滚动作"
                    "\n而上半身却保持快速射击的直立姿势。"
                    "\n试试看，自行判断哪些看起来好。"
                );
            }
        }
        ImGui::DrawCheckboxForHack(changeZoomLevelsWhenAimingBombs, "瞄准炸弹时改变缩放级别");
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip(
                "瞄准炸弹和断头台枪时 FOV（视野）会增大。\n"
                "瞄准炸弹时按鼠标右键可放大。"
            );
        }
        ImGui::DrawCheckboxForHack(cycleEquipmentUsingMouseWheel, "使用鼠标滚轮切换装备");
        ImGui::DrawCheckboxForHack(dontUnsheatheWhenInDisguise, "伪装时不要拔出武器");
        ImGui::DrawCheckboxForHack(slightlyMoreResponsiveCrouch, "略微更灵敏的蹲下按钮");
        ImGui::DrawCheckboxForHack(takingCoverIsLessSticky, "减少掩体吸附感");
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip(
                "默认情况下，只有_刚按下_掩体键（空格键）时才尝试进入掩体。\n"
                "启用此功能后，只要空格键被按住（且保持低位姿态），\n"
                "亚诺将始终尝试进入掩体。\n"
                "此外，朝远离掩体的方向移动可以更容易地脱离掩体。\n"
                "另外，当在掩体后方到达其边缘时，玩家会离开掩体\n"
                "而不是探出身来瞄准拐角处。"
            );
        }
        ImGui::DrawCheckboxForHack(breakfallAndCatchLedgeByPressingE, "更可靠的坠落受身和边缘抓取");
        if (ImGui::IsItemHovered()) {
            ImGui::SetTooltip(
                "当处于不受控的自由落体状态（亚诺挥舞手臂时，"
                "\n例如从很高的地方手动跳跃，"
                "\n或在攀墙时被击落），"
                "\n按住「跑酷下降/交互」键（默认 \"E\"）可抓住"
                "\n前方最近的边缘。\n"
                "如果同时按住方向键，可以抓住不同方向的边缘，"
                "\n只要附近有足够近的边缘。\n"
            );
        }
        ImGui::DrawCheckboxForHack(easierTurnWhenSwingingOnAHorizontalBar, "在横杆上荡秋千时更容易转身");
        if (ImGui::IsItemHovered()) {
            ImGui::SetTooltip(
                "如果你跳上横杆准备荡秋千时按住冲刺+后退，\n"
                "亚诺会改变方向并荡回去。"
            );
        }
        ImGui::DrawCheckboxForHack(aimBombsFromPeaks, "可从「尖顶」或「栖息处」（垂直管道、尖塔等）瞄准炸弹");
        ImGui::DrawCheckboxForHack(noMoreImaginaryBombThrows, "不再出现虚晃的炸弹投掷");
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip(
                "你知道有时候瞄准并投掷炸弹后"
                "\n投掷动画播放了，但实际上没有炸弹被扔出去吗？"
                "\n这种情况不会再发生了。"
            );
        }
        ImGui::DrawCheckboxForHack(dontRandomlyTurn180degrees, "不会随机180度转身");
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip(
                "你是否遇到过这种情况：你蹲着，非常隐秘地移动，"
                "\n当试图做一些小动作时，角色"
                "\n毫无征兆地瞬间转了180度？"
                "\n这至少移除了这种荒唐事的一个原因。"
            );
        }
        ImGui::DrawCheckboxForHack(automaticallyReloadWeaponsWhenRefillAllInShops, "在商店使用「全部补充」时自动装弹");
        DrawHoodControls();
        ImGui::DrawCheckboxForHack(noWaitForUnsafeEject, "取消不安全墙壁后跳等待时间");
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip(
                "在可以执行不安全墙壁后跳的地方，"
                "\n游戏通常会等你停止移动1秒后"
                "\n才允许你这样做。此功能移除了这个等待时间。"
            );
        }
        ImGui::DrawCheckboxForHack(allowCustomizeEquipmentOnLedges, "允许在边缘悬挂时打开装备自定义");
        ImGui::DrawCheckboxForHack(guillotineGunRange, "略微增加断头台枪快速射击范围");
        //ImGui::DrawCheckboxForHack(preventAutomaticReequipPistol, "Don't automatically reequip the pistol after switching from a rifle");
        //if (ImGui::IsItemHovered())
        //{
        //    ImGui::SetTooltip(
        //        "If you want to play without the pistol equipped,"
        //        "this is not as good as an \"Unequip\" button, but it's something."
        //    );
        //}
        ImGui::DrawCheckboxForHack(allowUnequipPistol, "允许卸下手枪");
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip(
                "前往暂停菜单 -> 角色自定义 -> 装备配置 -> 武器 -> 手枪。"
                "\n选择当前装备的手枪（黄色高亮），然后按回车/装备"
                "\n即可卸下。"
            );
        }
        ImGui::DrawCheckboxForHack(correctLandingHeightOutOfVault, "修正翻越后的落地高度");
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip(
                "在某些情况下，亚诺在完成一个小翻越后会出现严重的踉跄。\n"
                "这应该修复了那个 bug。"
            );
        }
        ImGui::DrawCheckboxForHack(lookbehindButton, "向后看按钮");
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip(
                "按下热键时翻转镜头。\n"
                "警告：瞄准断头台枪时无效。"
            );
        }
        if (lookbehindButton.IsActive())
        {
            ImGuiCTX::Indent _ind;
            ImGui::DrawEnumPicker("向后看热键", g_Config.hacks->lookbehindButton->hotkey.get(), ImGuiComboFlags_HeightLarge);
        }
        DrawSlowMotionTrickControls();
        if (g_Config.developmentExtras->showDevelopmentExtras)
        {
            if (ImGuiCTX::TreeNode _header{ "未完成和无用的功能" })
            {
                // This is one of the useless experimental hacks, and has some severe stuttering,
                // but only _sometimes_ for some reason.
                //ImGui::DrawCheckboxForHack(fovGames, "Play with FOV");
                ImGui::DrawCheckboxForHack(bombAimExperiments2, "炸弹瞄准实验2");
                ImGui::DrawCheckboxForHack(moreConsistentAimBombFromBehindCover, "略微宽松的掩体后瞄准炸弹");
                if (ImGui::IsItemHovered())
                {
                    ImGui::SetTooltip(
                        "你是否遇到这种情况：想从拐角处扔炸弹，"
                        "\n但因为没紧贴掩体的_最_边缘，角色反而离开了墙壁？"
                        "\n最终瞄准的几乎是_墙壁_而不是拐角处？"
                        "\n这应该会减少这种情况。"
                        "\n但目前，有些地方反而会变得更糟。"
                    );
                }
            }
        }
        ImGui::Separator();
        ImGui::Text("作弊");
        ImGui::Separator();
        DrawCheatsControls();
        ImGui::Separator();
        if (ImGuiCTX::TreeNode _{ "个人请求" })
        {
            DrawPersonalRequestsControls();
        }
        ImGui::Separator();
        DrawKeepCursorInsideGameWindowControls();
    }
    void DrawCheatsControls()
    {
        {
            ImGui::DrawCheckboxForHack(batlampOfFranciade, "允许在室外使用圣但尼之首");
            if (ImGui::IsItemHovered())
            {
                ImGui::SetTooltip(
                    "请阅读激活后的选项。"
                );
            }
            if (batlampOfFranciade.IsActive())
            {
                DrawBatlampControls();
            }
            Cheat_Health_DrawImGui();
            ImGui::DrawCheckboxForHack(dontDecreaseRemainingAmmo, "无限弹药");
            if (ImGuiCTX::TreeNode _disguiseUpgradesSection{ "伪装升级" })
            {
                ImGui::DrawCheckboxForHack(disableDisguiseCooldown, "禁用伪装冷却时间");
                ImGui::DrawCheckboxForHack(unbreakableDisguise, "不可破坏的伪装");
                if (ImGui::IsItemHovered())
                {
                    ImGui::SetTooltip(
                        "伪装会让你对敌人\"不可见\"，这让不可破坏的伪装\n"
                        "在正常游戏流程中变得相当无聊。\n"
                        "如果你只是想使用别人的外观，可以取消勾选\n"
                        "\"对敌人不可见\"作弊项，这样伪装就不会阻止被发现了。\n"
                        "另外，如果你想在穿着伪装时_战斗_，\n"
                        "可能还需要禁用「伪装时不要拔出武器」这个修复。"
                    );
                }
                ImGui::DrawCheckboxForHack(disguiseDoesntMakeYouInvisible, "启用后，伪装不会让你对敌人不可见");
            }
            Cheat_Invisibility_DrawImGui();
            ImGui::DrawCheckboxForHack(pretendYoureInFranciade, "假装你在法兰西亚德");
            if (ImGui::IsItemHovered())
            {
                ImGui::SetTooltip(
                    "例如，这允许将提灯（包括魔法圣但尼之首）"
                    "\n带入巴黎。\n"
                    "激活前请考虑备份存档。\n"
                    "副作用至少会有一些音效缺失。"
                );
            }
            DrawSlowMotionControls();
        }
    }
    void DrawPersonalRequestsControls()
    {
        ImGui::DrawCheckboxForHack(freezeFOV, "冻结全局 FOV（视野）");
        if (freezeFOV.IsActive())
        {
            DrawFreezeFOVControls();
        }
        ImGui::DrawCheckboxForHack(parkourHelpers, "跑酷辅助");
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip(
                "有人请我让一些华丽的跑酷动作更容易做出来。"
                "\n这些功能不是_修复_，不适用于所有人，但可以很有趣。"
            );
        }
        if (parkourHelpers.IsActive())
        {
            ImGuiCTX::Indent _ind;
            ImGui::Checkbox("旋转下落辅助", &g_Config.personalRequests->parkourHelper->spinningDescentHelper.get());
            if (ImGui::IsItemHovered())
            {
                ImGui::SetTooltip(
                    "简而言之：如果你做以下操作，旋转下落动作更容易触发："
                    "\n在横杆上荡秋千时，按住冲刺+前进+跑酷下降。"
                    "\n详情："
                    "\n有一个非常华丽、非常罕见的跑酷动画，亚诺"
                    "\n在空中做270度旋转后抓住边缘。"
                    "\n它出现在AC大革命E3游戏预告片中，最容易触发的地方"
                    "\n是在巴黎圣母院后半部两侧的横杆石像鬼上荡秋千时，"
                    "\n荡的过程中按住冲刺+前进+跑酷下降。"
                    "\n通常情况下，这个动画的触发条件如下："
                    "\n- 玩家需要从荡秋千（横杆、晾衣绳、"
                    "\n  两栋建筑之间悬挂的绳索等）跑酷到挂墙"
                    "\n  （双脚踩在墙上，而非自由悬垂）。"
                    "\n- 重要的是，玩家需要抓住的墙壁必须大约垂直于"
                    "\n  玩家荡秋千的横杆。"
                    "\n- 从荡秋千到目标的方向必须相当陡峭地向下。"
                    "\n- 最后，跑酷系统不能选择其他\"跑酷动作\"。"
                    "\n最后一点可能是这个动作如此罕见的原因：使用跑酷下降时"
                    "\n通常有更多合适的落地点。"
                    "\n此补丁使得使用跑酷下降荡秋千时，旋转下落被_极大_地优先考虑"
                    "\n——如果可以使用此动作，所有其他\"跑酷动作\"将被忽略。"
                    "\n这并不意味着你可以在_每次_荡秋千时都这样做，"
                    "\n有时动作会看起来不协调。"
                    "\n个人认为，这让这个动作出现得太频繁了，"
                    "\n但如果你在制作那些精彩的跑酷视频，可能会觉得有用。"
                    "\n与其他所有功能一样，这没有在多人游戏中测试过。"
                );
            }
            ImGui::Checkbox("飞扑辅助", &g_Config.personalRequests->parkourHelper->diveHelper->isActive.get());
            if (ImGui::IsItemHovered())
            {
                ImGui::SetTooltip(
                    "当按住一个新热键（默认鼠标5号键），飞扑动作获得比"
                    "\n旋转下落更高的优先级（\"飞扑\"或\"低空跳伞\"是一种高位跑酷跳跃，"
                    "\n让你过渡到双手抓住荡杆/横杆/悬挂绳索/悬垂屋顶"
                    "\n双脚自由悬垂的状态）。"
                    "\n你的其他操作仍然重要。如果你目标横杆在近距离下方，"
                    "\n你需要按冲刺+跑酷下降+前进+飞扑热键。"
                    "\n如果在近距离略高于你或更远的下方，"
                    "\n你需要按冲刺+跑酷上升+前进+飞扑热键。"
                    "\n与未修改的游戏大致相同，你需要判断距离"
                    "\n和适当的\"跑酷模式\"，但现在可以优先使用飞扑。"
                    "\n我对新热键不太满意，但你可以逐渐依赖它，"
                    "\n这个动作已经被请求过两次了。"
                );
            }
            if (g_Config.personalRequests->parkourHelper->diveHelper->isActive.get())
            {
                ImGuiCTX::Indent _ind2;
                ImGui::DrawEnumPicker("飞扑辅助热键", g_Config.personalRequests->parkourHelper->diveHelper->hotkey.get(), ImGuiComboFlags_HeightLarge);
                ImGui::Checkbox("允许墙壁弹跳到悬挂", &g_Config.personalRequests->parkourHelper->diveHelper->allowWallEjectToHang.get());
                {
                    static ImGuiTextBuffer infoAboutWallEjectToHang = []() {
                        ImGuiTextBuffer buf;
                        buf.appendf(
                            "按住飞扑热键时，如果可用，可使用墙壁侧弹悬挂\n"
                            "和墙壁后弹悬挂跑酷动作。\n"
                            "实验性功能。\n"
                            "\n"
                            "发现者：TheManWithNothing\n"
                            "   https://www.youtube.com/@TheManWithNothing\n"
                            "   https://www.youtube.com/watch?v=SjgeA2mUs30\n"
                            "（右键复制到剪贴板）\n"
                            "给任何正在阅读的 mod 制作者的小提示：\n"
                            "只需在 0x14015367f 处 NOP 掉 4 个字节\n"
                            "就已经能获得几乎可用的侧跳悬挂。\n"
                            "Unity 引擎有时就是这样。"
                        );
                        return buf;
                        }();
                    if (ImGui::IsItemHovered())
                        ImGui::SetTooltip(infoAboutWallEjectToHang.c_str());
                    if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
                        ImGui::SetClipboardText(infoAboutWallEjectToHang.c_str());
                }
            }
            ImGui::Checkbox("边缘刺杀后可立即站上边缘", &g_Config.personalRequests->parkourHelper->canRiseOnLedgeAfterLedgeAssassination.get());
        }
    }
    void ReadConfig(ConfigTop& cfg)
    {
        auto& hacksSection = cfg.hacks;
        enterWindowsByPressingAButton.Toggle(hacksSection->enterWindowsByPressingAButton->isActive);
        menacingWalkAndAutowalk.Toggle(hacksSection->menacingWalkAndAutowalk->isActive);
        changeZoomLevelsWhenAimingBombs.Toggle(hacksSection->changeZoomLevelsWhenAimingBombs);
        cycleEquipmentUsingMouseWheel.Toggle(hacksSection->cycleEquipmentUsingMouseWheel);
        dontUnsheatheWhenInDisguise.Toggle(hacksSection->dontUnsheatheWhenInDisguise);
        slightlyMoreResponsiveCrouch.Toggle(hacksSection->slightlyMoreResponsiveCrouch);
        takingCoverIsLessSticky.Toggle(hacksSection->takingCoverIsLessSticky);
        breakfallAndCatchLedgeByPressingE.Toggle(hacksSection->breakfallAndCatchLedgeByPressingE);
        easierTurnWhenSwingingOnAHorizontalBar.Toggle(hacksSection->easierTurnWhenSwingingOnAHorizontalBar);
        aimBombsFromPeaks.Toggle(hacksSection->aimBombsFromPeaks);
        noMoreImaginaryBombThrows.Toggle(hacksSection->noMoreImaginaryBombThrows);
        dontRandomlyTurn180degrees.Toggle(hacksSection->dontRandomlyTurn180degrees);
        moreSituationsToDropBombs.Toggle(hacksSection->moreSituationsToDropBombs->isActive);
        moreReliableQuickshot.Toggle(hacksSection->moreReliableQuickshot->isActive);
        automaticallyReloadWeaponsWhenRefillAllInShops.Toggle(hacksSection->automaticallyReloadWeaponsWhenRefillAllInShops);
        lookbehindButton.Toggle(hacksSection->lookbehindButton->isActive);
        noWaitForUnsafeEject.Toggle(hacksSection->noWaitForUnsafeEject);
        allowCustomizeEquipmentOnLedges.Toggle(hacksSection->allowCustomizeEquipmentOnLedges);
        guillotineGunRange.Toggle(hacksSection->slightlyGreaterGuillotineGunQuickshotRange);
        allowUnequipPistol.Toggle(hacksSection->allowUnequipPistol);
        correctLandingHeightOutOfVault.Toggle(hacksSection->correctLandingHeightOutOfVault);

        auto& cheatsSection = cfg.cheats;
        dontDecreaseRemainingAmmo.Toggle(cheatsSection->infiniteAmmo);

        auto& requestsSection = cfg.personalRequests;
        parkourHelpers.Toggle(requestsSection->parkourHelper->isActive);
        freezeFOV.Toggle(requestsSection->freezeFOV->isActive);
    }
    void WriteConfig(ConfigTop& cfg)
    {
        auto& hacksSection = cfg.hacks;
        hacksSection->enterWindowsByPressingAButton->isActive = enterWindowsByPressingAButton.IsActive();
        hacksSection->menacingWalkAndAutowalk->isActive = menacingWalkAndAutowalk.IsActive();
        hacksSection->changeZoomLevelsWhenAimingBombs = changeZoomLevelsWhenAimingBombs.IsActive();
        hacksSection->cycleEquipmentUsingMouseWheel = cycleEquipmentUsingMouseWheel.IsActive();
        hacksSection->dontUnsheatheWhenInDisguise = dontUnsheatheWhenInDisguise.IsActive();
        hacksSection->slightlyMoreResponsiveCrouch = slightlyMoreResponsiveCrouch.IsActive();
        hacksSection->takingCoverIsLessSticky = takingCoverIsLessSticky.IsActive();
        hacksSection->breakfallAndCatchLedgeByPressingE = breakfallAndCatchLedgeByPressingE.IsActive();
        hacksSection->easierTurnWhenSwingingOnAHorizontalBar = easierTurnWhenSwingingOnAHorizontalBar.IsActive();
        hacksSection->aimBombsFromPeaks = aimBombsFromPeaks.IsActive();
        hacksSection->noMoreImaginaryBombThrows = noMoreImaginaryBombThrows.IsActive();
        hacksSection->dontRandomlyTurn180degrees = dontRandomlyTurn180degrees.IsActive();
        hacksSection->moreSituationsToDropBombs->isActive = moreSituationsToDropBombs.IsActive();
        hacksSection->moreReliableQuickshot->isActive = moreReliableQuickshot.IsActive();
        hacksSection->automaticallyReloadWeaponsWhenRefillAllInShops = automaticallyReloadWeaponsWhenRefillAllInShops.IsActive();
        hacksSection->lookbehindButton->isActive = lookbehindButton.IsActive();
        hacksSection->noWaitForUnsafeEject = noWaitForUnsafeEject.IsActive();
        hacksSection->allowCustomizeEquipmentOnLedges = allowCustomizeEquipmentOnLedges.IsActive();
        hacksSection->slightlyGreaterGuillotineGunQuickshotRange = guillotineGunRange.IsActive();
        hacksSection->allowUnequipPistol = allowUnequipPistol.IsActive();
        hacksSection->correctLandingHeightOutOfVault = correctLandingHeightOutOfVault.IsActive();

        auto& cheatsSection = cfg.cheats;
        cheatsSection->infiniteAmmo = dontDecreaseRemainingAmmo.IsActive();

        auto& requestsSection = cfg.personalRequests;
        requestsSection->parkourHelper->isActive = parkourHelpers.IsActive();
        requestsSection->freezeFOV->isActive = freezeFOV.IsActive();
    }
    AutoAssembleWrapper<BatlampOfFrancide> batlampOfFranciade;
    AutoAssembleWrapper<AmmoCheat> dontDecreaseRemainingAmmo;
    AutoAssembleWrapper<PretendYoureInFranciade> pretendYoureInFranciade;
    AutoAssembleWrapper<DisableDisguiseCooldown> disableDisguiseCooldown;
    AutoAssembleWrapper<UnbreakableDisguise> unbreakableDisguise;
    AutoAssembleWrapper<DisguiseDoesntMakeYouInvisible> disguiseDoesntMakeYouInvisible;

    AutoAssembleWrapper<ParkourActionsExtraProcessing> parkourHelpers;
    AutoAssembleWrapper<FreezeFOV> freezeFOV;
};
std::optional<MyHacks> g_MyHacks;
void WriteAppliedHacksStateToConfig()
{
    if (g_MyHacks)
    {
        g_MyHacks->WriteConfig(g_Config);
    }
}
void DrawHacksControls()
{
    if (g_MyHacks)
    {
        g_MyHacks->DrawControls();
    }
}

#include "MyVariousHacks.h"
void MyVariousHacks_DevExtras_Start();
void MyVariousHacks::Start()
{
    g_MyHacks.emplace();
    MyVariousHacks_DevExtras_Start();
    g_MyHacks->ReadConfig(g_Config);
    g_MyHacks->WriteConfig(g_Config);
    MainConfig::WriteToFile();
}
