#include "pch.h"

#include "ACU/TypeInfo.h"
#include "ImGuiCTX.h"

namespace
{
struct ModuleBounds
{
    uintptr_t begin = 0;
    uintptr_t end = 0;
};

ModuleBounds GetMainGameModuleBounds()
{
    const HMODULE gameModule = GetModuleHandleW(nullptr);
    if (!gameModule)
    {
        return {};
    }

    const auto* dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(gameModule);
    if (dos->e_magic != IMAGE_DOS_SIGNATURE)
    {
        return {};
    }

    const auto* nt = reinterpret_cast<const IMAGE_NT_HEADERS*>(
        reinterpret_cast<const byte*>(gameModule) + dos->e_lfanew);
    if (nt->Signature != IMAGE_NT_SIGNATURE)
    {
        return {};
    }

    const uintptr_t begin = reinterpret_cast<uintptr_t>(gameModule);
    const uintptr_t end = begin + nt->OptionalHeader.SizeOfImage;
    return { begin, end };
}
}

bool IsInMainGameModule(uintptr_t addr)
{
    const ModuleBounds bounds = GetMainGameModuleBounds();
    return bounds.begin && addr >= bounds.begin && addr < bounds.end;
}
bool IsFunctionPtrInMainGameModule(uintptr_t addr)
{
    return ((addr & 7) == 0) // divisible by 8
        && IsInMainGameModule(addr);
}
bool IsAlignedPtrInMainGameModule_orNull(uintptr_t addr)
{
    return addr == 0 || IsFunctionPtrInMainGameModule(addr);
}
uintptr_t readQword(uintptr_t addr) { return *(uintptr_t*)addr; }
bool IsLooksLikeTypeInfo(uintptr_t addr)
{
    if (!IsInMainGameModule(addr)) { return false; }
    if ((addr & 7) != 0) { return false; }
    // addr + 0x50 should be a valid function pointer(or nullptr)
    uintptr_t creationFunc = *(uintptr_t*)(addr + 0x50);
    if (!IsAlignedPtrInMainGameModule_orNull(creationFunc)) { return false; }
    if (!IsAlignedPtrInMainGameModule_orNull(readQword(addr))) { return false; }
    if (!IsAlignedPtrInMainGameModule_orNull(readQword(addr + 8))) { return false; }
    return true;
}

TypeInfo* FindFirstTypeInfoInThinBranch(uintptr_t thinBranch)
{
    uintptr_t ptr = thinBranch;
    for (size_t i = 0; i < 0x100; i++)
    {
        uintptr_t potentialTI = readQword(ptr);
        if (IsLooksLikeTypeInfo(potentialTI)) { return (TypeInfo*)ptr; }
        ptr = ptr + 8;
    }
    return nullptr;
}

uintptr_t FindWhereTypeInfosStartInBranch(uintptr_t branch)
{
    // The operations below are done by the game during lookup of TypeInfos.
    // For the logic behind this, see code at 0x14277b4de
    uintptr_t thinBranch = readQword(branch);
    uintptr_t garbageLengthLookupArray = 0x14364A451;
    byte helperValue = *(byte*)(branch + 8);
    byte garbageLength = *(byte*)(garbageLengthLookupArray + helperValue);
    return thinBranch + 8 * garbageLength;
}
std::string GetTypeNameAndBasesFromTypeInfo(TypeInfo& ti)
{
    std::string s = ti.typeName;
    TypeInfo* currentTI = ti.base;
    while (currentTI != nullptr)
    {
        s.append(std::string(" : ") + currentTI->typeName);
        currentTI = currentTI->base;
    }
    return s;
}
size_t GetStructSizeFromTypeInfo(TypeInfo& ti)
{
    return ti.structSize;
}

void writeToClipboard(const std::string_view& str)
{
    ImGui::SetClipboardText(str.data());
}
class TypeInfoSystemDump
{
public:
    void Draw();
private:
    void FindAllTypeInfos()
    {
        foundTIs.clear();
        foundTIs.reserve(8000);
        // See function at 0x14277B010 in ACU.exe for the way these structures are walked
        // when looking up a TypeInfo.

        uintptr_t TISroot = 0x14525D950;
        uintptr_t trunk = readQword(TISroot);
        // This rounds down to address divisible by 8. This is what seems to be done
        // in the game.
        trunk = trunk & (-8);
        trunk = readQword(trunk + 8);
        uintptr_t branches = readQword(trunk);
        /*
        Branches here are 0x10-byte structures
        of the following type:
        struct Branch
        {
          ThinBranch* thinBranch;
          uint16 helperValue_8;               // Used for lookups in the ThinBranch, I think.
          uint16 incrementedForEachBranch_A;
          char zeros_C[2];
          uint16 sameForAllBranches_E;        // == 0x26, I think.
        };
        ThinBranch is a structure that contains an array of pointers to TypeInfo structs,
        but the array is preceded by a variable-length garbage (?) bytes.
        So like this:
          F6D309C0F4025425 6E34DC5029669922
          444AF243746238E5 F0BA5A74BF4FAA80
          E87FCC6C6BAE375E 8EB8EC87908F81EF
          AA7BA0D27096529F 72C88B21C0DE20B8
          F0CF74DCD3A1CCAC 0000000144FFF6FB
          00000000978375FF 0000000143F22000
          00000000279AA076 0000000143DCB340 <--- First TypeInfo pointer (on the right)
          0000000143EE0010 0000000143E39010
          000000014403ECC0 0000000144E59930
          ...more TypeInfos...
        In this instance the TypeInfo pointers start at (ThinBranch address)+0x68,
        but the "0x68" offset from the beginning isn't constant.
        The length of garbage is determined via hardcoded arrays in the ACU.exe
        and the "helperValue_8" above.
        Do note that the garbage may look like a valid aligned pointer (e.g. 0000000143F22000),
        so that criterium isn't sufficient to determine where TypeInfos start.
        */
        for (size_t branchNum = 0; branchNum < 0x100; branchNum++)
        {
            uintptr_t branch = branches + 0x10 * branchNum;
            uintptr_t thinBranch = readQword(branch);
            uintptr_t leaf = thinBranch;
            size_t _safetyCounter = 0;
            size_t _maxIterations = 0xFF;

            leaf = FindWhereTypeInfosStartInBranch(branch);
            _safetyCounter = 0;
            while (_safetyCounter < _maxIterations && IsLooksLikeTypeInfo(readQword(leaf)))
            {
                TypeInfo* ti = (TypeInfo*)readQword(leaf);
                foundTIs.push_back(ti);
                _safetyCounter = _safetyCounter + 1;
                leaf = leaf + 8;
            }
        }
        std::set<TypeInfo*> removedDoubles{ foundTIs.begin(), foundTIs.end() };
        foundTIs = { removedDoubles.begin(), removedDoubles.end() };
        std::sort(foundTIs.begin(), foundTIs.end(), [](TypeInfo* a, TypeInfo* b) {
            return std::string_view(a->typeName) < std::string_view(b->typeName);
            });
    }
    void ToClipboard_AllTIAddrs()
    {
        std::string allTIsString;
        allTIsString.reserve(foundTIs.size() * 10);
        std::stringstream ss;
        ss << std::hex;
        for (auto& ti : foundTIs)
        {
            ss << ti << '\n';
        }
        writeToClipboard(ss.str());
    }
    void ToClipboard_ParseAllFoundTypeInfos_NoVTBLs()
    {
        std::stringstream ss;
        for (TypeInfo* ti : foundTIs)
        {
            ss << GetTypeNameAndBasesFromTypeInfo(*ti)
                << ", size: 0x" << std::hex << GetStructSizeFromTypeInfo(*ti)
                << ", hash: 0x" << std::hex << (uintptr_t)ti->lookupInTypeInfoSystem_mb
                << ", ti: " << (uintptr_t)ti
                << ", create: " << (uintptr_t)ti->Create
                << ", deserialize: " << (uintptr_t)ti->DeserializeIfNoVTBL
                << '\n';
        }
        writeToClipboard(ss.str());
    }
    // Some of the game types I can't even construct, because the Create() function crashes immediately.
    // I inspected these types manually and I exclude them from the automated dumping process.
    struct ManuallyInspectedType
    {
        const std::string_view name;
        uintptr_t vtbl;
        uintptr_t deserializeVirtual;
    };
    void ToFileLog_AttemptToConstructAllObjects(size_t startFrom, size_t howManyAtMost)
    {
        //safetyhook::ThreadFreezer holdIt;
        std::vector<ManuallyInspectedType> toSkip = {
            ManuallyInspectedType{ "CameraManager", 0x14304E9C0, 0x141eeffe0 },
            ManuallyInspectedType{ "CameraProxy", 0x143041EC0, 0x1416afc50 },
            ManuallyInspectedType{ "NetPartyManager", 0x143012B70, 0x1416afc50 },
        };
        size_t howManyDone = 0;
        for (size_t i = startFrom; i < std::min(startFrom + howManyAtMost, foundTIs.size()); i++)
        {
            TypeInfo& ti = *foundTIs[i];
            ImGui::LogToFile();
            ImGui::LogText(
                "%4d: %s, size: 0x%X, hash: 0x%X, ti: %llX, create: %llX, vtbl: "
                , i
                , GetTypeNameAndBasesFromTypeInfo(ti).c_str()
                , ti.structSize
                , ti.lookupInTypeInfoSystem_mb
                , (uintptr_t)&ti
                , (uintptr_t)ti.Create
            );
            uintptr_t vtbl = 0;
            bool isHasNoCreateFunc = false;
            {
                bool isThisTypeSkipped = false;
                for (const ManuallyInspectedType& skippedType : toSkip)
                {
                    if (skippedType.name == ti.typeName)
                    {
                        isThisTypeSkipped = true;
                        vtbl = skippedType.vtbl;
                        break;
                    }
                }
                if (!isThisTypeSkipped)
                {
                    bool hasCreateFuncAndIsConstructed = TryToConstruct(ti);
                    if (hasCreateFuncAndIsConstructed)
                    {
                        vtbl = (uintptr_t&)m_constructedObject.value().bytes[0];
                    }
                    else
                    {
                        isHasNoCreateFunc = true;
                    }
                }
            }
            {
                if (isHasNoCreateFunc)
                {
                    ImGui::LogText("VTBL-NoCreateFunc");
                }
                else
                {
                    // Pulling the size from the PE header seems simpler and less fragile.
                    // If there was a reason to keep the hardcoded constants here,
                    // this can be reverted pretty easily.
                    // constexpr uintptr_t gameModuleStart = 0x140000000;
                    // constexpr uintptr_t gameModuleEnd = 0x147825000;
                    const ModuleBounds gameModuleBounds = GetMainGameModuleBounds();
                    // Many types have this pointer at the start, but it's not a VTBL,
                    // just an empty shared pointer.
                    constexpr uintptr_t gameModuleEmptySharedBlock = 0x14525DA98;
                    if (vtbl == gameModuleEmptySharedBlock)
                    {
                        ImGui::LogText("VTBL-DefinitelyNone");
                        vtbl = 0;
                    }
                    else if (!gameModuleBounds.begin || vtbl < gameModuleBounds.begin || vtbl >= gameModuleBounds.end)
                    {
                        ImGui::LogText("VTBL-NoneOrOutsideGameModule");
                        vtbl = 0;
                    }
                    else
                    {
                        ImGui::LogText("%llX", vtbl);
                    }
                }
            }
            ImGui::LogText(", deser-static: ");
            ImGui::LogText("%llX", (uintptr_t)ti.DeserializeIfNoVTBL);
            ImGui::LogText(", deser-virt: ");
            if (!vtbl)
            {
                ImGui::LogText("DESER-NoVTBL");
            }
            else
            {
                uintptr_t deserializeVirtual = ((uintptr_t*)vtbl)[2];
                ImGui::LogText("%llX", deserializeVirtual);
            }
            ImGui::LogFinish();
        }
    }
    bool TryToConstruct(TypeInfo& ti)
    {
        if (!ti.Create)
        {
            m_constructedObject.reset();
            return false;
        }
        m_constructedObject.emplace();
        m_constructedObject->name = ti.typeName;
        m_constructedObject->bytes.resize(ti.structSize, 0);
        ti.Create(m_constructedObject->bytes.data());
        return true;
    }
private:
    std::vector<TypeInfo*> foundTIs;
    struct ConstructedObject
    {
        std::string name;
        std::vector<byte> bytes;
    };
    std::optional<ConstructedObject> m_constructedObject;
};

void TypeInfoSystemDump::Draw()
{
    ImGui::Text("找到类型信息: %d", foundTIs.size());
    if (ImGui::Button("查找所有类型信息")) {
        FindAllTypeInfos();
    }
    if (ImGui::Button("复制找到的类型信息到剪贴板（仅地址）")) {
        ToClipboard_AllTIAddrs();
    }
    if (ImGui::Button("解析所有 TI 并复制到剪贴板")) {
        ToClipboard_ParseAllFoundTypeInfos_NoVTBLs();
    }
    if (ImGui::Button("警告：会崩溃！- 尝试构造所有对象以查找 vtable")) {
        ImGui::OpenPopup("TIS_confirm_construction_crash");
    }
    if (ImGuiCTX::Popup _{ "TIS_confirm_construction_crash" }) {
        ImGui::Text(
            "确定吗？如果针对所有类型信息执行，之后程序一定会崩溃。\n"
            "即使不崩溃，我也建议执行后立即重启游戏。\n"
            "崩溃的原因是某些构造函数也设置了全局变量。\n"
            "我添加了一组不尝试创建的类型，以便能\n"
            "通过剩余的列表，但崩溃仍然会发生。\n"
            "要完全生成转储，只需重启游戏几次，每次从上次失败的类型开始。\n"
            "大约只需要重复3-4次。\n"
            "结果将追加到游戏目录中的 imgui_log.txt。\n"
        );
        static size_t attemptedConstructionFirstIdx = 0;
        static size_t howManyToAttempt = 10000;
        ImGui::InputInt("attemptedConstructionFirstIdx", (int*)&attemptedConstructionFirstIdx);
        ImGui::InputInt("howManyToAttempt", (int*)&howManyToAttempt);
        if (ImGui::Button("是的，我确定")) {
            ToFileLog_AttemptToConstructAllObjects(attemptedConstructionFirstIdx, howManyToAttempt);
        }
    }
    if (m_constructedObject) {
        ImGui::Text("%s, size: 0x%X, addr: %llX", m_constructedObject->name.c_str(), m_constructedObject->bytes.size(), m_constructedObject->bytes.data());
        ImGui::SameLine();
        if (ImGui::Button("复制地址到剪贴板")) {
            ImGui::LogToClipboard();
            ImGui::LogText("%llX", m_constructedObject->bytes.data());
            ImGui::LogFinish();
        }
    }
    ImGuiCTX::WindowChild _subwindow{ "TIS list", { 0, 250 }, true };
    static ImGuiTextFilter filter;
    filter.Draw("Filter (inc,-exc)##TIS");
    for (TypeInfo* ti : foundTIs)
    {
        if (filter.PassFilter(ti->typeName))
        {
            if (ImGui::Selectable(ti->typeName))
            {
                writeToClipboard(ti->typeName);
                TryToConstruct(*ti);
            }
            if (ImGui::IsItemHovered())
            {
                ImGui::SetTooltip("构建");
            }
        }
    }
}
void TypeInfoSystemTests()
{
    static TypeInfoSystemDump tis;
    tis.Draw();
}
