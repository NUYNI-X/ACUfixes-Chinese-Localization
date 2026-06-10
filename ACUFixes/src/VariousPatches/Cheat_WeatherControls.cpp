#include "pch.h"

#include "ACU/WeatherControls.h"
#include "ACU/TimeOfDayManager.h"
#include "ACU/Season.h"

#include "ACU_DefineNativeFunction.h"

class ActionWeatherWindSelection;
DEFINE_GAME_FUNCTION(ActionWeatherWindSelection__SetWeather_mb, 0x141EC0A60, void, __fastcall, (ActionWeatherWindSelection* a1, char a2));

class Fake_ActionWeatherWindSelection
{
public:
    char pad0[0x40] = { 0 };

    /*
    If using the Action sliders, flt_44 and flt_48 need to be changed simultaneously for nonungly nonflickering nonconfusing results
    When combined, and with byte_68 and byte_69 enabled, they are the "cloudiness+rain" slider.
    If the value in [0, 0.8), there is no rain + cannot edit cloudiness directly.
    The range of [0.8, 1] controls the rain strength + the cloudiness can be controlled directly.
    */

    uint32 dword_40; //0x0040
    float flt_44; //0x0044 // cloudiness+rain TO
    float flt_48; //0x0048 // cloudiness+rain FROM
    float flt_4C; //0x004C // cloudiness+rain transition duration 1
    float flt_50; //0x0050
    float flt_54; //0x0054 // highest dryest spots wetness (includes the player's wetness)
    float flt_58; //0x0058 // lowest wettest spots wetness (puddles size)
    float flt_5C; //0x005C // wind strength
    float flt_60; //0x0060 // cloudiness+rain transition duration 2
    float flt_64; //0x0064
    uint8 byte_68; //0x0068 // doControlCloudinessPlusRain_1
    uint8 byte_69; //0x0069 // doControlCloudinessPlusRain_2
    uint8 byte_6A; //0x006A
    uint8 byte_6B; //0x006B // use flt_54==doControlDrySpots
    uint8 byte_6C; //0x006C // use flt_58==doControlPuddles
    uint8 byte_6D; //0x006D
    uint8 byte_6E; //0x006E
    uint8 byte_6F; //0x006F // use flt_5C==windStrength (flags and vegetation)
    uint8 byte_70; //0x0070 // use flt_64
    uint8 byte_71; //0x0071
    uint8 byte_72; //0x0072
    uint8 byte_73; //0x0073
    uint8 byte_74; //0x0074
    uint8 byte_75; //0x0075
    uint8 byte_76; //0x0076
    uint8 byte_77; //0x0077
    float flt_78; //0x0078 // prev flt_5C
    float flt_7C; //0x007C
    float flt_80; //0x0080 // prev flt_64
    float flt_84; //0x0084
    float flt_88; //0x0088 // prev flt_58
    uint32 dword_8C; //0x008C
}; //Size: 0x0090
assert_sizeof(Fake_ActionWeatherWindSelection, 0x90);
void Weather_Clear()
{
    Fake_ActionWeatherWindSelection fakeWeatherSetter;
    fakeWeatherSetter.dword_40 = 1;
    fakeWeatherSetter.flt_44 = 0;
    fakeWeatherSetter.flt_48 = 0;
    fakeWeatherSetter.flt_4C = 0;
    fakeWeatherSetter.flt_50 = 10.0f;
    fakeWeatherSetter.flt_54 = 0;
    fakeWeatherSetter.flt_58 = 0;
    fakeWeatherSetter.flt_5C = 0.1f;
    fakeWeatherSetter.flt_60 = 0;
    fakeWeatherSetter.flt_64 = 10.000f;
    fakeWeatherSetter.byte_68 = 1;
    fakeWeatherSetter.byte_69 = 1;
    fakeWeatherSetter.byte_6A = 1;
    fakeWeatherSetter.byte_6B = 1;
    fakeWeatherSetter.byte_6C = 1;
    fakeWeatherSetter.byte_6D = 0;
    fakeWeatherSetter.byte_6E = 1;
    fakeWeatherSetter.byte_6F = 1;
    fakeWeatherSetter.byte_70 = 0;
    fakeWeatherSetter.byte_71 = 0;
    fakeWeatherSetter.byte_72 = 0;
    fakeWeatherSetter.byte_73 = 0;
    fakeWeatherSetter.byte_74 = 0;
    fakeWeatherSetter.byte_75 = 0;
    fakeWeatherSetter.byte_76 = 0;
    fakeWeatherSetter.byte_77 = 0;
    fakeWeatherSetter.flt_78 = 0;
    fakeWeatherSetter.flt_7C = 0;
    fakeWeatherSetter.flt_80 = 0;
    fakeWeatherSetter.flt_84 = 0;
    fakeWeatherSetter.flt_88 = 0;
    fakeWeatherSetter.dword_8C = 1;
    ActionWeatherWindSelection__SetWeather_mb((ActionWeatherWindSelection*)&fakeWeatherSetter, 1);
}
void Weather_ModeratelyCloudy()
{
    // When in paris, this removes rain and sets moderately clear sky.

    Fake_ActionWeatherWindSelection fakeWeatherSetter;
    fakeWeatherSetter.dword_40 = 1;
    fakeWeatherSetter.flt_44 = 0.7179999948f;
    fakeWeatherSetter.flt_48 = 0.7179999948f;
    fakeWeatherSetter.flt_4C = 0;
    fakeWeatherSetter.flt_50 = 10.0f;
    fakeWeatherSetter.flt_54 = 0;
    fakeWeatherSetter.flt_58 = 0;
    fakeWeatherSetter.flt_5C = 0.1f;
    fakeWeatherSetter.flt_60 = 0.0f;
    fakeWeatherSetter.flt_64 = 10.0f;
    fakeWeatherSetter.byte_68 = 1;
    fakeWeatherSetter.byte_69 = 1;
    fakeWeatherSetter.byte_6A = 1;
    fakeWeatherSetter.byte_6B = 1;
    fakeWeatherSetter.byte_6C = 1;
    fakeWeatherSetter.byte_6D = 0;
    fakeWeatherSetter.byte_6E = 1;
    fakeWeatherSetter.byte_6F = 1;
    fakeWeatherSetter.byte_70 = 0;
    fakeWeatherSetter.byte_71 = 0;
    fakeWeatherSetter.byte_72 = 0;
    fakeWeatherSetter.byte_73 = 1;
    fakeWeatherSetter.byte_74 = 1;
    fakeWeatherSetter.byte_75 = 0;
    fakeWeatherSetter.byte_76 = 0;
    fakeWeatherSetter.byte_77 = 0;
    fakeWeatherSetter.flt_78 = 0.3f;
    fakeWeatherSetter.flt_7C = 0.81f;
    fakeWeatherSetter.flt_80 = 0;
    fakeWeatherSetter.flt_84 = 0.4f;
    fakeWeatherSetter.flt_88 = 0.2f;
    fakeWeatherSetter.dword_8C = 1;

    ActionWeatherWindSelection__SetWeather_mb((ActionWeatherWindSelection*)&fakeWeatherSetter, 1);
}
void Weather_RainSmugglersParadise()
{
    Fake_ActionWeatherWindSelection fakeWeatherSetter;
    fakeWeatherSetter.dword_40 = 1;
    fakeWeatherSetter.flt_44 = 0.850f;
    fakeWeatherSetter.flt_48 = 0.850f;
    fakeWeatherSetter.flt_4C = 0;
    fakeWeatherSetter.flt_50 = 10.0f;
    fakeWeatherSetter.flt_54 = 0.600f;
    fakeWeatherSetter.flt_58 = 0.800f;
    fakeWeatherSetter.flt_5C = 0.350f;
    fakeWeatherSetter.flt_60 = 0.0f;
    fakeWeatherSetter.flt_64 = 10.0f;
    fakeWeatherSetter.byte_68 = 1;
    fakeWeatherSetter.byte_69 = 1;
    fakeWeatherSetter.byte_6A = 1;
    fakeWeatherSetter.byte_6B = 1;
    fakeWeatherSetter.byte_6C = 1;
    fakeWeatherSetter.byte_6D = 1;
    fakeWeatherSetter.byte_6E = 1;
    fakeWeatherSetter.byte_6F = 1;
    fakeWeatherSetter.byte_70 = 0;
    fakeWeatherSetter.byte_71 = 0;
    fakeWeatherSetter.byte_72 = 0;
    fakeWeatherSetter.byte_73 = 0;
    fakeWeatherSetter.byte_74 = 0;
    fakeWeatherSetter.byte_75 = 0;
    fakeWeatherSetter.byte_76 = 0;
    fakeWeatherSetter.byte_77 = 0;
    fakeWeatherSetter.flt_78 = 0;
    fakeWeatherSetter.flt_7C = 0;
    fakeWeatherSetter.flt_80 = 0;
    fakeWeatherSetter.flt_84 = 0;
    fakeWeatherSetter.flt_88 = 0;
    fakeWeatherSetter.dword_8C = 0;

    ActionWeatherWindSelection__SetWeather_mb((ActionWeatherWindSelection*)&fakeWeatherSetter, 1);
}
void Weather_RainBellecFight()
{
    Fake_ActionWeatherWindSelection fakeWeatherSetter;
    fakeWeatherSetter.dword_40 = 1;
    fakeWeatherSetter.flt_44 = 1.0f;
    fakeWeatherSetter.flt_48 = 1.0f;
    fakeWeatherSetter.flt_4C = 0;
    fakeWeatherSetter.flt_50 = 10.0f;
    fakeWeatherSetter.flt_54 = 0.8f;
    fakeWeatherSetter.flt_58 = 1.0f;
    fakeWeatherSetter.flt_5C = 0.350f;
    fakeWeatherSetter.flt_60 = 0.0f;
    fakeWeatherSetter.flt_64 = 1.0f;
    fakeWeatherSetter.byte_68 = 1;
    fakeWeatherSetter.byte_69 = 1;
    fakeWeatherSetter.byte_6A = 1;
    fakeWeatherSetter.byte_6B = 1;
    fakeWeatherSetter.byte_6C = 1;
    fakeWeatherSetter.byte_6D = 1;
    fakeWeatherSetter.byte_6E = 1;
    fakeWeatherSetter.byte_6F = 1;
    fakeWeatherSetter.byte_70 = 1;
    fakeWeatherSetter.byte_71 = 0;
    fakeWeatherSetter.byte_72 = 0;
    fakeWeatherSetter.byte_73 = 0;
    fakeWeatherSetter.byte_74 = 0;
    fakeWeatherSetter.byte_75 = 0;
    fakeWeatherSetter.byte_76 = 0;
    fakeWeatherSetter.byte_77 = 0;
    fakeWeatherSetter.flt_78 = 0;
    fakeWeatherSetter.flt_7C = 1.0f;
    fakeWeatherSetter.flt_80 = 0;
    fakeWeatherSetter.flt_84 = 20.0f;
    fakeWeatherSetter.flt_88 = 0;
    fakeWeatherSetter.dword_8C = 0;

    ActionWeatherWindSelection__SetWeather_mb((ActionWeatherWindSelection*)&fakeWeatherSetter, 1);
}
void LetTheWeatherGo()
{
    auto& weather = WeatherControls::GetSingleton();
    weather.semaphoreMb_dontRerandomizeCloudiness = 0;
    weather.semaphoreMb_freezeWind = 0;
    weather.int70_needsToBeLe0ForRain = 0;
    weather.semaphoreMb_groundWetness = 0;
}


DEFINE_GAME_FUNCTION(TimeOfDayManager__SetCurrentSeason_mb, 0x141E2F2F0, void, __fastcall, (TimeOfDayManager* a1, unsigned int seasonID));
void SetTimeOfDay(SeasonID seasonID)
{
    TimeOfDayManager* todMan = TimeOfDayManager::GetSingleton();
    if (!todMan) { return; }
    TimeOfDayManager__SetCurrentSeason_mb(todMan, (unsigned int)seasonID);
}


enum class WorldSection
{
    Paris = 1,
    Versailles = 2,
    Franciade = 3,
};
enum class KnownTimeOfDayHandles : uint64
{
    Paris_Morning = 0x7F5F1BB41,               // PAR_TOD_Paris_Dawn/PAR_TOD_Paris_Dawn.Season
    Paris_Day = 0x7F5F1BC0B,                   // PAR_TOD_Paris_Noon/PAR_TOD_Paris_Noon.Season
    Paris_Evening = 0x7F5F1BC07,               // PAR_TOD_Paris_Sunset/PAR_TOD_Paris_Sunset.Season
    Paris_Night = 0x7F5F1BC03,                 // PAR_TOD_Paris_Night/PAR_TOD_Paris_Night.Season
    Versailles_Morning = 0x10CDE128B3,         // VERS_TOD_Dawn/VERS_TOD_Dawn.Season
    Versailles_Day = 0x10CDE128BF,             // VERS_TOD_Noon/VERS_TOD_Noon.Season
    Versailles_Evening = 0x10CDE128C5,         // VERS_TOD_Sunset/VERS_TOD_Sunset.Season
    Versailles_Night = 0x10CDE128B9,           // VERS_TOD_Night/VERS_TOD_Night.Season
    Franciade_Dayish = 0x12F8374554,           // SDN_Season/SDN_Season.Season
    Franciade_Nightish = 0x13786FCCA1,         // SDN_moon_Season/SDN_moon_Season.Season

    OccupiedParis_Unknown0 = 0x61765A1F0,      // ACU_Generic_Season/ACU_Generic_Season.Season
    OccupiedParis_Night = 0x110E816873,        // LGS_WWR_AP_Night/LGS_WWR_AP_Night.Season
};
std::array<KnownTimeOfDayHandles, 4> g_KnownSeasons_Paris = {
    KnownTimeOfDayHandles::Paris_Morning,
    KnownTimeOfDayHandles::Paris_Day,
    KnownTimeOfDayHandles::Paris_Evening,
    KnownTimeOfDayHandles::Paris_Night,
};
std::array<KnownTimeOfDayHandles, 4> g_KnownSeasons_Versailles = {
    KnownTimeOfDayHandles::Versailles_Morning,
    KnownTimeOfDayHandles::Versailles_Day,
    KnownTimeOfDayHandles::Versailles_Evening,
    KnownTimeOfDayHandles::Versailles_Night,
};
std::array<KnownTimeOfDayHandles, 2> g_KnownSeasons_Franciade = {
    KnownTimeOfDayHandles::Franciade_Dayish,
    KnownTimeOfDayHandles::Franciade_Nightish,
};
std::optional<WorldSection> WhereIsPlayerNow()
{
    auto* todMan = TimeOfDayManager::GetSingleton();
    if (!todMan) { return {}; }
    auto* currentSeason = todMan->shared_currentSeason;
    if (!currentSeason) { return {}; }
    uint64 currentSeasonHandle = currentSeason->handle;
    for (KnownTimeOfDayHandles knownSeason : g_KnownSeasons_Paris)
    {
        if (currentSeasonHandle == (uint64)knownSeason)
        {
            return WorldSection::Paris;
        }
    }
    for (KnownTimeOfDayHandles knownSeason : g_KnownSeasons_Versailles)
    {
        if (currentSeasonHandle == (uint64)knownSeason)
        {
            return WorldSection::Versailles;
        }
    }
    for (KnownTimeOfDayHandles knownSeason : g_KnownSeasons_Franciade)
    {
        if (currentSeasonHandle == (uint64)knownSeason)
        {
            return WorldSection::Franciade;
        }
    }
    return {};
}

#include "ImGuiCTX.h"
void DrawWeatherControls()
{
    std::optional<WorldSection> recognizedCurrentLocation = WhereIsPlayerNow();
    if (!recognizedCurrentLocation)
    {
        ImGui::Text(
            "你处于不支持的位置。"
            "\n天气滑块可能有效，也可能无效。"
        );
        WeatherControls& weatherSettings = WeatherControls::GetSingleton();
        if (ImGui::SliderFloat("阴天程度", &weatherSettings.cloudinessCurrent, 0.0f, 1.0f))
        {
            weatherSettings.cloudinessFrom = weatherSettings.cloudinessTo = weatherSettings.cloudinessCurrent;
        }
        if (ImGui::SliderFloat("雨量", &weatherSettings.rainStrengthCurrent, 0.0f, 1.0f))
        {
            weatherSettings.rainStrengthFrom = weatherSettings.rainStrengthTo = weatherSettings.rainStrengthCurrent;
        }
        ImGui::SliderFloat("干燥区域", &weatherSettings.drySpotsWetness, 0.0f, 1.0f);
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip("最干燥区域的湿度，也影响玩家衣物");
        }
        ImGui::SliderFloat("水坑大小", &weatherSettings.puddlesSize, 0.0f, 1.0f);
        ImGui::SliderFloat("风力（可能）", &weatherSettings.windStrength, 0.0f, 1.0f);
        return;
    }
    ImGui::Text("时间。悬停查看说明。");
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(
            "过场动画中的光照是针对特定时间调整的，所以我建议"
            "\n不要在过场动画中更改时间。"
            "\n另外，在法兰西亚德地下时更改时间，"
            "\n回到地面时会产生奇怪或糟糕的结果。"
        );
    }
    ImGui::Separator();

    if (*recognizedCurrentLocation == WorldSection::Paris
        || *recognizedCurrentLocation == WorldSection::Versailles)
    {

        if (ImGui::Button("早晨"))
            SetTimeOfDay(SeasonID::Paris_Morning);
        ImGui::SameLine();
        if (ImGui::Button("白天"))
            SetTimeOfDay(SeasonID::Paris_Daytime);
        ImGui::SameLine();
        if (ImGui::Button("傍晚"))
            SetTimeOfDay(SeasonID::Paris_Evening);
        ImGui::SameLine();
        if (ImGui::Button("夜晚"))
            SetTimeOfDay(SeasonID::Paris_Night);

        ImGui::Separator();
        ImGui::Text("天气");
        ImGui::Separator();

        if (ImGui::Button("晴朗"))
            Weather_Clear();
        ImGui::SameLine();
        if (ImGui::Button("多云"))
            Weather_ModeratelyCloudy();
        ImGui::SameLine();
        if (ImGui::Button("小雨"))
            Weather_RainSmugglersParadise();
        ImGui::SameLine();
        if (ImGui::Button("大雨"))
            Weather_RainBellecFight();
        if (ImGui::Button("让天气自然变化"))
            LetTheWeatherGo();


        ImGui::Separator();
        if (ImGuiCTX::TreeNode _finerControls{ "精细控制" })
        {
            //ImGui::Text(
            //    "If you quickly flick the Cloudiness slider left then right, maybe several times,"
            //    "\nyou may end up with a weather that's actually very sunny instead of maximum cloudy."
            //    "\nThis is a bug, but can give you a lovely golden sunlight in the evenings."
            //);
            WeatherControls& weatherSettings = WeatherControls::GetSingleton();
            if (ImGui::SliderFloat("阴天程度", &weatherSettings.cloudinessCurrent, 0.0f, 1.0f))
            {
                weatherSettings.cloudinessFrom = weatherSettings.cloudinessTo = weatherSettings.cloudinessCurrent;
            }
            if (ImGui::SliderFloat("雨量", &weatherSettings.rainStrengthCurrent, 0.0f, 1.0f))
            {
                weatherSettings.rainStrengthFrom = weatherSettings.rainStrengthTo = weatherSettings.rainStrengthCurrent;
            }
            ImGui::SliderFloat("干燥区域", &weatherSettings.drySpotsWetness, 0.0f, 1.0f);
            if (ImGui::IsItemHovered())
            {
                ImGui::SetTooltip("最干燥区域的湿度，也影响玩家衣物");
            }
            ImGui::SliderFloat("水坑大小", &weatherSettings.puddlesSize, 0.0f, 1.0f);
            ImGui::SliderFloat("风力", &weatherSettings.windStrength, 0.0f, 1.0f);
            if (ImGui::IsItemHovered())
            {
                ImGui::SetTooltip("风力强度（仅影响旗帜和植被）");
            }
        }
    }
    else if (*recognizedCurrentLocation == WorldSection::Franciade)
    {
        if (ImGui::Button("法兰西亚德白天"))
            SetTimeOfDay(SeasonID::Franciade_0xC_orangeishDay);
        ImGui::SameLine();
        if (ImGui::Button("法兰西亚德夜晚"))
            SetTimeOfDay(SeasonID::Franciade_0x13_greyishNight);
        ImGui::Separator();

        WeatherControls& weatherSettings = WeatherControls::GetSingleton();
        if (ImGui::SliderFloat("阴天程度", &weatherSettings.cloudinessCurrent, 0.0f, 1.0f))
        {
            weatherSettings.cloudinessFrom = weatherSettings.cloudinessTo = weatherSettings.cloudinessCurrent;
        }
        if (ImGui::SliderFloat("雨量", &weatherSettings.rainStrengthCurrent, 0.0f, 1.0f))
        {
            weatherSettings.rainStrengthFrom = weatherSettings.rainStrengthTo = weatherSettings.rainStrengthCurrent;
        }
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip("法兰西亚德没有雨水视觉效果。");
        }
        ImGui::SliderFloat("湿度", &weatherSettings.drySpotsWetness, 0.0f, 1.0f);
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip("最干燥区域的湿度，也影响玩家衣物");
        }
        ImGui::SliderFloat("水坑大小", &weatherSettings.puddlesSize, 0.0f, 1.0f);
    }
}
