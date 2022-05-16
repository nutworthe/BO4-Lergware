#include "common.h"

template<class T> T __ROL__(T value, int count)
{
    const unsigned int nbits = sizeof(T) * 8;

    if (count > 0)
    {
        count %= nbits;
        T high = value >> (nbits - count);
        if (T(-1) < 0) // signed value
            high &= ~((T(-1) << count));
        value <<= count;
        value |= high;
    }
    else
    {
        count = -count % nbits;
        T low = value << (nbits - count);
        value >>= count;
        value |= low;
    }
    return value;
}

inline __int64 __ROL8__(__int64 value, int count) { return __ROL__((__int64)value, count); }

uintptr_t dwProcessBase = reinterpret_cast<uintptr_t>(GetModuleHandleA(NULL));
uintptr_t cgArray;
uintptr_t cg_entitiesArray;

dwNetadrToCommonAddrT dwNetadrToCommonAddr = (dwNetadrToCommonAddrT)(dwProcessBase + 0x2ED5050);
NET_OutOfBandPrintT NET_OutOfBandPrint = (NET_OutOfBandPrintT)(dwProcessBase + 0x2E065A0);
Com_GetClientDObjT Com_GetClientDObj = (Com_GetClientDObjT)(dwProcessBase + 0x3EAD620);
CG_DObjGetWorldTagPosInternalT CG_DObjGetWorldTagPosInternal = (CG_DObjGetWorldTagPosInternalT)(dwProcessBase + 0x90BD70);
tCG_CanSeeFriendlyHeadTrace CG_CanSeeFriendlyHeadTrace = (tCG_CanSeeFriendlyHeadTrace)(dwProcessBase + 0x1274030);
CG_IsEntityFriendlyNotEnemyT CG_IsEntityFriendlyNotEnemy = (CG_IsEntityFriendlyNotEnemyT)(dwProcessBase + 0x9462D0);
WorldPosToScreenPosT WorldPosToScreenPos = (WorldPosToScreenPosT)(dwProcessBase + 0x1206910);
draw2DInternalT draw2DInternal;
CG_PredictPlayerStateT CG_PredictPlayerState;
CL_WritePacketT CL_WritePacket;
CG_BulletHitEventT CG_BulletHitEvent;
CL_GetConfigStringT CL_GetConfigString;

float flHue;
float boldness = 1.75f;
float rgb_speed = 0.25;

int aim_sel;
int speed_sel;
int style_sel;
int tracer_type;
int slotItem = 0;
int kick_sel = 0;
int class_sel = 0;
int kick_selection;
int kick[] = { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5 };

bool show = true;
bool open = true;
bool rgb_enemy;
bool rgb_friendly;
bool enemies_only;
bool filled_boxes;
bool unfilled_boxes;
bool corner_esp;
bool snaplines;
bool name_esp;
bool bone_esp;
bool healthbars;
bool distance_esp;
bool visuals_active;
bool thirdPerson;
bool BulletTracers;
bool aimbot_active;
bool SilentAim;
bool Auto_Fire;
bool AntiAim;
bool ads_check;
bool VisCheck;
bool no_recoil;
bool selected[14];
bool spoof_hostname;
bool spoof_hostxuid;
bool spoof_netadr;

char lookupName[255];
char name_input[32] = { 0 };

const char* kicks[] = {
    "DROP",
    "KICK",
    "BADDLC",
    "KICK_PARTY",
    "HOSTRELOAD",
    "NOPARTYCHAT"
};

const char* boneSel[]{
    "j_ankle_le",
    "j_ankle_ri",
    "j_wrist_le",
    "j_wrist_ri",
    "j_elbow_le",
    "j_elbow_ri",
    "j_knee_le",
    "j_knee_ri",
    "j_shoulder_le",
    "j_shoulder_ri",
    "j_spineupper",
    "j_spinelower",
    "j_neck",
    "j_head",
};

const char* slots[] = {
    "primary",
    "primaryattachment1",
    "primaryattachment2",
    "primaryattachment3",
    "primaryattachment4",
    "primaryattachment5",
    "primaryattachment6",
    "primaryattachment7",
    "primarycamo",
    "primaryreticle",
    "primarypaintjobslot",
    "primaryweaponmodelslot",
    "primarycharm",
    "primarydeathfx",
    "secondary",
    "secondaryattachment1",
    "secondaryattachment2",
    "secondaryattachment3",
    "secondaryattachment4",
    "secondaryattachment5",
    "secondaryattachment6",
    "secondaryattachment7",
    "secondarycamo",
    "secondaryreticle",
    "secondarypaintjobslot",
    "secondaryweaponmodelslot",
    "secondarycharm",
    "secondarydeathfx",
    "primarygrenade",
    "primarygrenadecount",
    "specialgrenade",
    "specialgrenadecount",
    "herogadget",
    "specialty1",
    "specialty2",
    "specialty3",
    "specialty4",
    "specialty5",
    "specialty6",
    "talisman1",
    "tacticalgear",
    "talent1",
    "talent2",
    "talent3",
    "talent4",
    "talent5",
    "talent6",
    "bonuscard1",
    "bonuscard2",
    "bonuscard3",
    "equippedBubbleGumPack",
    "killStreak1",
    "killStreak2",
    "killStreak3",
    "killStreak4"
};

const char* customclass[] = {

    "Custom Class 1",
    "Custom Class 2",
    "Custom Class 3",
    "Custom Class 4",
    "Custom Class 5",
    "Custom Class 6",
    "Custom Class 7",
    "Custom Class 8",
    "Custom Class 9",
    "Custom Class 10",
    "Custom Class 11",
    "Custom Class 12"

};

const char* tracerstypes[]{ "Cross", "Circle" };
const char* speed_selection[] = { "Slow", "Medium", "Fast", "Hyper" };
const char* style_selection[] = { "Main", "Alternative", "Blackout", "RGB" };

//HOST
float timescale_slider = 1.0f;

int client_number = 0;
int setpoints_input;

bool pGod = false;
bool pAmmo = false;
bool pPoints = false;
bool pNoTarget = false;
bool pBarriers = false;
bool pHeadshots = false;
bool godmode = false;
bool NoTarget = false;
bool setpoints = false;
bool enableplasma = false;
bool unlimitedammo = false;
bool unlimitedspecial = false;

ImColor text_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0);
ImColor enemy_colors = ImVec4(1.0f, 0.682f, 0.0f, 0.9);
ImColor friendly_colors = ImVec4(1.0f, 1.0f, 1.0f, 0.9);

//STD NAMESPACE
std::vector<player> players;
std::vector<bullet_tracer> tracers;

DWORD_PTR nameBuffer = *(DWORD_PTR*)(dwProcessBase + 0x59F1288) + 4i64;

BOOLEAN MaskCompare(uintptr_t buffer, LPCSTR pattern, LPCSTR mask) {
    for (auto b = reinterpret_cast<PBYTE>(buffer); *mask; ++pattern, ++mask, ++b) {
        if (*mask == 'x' && *reinterpret_cast<LPCBYTE>(pattern) != *b) {
            return FALSE;
        }
    }

    return TRUE;
}

uintptr_t FindSpoof()
{
    static uintptr_t spoofAddr = 0;
    if (spoofAddr <= 0) {
        MODULEINFO info = { 0 };
        GetModuleInformation(GetCurrentProcess(), GetModuleHandle(0), &info, sizeof(info));

        bool found = false;
        while (!found) {
            PVOID base = info.lpBaseOfDll;
            DWORD size = info.SizeOfImage;
            LPCSTR pattern = "\xFF\x26";
            LPCSTR mask = "xx";

            size -= static_cast<DWORD>(strlen(mask));

            for (auto i = 0UL; i < size; ++i) {
                auto addr = reinterpret_cast<uintptr_t>(base) + i;
                if (MaskCompare(addr, pattern, mask)) {
                    uintptr_t relative = addr - dwProcessBase;
                    if (relative < (uintptr_t)(dwProcessBase + 0x2000000))
                        if (MaskCompare((uintptr_t)(addr - 0x2), "\xFF", "x") || MaskCompare((uintptr_t)(addr - 0x3), "\xFF", "x") || MaskCompare((uintptr_t)(addr - 0x3), "\xFF", "x") || MaskCompare((uintptr_t)(addr - 0x4), "\xFF", "x") || MaskCompare((uintptr_t)(addr - 0x5), "\xE8", "x") || MaskCompare((uintptr_t)(addr - 0x6), "\xFF", "x") || MaskCompare((uintptr_t)(addr - 0x7), "\xFF", "x")) {
                            spoofAddr = addr;
                            return addr;
                        }
                }
            }
        }

        return 0;
    }
    return spoofAddr;
}

__int64 GetClientDObj(int handle)
{
    auto spoof = FindSpoof();
    if (!spoof)
        return 0;

	int customHandle[2] = { 1337, handle };
	return spoof_call((const void*)spoof, Com_GetClientDObj, customHandle, 0);
}

bool IsEntityFriendly(__int64 centity_t) {
    return spoof_call((const void*)FindSpoof(), CG_IsEntityFriendlyNotEnemy, 0, centity_t);
}

bool IsTargetVisible(__int64 centity_t, vec3_t start, vec3_t end) {
    if (!VisCheck) return true;

    float s[3] = { start.x, start.y, start.z };
    float t[3] = {end.x, end.y, end.z};
    return spoof_call((const void*)FindSpoof(), CG_CanSeeFriendlyHeadTrace, 0, centity_t, (__int64)s, (__int64)t);

}
typedef bool(__fastcall* CG_GetPlayerViewOriginT)(unsigned int localClientNum, __int64 playerstate, float* origin);
CG_GetPlayerViewOriginT CG_GetPlayerViewOrigin1 = (CG_GetPlayerViewOriginT)((DWORD64)GetModuleHandleA(NULL) + 0x21031D0);

vec3_t CG_GetPlayerViewOrigin(__int64 playerstate) {
    float* origin{};
    spoof_call((const void*)FindSpoof(), CG_GetPlayerViewOrigin1, (unsigned int)0, playerstate, origin);

    return { 0, 0, 0 };
}

uintptr_t get_entity(uintptr_t array, int index)
{
	return *(uintptr_t*)(array + (3560 * index));
};
/*
XSESSION_INFO get_session_info(const InfoResponseLobby& lobby)
{
    XSESSION_INFO sess_info{};
    sess_info.sessionID = lobby.secId;
    sess_info.keyExchangeKey = lobby.secKey;
    sess_info.hostAddress = lobby.serializedAdr.xnaddr;

    return sess_info;
}

netadr_t get_net_adr(const XSESSION_INFO& info)
{
    netadr_t netadr{};
    dwRegisterSecIDAndKey(&info.sessionID, &info.keyExchangeKey);
    dwCommonAddrToNetadr(&netadr, &info.hostAddress, &info.sessionID);

    return netadr;
}
*/
bool send_oob(const netadr_t& target, const std::string& data, const netsrc_t& sock)
{
    return NET_OutOfBandPrint(sock, target, data.data());
}

bool is_in_array(std::string cmp1, std::vector<std::string> cmp2)
{
	for (auto cmp : cmp2)
	{
		if (!_stricmp(cmp1.data(), cmp.data()))
			return true;
	}
	return false;
}

bool is_in_number_array(int cmp1, std::vector<int> cmp2)
{
    for (auto cmp : cmp2)
    {
        if (cmp1 == cmp)
            return true;
    }
    return false;
}

void VectorSubtract(vec3_t a, const vec3_t& b, vec3_t& c)
{
    c.x = a.x - b.x;
    c.y = a.y - b.y;
    c.z = a.z - b.z;
}

float DotProduct(const vec3_t& v1, const vec3_t& v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

vec2_t WorldToScreen(vec3_t vWorldLocation, int type)
{
    if (cgArray > 0) {
        if (type == 0)
        {
            RECT desktop;
            const HWND hDesktop = GetDesktopWindow();
            GetWindowRect(hDesktop, &desktop);
            vec2_t res = { desktop.right, desktop.bottom };

            int width = res.x / 2;
            int height = res.y / 2;

            vec3_t vLocal, vTransForm;

            vec3_t vfwd;
            vfwd.x = *(float*)(cgArray + 1479220);
            vfwd.y = *(float*)(cgArray + 1479224);
            vfwd.z = *(float*)(cgArray + 1479228);

            vec3_t vright;
            vright.x = *(float*)(cgArray + 1479232);
            vright.y = *(float*)(cgArray + 1479236);
            vright.z = *(float*)(cgArray + 1479240);

            vec3_t vup;
            vup.x = *(float*)(cgArray + 1479244);
            vup.y = *(float*)(cgArray + 1479248);
            vup.z = *(float*)(cgArray + 1479252);

            vec3_t viewOrg;
            viewOrg.x = *(float*)(cgArray + 1479192);
            viewOrg.y = *(float*)(cgArray + 1479196);
            viewOrg.z = *(float*)(cgArray + 1479200);
            VectorSubtract(vWorldLocation, viewOrg, vLocal);

            vTransForm.x = DotProduct(vLocal, vright);
            vTransForm.y = DotProduct(vLocal, vup);
            vTransForm.z = DotProduct(vLocal, vfwd);

            if (vTransForm.z < 0.01)
                return { 0, 0 };

            float fovX = *(float*)(cgArray + 1479168);
            float fovY = *(float*)(cgArray + 1479172);

            return { width * (1 - (vTransForm.x / fovX / vTransForm.z)) , height * (1 - (vTransForm.y / fovY / vTransForm.z)) };
        }
        else if (type == 1)
        {
            float world[3];
            world[0] = vWorldLocation.x;
            world[1] = vWorldLocation.y;
            world[2] = vWorldLocation.z;
            float out[3];
            spoof_call((const void*)FindSpoof(), WorldPosToScreenPos, 0, world, out);

            return { out[0], out[1] };

        }
    }
}

clientactivet* GetClientActivet()
{
    __int64 v0; // rdx
    unsigned int v1; // eax
    int v2; // er8
    __int64 v3; // r9

    v0 = *(__int64*)(dwProcessBase + 0x8A5A508);
    if (!v0)
        return (clientactivet*)v0;
    v1 = -1674788982;
    v2 = 583459170;
    v3 = 0xFFFFFFFFi64;
    while (1)
    {
        if (v1 <= 0x9C2CBF8A)
        {
            switch (v1)
            {
            case 0x8B2A0311:
                v3 ^= 0xAE2F0A8ui64;
                v2 = 723440567;
                break;
            case 0x935FC8E2:
                v3 ^= 0x455F1A0ui64;
                v2 = 1556625907;
                break;
            case 0x9C2CBF8A:
                v3 = (unsigned int)v0;
                v2 = 259225448;
                break;
            case 0x3B1830B7u:
                v3 = (unsigned int)v0;
                v2 = -1338887258;
                break;
            case 0x4E48AFCDu:
                v0 ^= v3;
                v2 = 1968217978;
                break;
            }
            goto LABEL_22;
        }
        if (v1 != -1607151450)
            break;
        v2 = 1579923078;
    LABEL_21:
        v0 = __ROL8__(v0, 32);
    LABEL_22:
        v1 ^= v2;
        if (v1 == -144223113)
            return (clientactivet*)v0;
    }
    if (v1 != -1091636405)
    {
        if (v1 != -812125935)
        {
            if (v1 == -31496672)
            {
                v0 ^= v3;
                v2 = 1089574251;
            }
            goto LABEL_22;
        }
        v2 = -2116067620;
        goto LABEL_21;
    }
    return (clientactivet*)__ROL8__(v0, 32);
}

clientactive* GetClientActive()
{
    __int64 v0; // rdx
    unsigned int v1; // eax
    int v2; // er8
    __int64 v3; // r9

    v0 = *(__int64*)(dwProcessBase + 0x8A5A508);
    if (!v0)
        return (clientactive*)v0;
    v1 = -1674788982;
    v2 = 583459170;
    v3 = 0xFFFFFFFFi64;
    while (1)
    {
        if (v1 <= 0x9C2CBF8A)
        {
            switch (v1)
            {
            case 0x8B2A0311:
                v3 ^= 0xAE2F0A8ui64;
                v2 = 723440567;
                break;
            case 0x935FC8E2:
                v3 ^= 0x455F1A0ui64;
                v2 = 1556625907;
                break;
            case 0x9C2CBF8A:
                v3 = (unsigned int)v0;
                v2 = 259225448;
                break;
            case 0x3B1830B7u:
                v3 = (unsigned int)v0;
                v2 = -1338887258;
                break;
            case 0x4E48AFCDu:
                v0 ^= v3;
                v2 = 1968217978;
                break;
            }
            goto LABEL_22;
        }
        if (v1 != -1607151450)
            break;
        v2 = 1579923078;
    LABEL_21:
        v0 = __ROL8__(v0, 32);
    LABEL_22:
        v1 ^= v2;
        if (v1 == -144223113)
            return (clientactive*)v0;
    }
    if (v1 != -1091636405)
    {
        if (v1 != -812125935)
        {
            if (v1 == -31496672)
            {
                v0 ^= v3;
                v2 = 1089574251;
            }
            goto LABEL_22;
        }
        v2 = -2116067620;
        goto LABEL_21;
    }
    return (clientactive*)__ROL8__(v0, 32);
}

vec3_t CG_DObjGetWorldTagPos(__int64 centity, __int64 DObj, int tag, float* whatever)
{
    vec3_t pos;
    float WORLD[3];
    auto DOBJ = CG_DObjGetWorldTagPosInternal(centity, DObj, tag, whatever, WORLD, 0xFFFF);

    pos.x = WORLD[0];
    pos.y = WORLD[1];
    pos.z = WORLD[2];

    return pos;
}

bones updateBones(int i)
{
    bones bone_arr;
	DWORD j_neck = *(int*)(dwProcessBase + 0x7D9640C);
	DWORD j_helmet = *(int*)(dwProcessBase + 0x7D963E8);
	DWORD j_head = *(int*)(dwProcessBase + 0x7D963E4);
	DWORD j_spineupper = *(int*)(dwProcessBase + 0x7D9642C);
	DWORD j_spinelower = *(int*)(dwProcessBase + 0x7D96428);
	DWORD j_shoulder_le = *(int*)(dwProcessBase + 0x7D96424);
	DWORD j_shoulder_ri = *(int*)(dwProcessBase + 0x7D96420);
	DWORD j_elbow_le = *(int*)(dwProcessBase + 0x7D963FC);
	DWORD j_elbow_ri = *(int*)(dwProcessBase + 0x7D96418);
	DWORD j_wrist_ri = *(int*)(dwProcessBase + 0x7D9641C);
	DWORD j_wrist_le = *(int*)(dwProcessBase + 0x7D96400);
	DWORD j_knee_le = *(int*)(dwProcessBase + 0x7D963F4);
	DWORD j_knee_ri = *(int*)(dwProcessBase + 0x7D963F8);
	DWORD j_ankle_ri = *(int*)(dwProcessBase + 0x7D963B0);
	DWORD j_ankle_le = *(int*)(dwProcessBase + 0x7D963AC);

	__int64 centity_t = cg_entitiesArray + (3560 * i);

	__int64 DObj = GetClientDObj(i);
  
    
	float whatever1[255];
	if (DObj > 0) {
        bone_arr.j_headpos = CG_DObjGetWorldTagPos(centity_t, DObj, j_head, whatever1);
        bone_arr.j_neckpos = CG_DObjGetWorldTagPos(centity_t, DObj, j_neck, whatever1);
        bone_arr.j_shoulder_lepos = CG_DObjGetWorldTagPos(centity_t, DObj, j_shoulder_le, whatever1);
        bone_arr.j_shoulder_ripos = CG_DObjGetWorldTagPos(centity_t, DObj, j_shoulder_ri, whatever1);
        bone_arr.j_spineupperpos = CG_DObjGetWorldTagPos(centity_t, DObj, j_spineupper, whatever1);
        bone_arr.j_spinelowerpos = CG_DObjGetWorldTagPos(centity_t, DObj, j_spinelower, whatever1);
        bone_arr.j_elbow_lepos = CG_DObjGetWorldTagPos(centity_t, DObj, j_elbow_le, whatever1);
        bone_arr.j_elbow_ripos = CG_DObjGetWorldTagPos(centity_t, DObj, j_elbow_ri, whatever1);
        bone_arr.j_wrist_ripos = CG_DObjGetWorldTagPos(centity_t, DObj, j_wrist_ri, whatever1);
        bone_arr.j_wrist_lepos = CG_DObjGetWorldTagPos(centity_t, DObj, j_wrist_le, whatever1);
        bone_arr.j_knee_lepos = CG_DObjGetWorldTagPos(centity_t, DObj, j_knee_le, whatever1);
        bone_arr.j_knee_ripos = CG_DObjGetWorldTagPos(centity_t, DObj, j_knee_ri, whatever1);
        bone_arr.j_ankle_ripos = CG_DObjGetWorldTagPos(centity_t, DObj, j_ankle_ri, whatever1);
        bone_arr.j_ankle_lepos = CG_DObjGetWorldTagPos(centity_t, DObj, j_ankle_le, whatever1);
        
        
        bone_arr.j_headpos_screen = WorldToScreen(bone_arr.j_headpos);
        bone_arr.j_neckpos_screen = WorldToScreen(bone_arr.j_neckpos);
        bone_arr.j_shoulder_lepos_screen = WorldToScreen(bone_arr.j_shoulder_lepos);
        bone_arr.j_shoulder_ripos_screen = WorldToScreen(bone_arr.j_shoulder_ripos);
        bone_arr.j_spineupperpos_screen = WorldToScreen(bone_arr.j_spineupperpos);
        bone_arr.j_spinelowerpos_screen = WorldToScreen(bone_arr.j_spinelowerpos);
        bone_arr.j_elbow_lepos_screen = WorldToScreen(bone_arr.j_elbow_lepos);
        bone_arr.j_elbow_ripos_screen = WorldToScreen(bone_arr.j_elbow_ripos);
        bone_arr.j_wrist_ripos_screen = WorldToScreen(bone_arr.j_wrist_ripos);
        bone_arr.j_wrist_lepos_screen = WorldToScreen(bone_arr.j_wrist_lepos);
        bone_arr.j_knee_lepos_screen = WorldToScreen(bone_arr.j_knee_lepos);
        bone_arr.j_knee_ripos_screen = WorldToScreen(bone_arr.j_knee_ripos);
        bone_arr.j_ankle_lepos_screen = WorldToScreen(bone_arr.j_ankle_lepos);
        bone_arr.j_ankle_ripos_screen = WorldToScreen(bone_arr.j_ankle_ripos);
	}

    return bone_arr;
}