#pragma once
#include "includes.h"

static auto info_response_hostname = ""s;
static auto info_response_hostxuid = ""s;

extern uintptr_t dwProcessBase;
extern uintptr_t cgArray;
extern uintptr_t cg_entitiesArray;

extern float boldness;
extern float flHue;
extern float rgb_speed;

extern int kick[6];
extern int aim_sel;
extern int slotItem;
extern int kick_sel;
extern int class_sel;
extern int speed_sel;
extern int style_sel;
extern int tracer_type;
extern int kick_selection;

extern bool show;
extern bool open;
extern bool init;
extern bool show;
extern bool enemies_only;
extern bool visuals_active;
extern bool enemies_only;
extern bool filled_boxes;
extern bool unfilled_boxes;
extern bool rgb_enemy;
extern bool rgb_friendly;
extern bool corner_esp;
extern bool snaplines;
extern bool name_esp;
extern bool bone_esp;
extern bool healthbars;
extern bool distance_esp;
extern bool thirdPerson;
extern bool BulletTracers;
extern bool aimbot_active;
extern bool SilentAim;
extern bool Auto_Fire;
extern bool AntiAim;
extern bool VisCheck;
extern bool ads_check;
extern bool no_recoil;
extern bool selected[14];
extern bool spoof_hostname;
extern bool spoof_hostxuid;
extern bool spoof_netadr;

extern char lookupName[255];
extern char name_input[32];

extern const char* kicks[6];
extern const char* slots[55];
extern const char* boneSel[14];
extern const char* customclass[12];
extern const char* tracerstypes[2];
extern const char* speed_selection[4];
extern const char* style_selection[4];

extern ImColor text_color;
extern ImColor enemy_colors;
extern ImColor friendly_colors;

//HOST
extern float timescale_slider;

extern int client_number;
extern int setpoints_input;

extern bool pGod;
extern bool pAmmo;
extern bool pPoints;
extern bool pNoTarget;
extern bool pBarriers;
extern bool pHeadshots;
extern bool godmode;
extern bool NoTarget;
extern bool setpoints;
extern bool enableplasma;
extern bool unlimitedammo;
extern bool unlimitedspecial;

//STD NAMESPACE
extern std::vector<player> players;
extern std::vector<bullet_tracer> tracers;

//HOOKED FUNCTIONS
static auto CL_ConnectionlessCMD = dwProcessBase + 0x2857545;
static auto dwInstantDispatchMessage = dwProcessBase + 0x2ED1F80;
static auto LobbyVM_OnDisconnect = dwProcessBase + 0x38F2CA0;

//PACKAGING & SENDING
const static auto LobbyMsgTransport_SendToAdr = reinterpret_cast<bool(__fastcall*)(int controllerIndex, int channel, LobbyModule destModule, netadr_t adr, __int64 toXuid, msg_t * msg, MsgType msgType)>(dwProcessBase + 0x38F9410);
const static auto LobbyMsgRW_PrepReadMsg = reinterpret_cast<bool(*)(LobbyMsg*, msg_t*)>(dwProcessBase + 0x38F81F0);
const static auto LobbyMsgRW_PrepReadData = reinterpret_cast<bool(*)(LobbyMsg*, char*, int)>(dwProcessBase + 0x38F81C0);
const static auto LobbyMsgRW_PrepWriteMsg = reinterpret_cast<bool(*)(LobbyMsg*, char*, int, MsgType)>(dwProcessBase + 0x38F8220);
const static auto LobbyMsgRWFriends_PrepWriteMsg = reinterpret_cast<bool(*)(LobbyMsg*, char*, int, MsgType, char)>(dwProcessBase + 0x38F8220);
const static auto LobbyMsgRW_PackageInt = reinterpret_cast<bool(*)(void* lobbyMsg, const char* key, int32_t * val)>(dwProcessBase + 0x38F7D50);
const static auto LobbyMsgRW_PackageXuid = reinterpret_cast<bool(*)(void* lobbyMsg, const char* key, __int64* val)>(dwProcessBase + 0x38F8120);
const static auto LobbyMsgRW_PackageUInt = reinterpret_cast<bool(*)(LobbyMsg*, const char*, const unsigned int*)>(dwProcessBase + 0x38F7FE0);

//CLASS EDITOR
const static auto Class1 = reinterpret_cast<std::uintptr_t(__fastcall*)()>(dwProcessBase + 0x289EDD0);
const static auto Class2 = reinterpret_cast<std::uintptr_t(__fastcall*)()>(dwProcessBase + 0x289F0F0);
const static auto Class3 = reinterpret_cast<std::uintptr_t(__fastcall*)(__int64, __int64)>(dwProcessBase + 0x38B42B0);
const static auto Class4 = reinterpret_cast<std::uintptr_t(__fastcall*)(char*, __int64, __int64)>(dwProcessBase + 0x38B4190);
const static auto Class5 = reinterpret_cast<std::uintptr_t(__fastcall*)(const char*)>(dwProcessBase + 0x6BF0E0);
const static auto SetClass = reinterpret_cast<std::uintptr_t(__fastcall*)(char*, __int64, __int64, __int64)>(dwProcessBase + 0x6B94B0);

//LOBBYSESSION
const static auto LobbySession_GetClientByClientNum = reinterpret_cast<SessionClient * (*)(LobbySession*, int)>(dwProcessBase + 0x3905C60);
const static auto LobbySession_GetClientNetAdrByIndex = reinterpret_cast<netadr_t(__fastcall*)(const LobbyType lobbyType, bool a2, const int clientNum, int a4)>(dwProcessBase + 0x39062A0);
const static auto LobbySession_GetControllingLobbySession = reinterpret_cast<bool(__fastcall*)(int a1, const LobbyModule lobbyModule)>(dwProcessBase + 0x3906B10);
const static auto LobbySession_GetSession = reinterpret_cast<LobbySession * (__fastcall*)(unsigned int a1)>(dwProcessBase + 0x3906A30);

//CMDS
const static auto Cbuf_AddText = reinterpret_cast<std::uintptr_t(__fastcall*)(int, const char*)>(dwProcessBase + 0x3CDF020);
const static auto Cmd_ExecuteSingleCommand = reinterpret_cast<std::uintptr_t(__fastcall*)(int, int, const char* text, bool)>(dwProcessBase + 0x3CDFC30);
const static auto CL_AddReliableCommand = reinterpret_cast<std::uintptr_t(__fastcall*)(int, const char*)>(dwProcessBase + 0x28544A0);
const static auto Dvar_SetFromString = reinterpret_cast<std::uintptr_t(__fastcall*)(const char* dvarname, const char* value, bool createifmissing)>(dwProcessBase + 0x3CF54A0);

//MSG
const static auto MSG_InitReadOnly = reinterpret_cast<void(*)(msg_t*, const char*, int)>(dwProcessBase + 0x2DDF230);
const static auto MSG_BeginReading = reinterpret_cast<void(*)(msg_t*)>(dwProcessBase + 0x2DDECB0);
const static auto MSG_ReadByte = reinterpret_cast<std::uint8_t(*)(msg_t*)>(dwProcessBase + 0x2DDF700);
const static auto MSG_ReadData = reinterpret_cast<void(*)(msg_t*, void*, unsigned int, unsigned int)>(dwProcessBase + 0x2DDF940);
const static auto MSG_InfoResponse = reinterpret_cast<bool(*)(void*, LobbyMsg*)>(dwProcessBase + 0x38DD1A0);

//MISC
const static auto G_Chat = reinterpret_cast<void(__fastcall*)(unsigned int controllerIndex)>(dwProcessBase + 0x2761CE0);
const static auto LiveUser_GetXuid = reinterpret_cast<__int64(__fastcall*)(unsigned int controllerIndex)>(dwProcessBase + 0x38C3420);
const static auto Sys_GetTLS = reinterpret_cast<char(__fastcall*)()>(dwProcessBase + 0x3C568E0);
const static auto Live_IsUserInGame = reinterpret_cast<bool(__fastcall*)(int)>(dwProcessBase + 0x38040A0);
const static auto LiveUser_GetXUID = reinterpret_cast<__int64(__fastcall*)(int)>(dwProcessBase + 0x38C3420);
const static auto LiveStats_SetClanTagText = reinterpret_cast<std::uintptr_t(__fastcall*)(int, const char*)>(dwProcessBase + 0x38ABD30);
const static auto CL_StoreConfigString = reinterpret_cast<const char* (*)(int configStringIndex, const char* string)>(dwProcessBase + 0x2875FE0);
const static auto CG_BoldGameMessageCenter = reinterpret_cast<std::uintptr_t(__fastcall*)(int clientNum, const char* msg, int duration)>(dwProcessBase + 0x1482510);
const static auto CG_GameMessage = reinterpret_cast<std::uintptr_t(__fastcall*)(int clientNum, const char* msg)>(dwProcessBase + 0x14823E0);
const static auto dwCommonAddrToNetadr = reinterpret_cast<bool(*)(netadr_t* const, const void* const, const bdSecurityID*)>(dwProcessBase + 0x2ED3660);
const static auto dwRegisterSecIDAndKey = reinterpret_cast<bool(*)(const bdSecurityID*, const bdSecurityKey*)>(dwProcessBase + 0x2ED5640);
const static auto LobbyHostBots_AddBotsToLobby = reinterpret_cast<bool(__fastcall*)(LobbyType lobbyType, int numOfBots, bool isPermanent, LobbyID lobbyId, int passedXUID)>(dwProcessBase + 0x38D6CC0);
const static auto Live_IsDemonwareFetchingDone = reinterpret_cast<bool(*)(const ControllerIndex_t)>(dwProcessBase + 0x3804010);
const static auto Live_IsUserSignedInToDemonware = reinterpret_cast<bool(*)(const ControllerIndex_t)>(dwProcessBase + 0x38040F0);
const static auto dwInstantSendMessage = reinterpret_cast<void(*)(ControllerIndex_t, const std::uint64_t*, unsigned int, char, const void*, unsigned int)>(dwProcessBase + 0x2ED2150);

const static auto LobbyMsg_HandleIM = reinterpret_cast<std::uintptr_t(__fastcall*)(unsigned int a1, __int64 a2, void* a3, unsigned int a4)>(dwProcessBase + 0x38F7780);
const static auto MSG_ReadBigString = reinterpret_cast<const char* (*)(msg_t*)>(dwProcessBase + 0x2DDF310);
const static auto MSG_ReadString = reinterpret_cast<char* (*)(msg_t*, char*, unsigned int)>(dwProcessBase + 0x2DE4CE0);
const static auto MSG_ReadStringLine = reinterpret_cast<char* (*)(msg_t*, char*, unsigned int)>(dwProcessBase + 0x2DE4D60);
const static auto LiveGroups_SearchPlayerByGamertag = reinterpret_cast<std::uintptr_t(__fastcall*)(int controllerIndex, __int64 XUID)>(dwProcessBase + 0x37F0C30);
const static auto LobbyClientMsg_SendDisconnectClient = reinterpret_cast<__int64(__fastcall*)(const ControllerIndex_t controllerIndex, LobbyType lobbyType, __int64 xuid, __int32 disconnectClient)>(dwProcessBase + 0x38CDF10);
const static auto LobbyVM_JoinEvent = reinterpret_cast<__int64(__fastcall*)(int controllerIndex, uint64_t xuid, int joinType)>(dwProcessBase + 0x38F2380);
const static auto CL_GetCurrentCmdNumber = reinterpret_cast<int(__fastcall*)(int localClientNum)>(dwProcessBase + 0x277B2C0);

//TYPEDEFS
typedef bool(__fastcall* dwNetadrToCommonAddrT)(netadr_t, void*, const unsigned int, bdSecurityID*);
typedef bool(__fastcall* NET_OutOfBandPrintT)(netsrc_t, netadr_t, const char*);
typedef __int64(__fastcall* Com_GetClientDObjT)(int* handle, int a2);
typedef __int64(__fastcall* CG_DObjGetWorldTagPosInternalT)(__int64 centity, __int64 DObj, int tag, float* whatever, float* pos, int something);
typedef bool(__fastcall* tCG_CanSeeFriendlyHeadTrace)(int localClientNum, __int64 centity_t, __int64 start, __int64 end);
typedef bool(__fastcall* CG_IsEntityFriendlyNotEnemyT)(int localClientNum, __int64 centity_t);
typedef char(__fastcall* draw2DInternalT)(int);
typedef void(__fastcall* CG_BulletHitEventT)(LocalClientNum_t localClientNum, int sourceEntityNum, int targetEntityNum, __int64 weapon, vec3_t* startPos, vec3_t* position, vec3_t* normal, vec3_t* seeThruDecalNormal, int surfType, int* _event, __int64 eventParam);
typedef bool(__fastcall* WorldPosToScreenPosT)(int localClientNum, float* worldPos, float* out);
typedef __int64(__fastcall* CG_PredictPlayerStateT)(unsigned int localClientNum);
typedef __int64(__fastcall* CL_WritePacketT)(unsigned int localClientNum);
typedef const char* (__fastcall* CL_GetConfigStringT)(int);
extern dwNetadrToCommonAddrT dwNetadrToCommonAddr;
extern NET_OutOfBandPrintT NET_OutOfBandPrint;
extern Com_GetClientDObjT Com_GetClientDObj;
extern CG_DObjGetWorldTagPosInternalT CG_DObjGetWorldTagPosInternal;
extern tCG_CanSeeFriendlyHeadTrace CG_CanSeeFriendlyHeadTrace;
extern CG_IsEntityFriendlyNotEnemyT CG_IsEntityFriendlyNotEnemy;
extern draw2DInternalT draw2DInternal;
extern CG_PredictPlayerStateT CG_PredictPlayerState;
extern CL_WritePacketT CL_WritePacket;
extern CG_BulletHitEventT CG_BulletHitEvent;
extern WorldPosToScreenPosT WorldPosToScreenPos;
extern CL_GetConfigStringT CL_GetConfigString;
extern DWORD_PTR nameBuffer;

//XSESSION_INFO get_session_info(const InfoResponseLobby& lobby);
//netadr_t get_net_adr(const XSESSION_INFO& info);
bool send_oob(const netadr_t& target, const std::string& data, const netsrc_t& sock = NS_SERVER);
__int64 GetClientDObj(int handle);
vec3_t CG_DObjGetWorldTagPos(__int64 centity, __int64 DObj, int tag, float* whatever);
bool is_in_array(std::string cmp1, std::vector<std::string> cmp2);
bool is_in_number_array(int cmp1, std::vector<int> cmp2);
bones updateBones(int i);
uintptr_t get_entity(uintptr_t array, int index);
vec2_t WorldToScreen(vec3_t vWorldLocation, int type = 1);
bool IsEntityFriendly(__int64 centity_t);
bool IsTargetVisible(__int64 centity_t, vec3_t start, vec3_t end);
clientactive* GetClientActive();
clientactivet* GetClientActivet();
vec3_t CG_GetPlayerViewOrigin(__int64 playerstate);