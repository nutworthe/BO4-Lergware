#pragma once
#include "includes.h"

typedef uint64_t LobbyID;

typedef unsigned __int64 __uint64_t;

typedef __uint64_t bdUInt64;

typedef bdUInt64 bdEntityID;

typedef bdEntityID bdOnlineUserID;

typedef bdOnlineUserID XUID;

using ControllerIndex_t = std::uint32_t;
using ClientNum_t = std::uint32_t;
using namespace std;

enum JoinType
{
	JOIN_TYPE_NORMAL = 0x0,
	JOIN_TYPE_PLAYLIST = 0x1,
	JOIN_TYPE_FRIEND = 0x2,
	JOIN_TYPE_INVITE = 0x3,
	JOIN_TYPE_PARTY = 0x4,
	JOIN_TYPE_GROUPS = 0x5,
	JOIN_TYPE_COUNT = 0x6,
};

enum class LocalClientNum_t
{
	INVALID_LOCAL_CLIENT = 0xFFFFFFFF,
	LOCAL_CLIENT_0 = 0x0,
	LOCAL_CLIENT_FIRST = 0x0,
	LOCAL_CLIENT_KEYBOARD_AND_MOUSE = 0x0,
	LOCAL_CLIENT_1 = 0x1,
	LOCAL_CLIENT_2 = 0x2,
	LOCAL_CLIENT_3 = 0x3,
	LOCAL_CLIENT_COUNT = 0x4,
};


enum PackageType
{
	PACKAGE_TYPE_NONE = 0x0,
	PACKAGE_TYPE_WRITE = 0x1,
	PACKAGE_TYPE_READ = 0x2,
};

enum MsgType
{
	MESSAGE_TYPE_NONE = 0xFFFFFFFF,
	MESSAGE_TYPE_INFO_REQUEST = 0x0,
	MESSAGE_TYPE_INFO_RESPONSE = 0x1,
	MESSAGE_TYPE_LOBBY_STATE_PRIVATE = 0x2,
	MESSAGE_TYPE_LOBBY_STATE_GAME = 0x3,
	MESSAGE_TYPE_LOBBY_STATE_GAMEPUBLIC = 0x4,
	MESSAGE_TYPE_LOBBY_STATE_GAMECUSTOM = 0x5,
	MESSAGE_TYPE_LOBBY_STATE_GAMETHEATER = 0x6,
	MESSAGE_TYPE_LOBBY_HOST_HEARTBEAT = 0x7,
	MESSAGE_TYPE_LOBBY_HOST_DISCONNECT = 0x8,
	MESSAGE_TYPE_LOBBY_HOST_DISCONNECT_CLIENT = 0x9,
	MESSAGE_TYPE_LOBBY_HOST_LEAVE_WITH_PARTY = 0xA,
	MESSAGE_TYPE_LOBBY_CLIENT_HEARTBEAT = 0xB,
	MESSAGE_TYPE_LOBBY_CLIENT_DISCONNECT = 0xC,
	MESSAGE_TYPE_LOBBY_CLIENT_RELIABLE_DATA = 0xD,
	MESSAGE_TYPE_LOBBY_CLIENT_CONTENT = 0xE,
	MESSAGE_TYPE_LOBBY_MODIFIED_STATS = 0xF,
	MESSAGE_TYPE_JOIN_LOBBY = 0x10,
	MESSAGE_TYPE_JOIN_RESPONSE = 0x11,
	MESSAGE_TYPE_JOIN_AGREEMENT_REQUEST = 0x12,
	MESSAGE_TYPE_JOIN_AGREEMENT_RESPONSE = 0x13,
	MESSAGE_TYPE_JOIN_COMPLETE = 0x14,
	MESSAGE_TYPE_JOIN_MEMBER_INFO = 0x15,
	MESSAGE_TYPE_SERVERLIST_INFO = 0x16,
	MESSAGE_TYPE_PEER_TO_PEER_CONNECTIVITY_TEST = 0x17,
	MESSAGE_TYPE_PEER_TO_PEER_INFO = 0x18,
	MESSAGE_TYPE_LOBBY_MIGRATE_TEST = 0x19,
	MESSAGE_TYPE_LOBBY_MIGRATE_ANNOUNCE_HOST = 0x1A,
	MESSAGE_TYPE_LOBBY_MIGRATE_START = 0x1B,
	MESSAGE_TYPE_INGAME_MIGRATE_TO = 0x1C,
	MESSAGE_TYPE_INGAME_MIGRATE_NEW_HOST = 0x1D,
	MESSAGE_TYPE_VOICE_PACKET = 0x1E,
	MESSAGE_TYPE_VOICE_RELAY_PACKET = 0x1F,
	MESSAGE_TYPE_DEMO_STATE = 0x20,
	MESSAGE_TYPE_COUNT = 0x21,
};

enum netadrtype_t
{
	NA_BOT = 0x0,
	NA_BAD = 0x1,
	NA_LOOPBACK = 0x2,
	NA_RAWIP = 0x3,
	NA_IP = 0x4,
};

enum netsrc_t
{
	NS_NULL = 0xFFFFFFFF,
	NS_CLIENT1 = 0x0,
	NS_CLIENT2 = 0x1,
	NS_CLIENT3 = 0x2,
	NS_CLIENT4 = 0x3,
	NS_SERVER = 0x4,
	NS_MAXCLIENTS = 0x4,
	NS_PACKET = 0x5,
};

enum LobbyNetworkMode
{
	LOBBY_NETWORKMODE_INVALID = 0xFFFFFFFF,
	LOBBY_NETWORKMODE_UNKNOWN = 0x0,
	LOBBY_NETWORKMODE_LAN = 0x1,
	LOBBY_NETWORKMODE_LIVE = 0x2,
	LOBBY_NETWORKMODE_COUNT = 0x3,
	LOBBY_NETWORKMODE_LOCAL = 0x1,
};

enum LobbyMainMode
{
	LOBBY_MAINMODE_INVALID = 0xFFFFFFFF,
	LOBBY_MAINMODE_CP = 0x0,
	LOBBY_MAINMODE_MP = 0x1,
	LOBBY_MAINMODE_ZM = 0x2,
	LOBBY_MAINMODE_COUNT = 0x3,
};

enum IMType
{
	JOIN_REQUEST = 0x0,
	JOIN_REPLY = 0x1,
	INVITE = 0x2,
	UPDATE_INVITE_INFO = 0x3,
	NUM_INSTANT_MESSAGE_TYPES = 0x4,
};

enum LobbyModule
{
	LOBBY_MODULE_INVALID = 0xFFFFFFFF,
	LOBBY_MODULE_HOST = 0x0,
	LOBBY_MODULE_CLIENT = 0x1,
	LOBBY_MODULE_PEER_TO_PEER = 0x3,
	LOBBY_MODULE_COUNT = 0x4,
};

enum LobbyMode
{
	LOBBY_MODE_INVALID = 0xFFFFFFFF,
	LOBBY_MODE_PUBLIC = 0x0,
	LOBBY_MODE_CUSTOM = 0x1,
	LOBBY_MODE_THEATER = 0x2,
	LOBBY_MODE_ARENA = 0x3,
	LOBBY_MODE_FREERUN = 0x4,
	LOBBY_MODE_COUNT = 0x5,
};

enum SessionActive
{
	SESSION_INACTIVE = 0x0,
	SESSION_KEEP_ALIVE = 0x1,
	SESSION_ACTIVE = 0x2,
};

enum LobbyDisconnectClient
{
	LOBBY_DISCONNECT_CLIENT_INVALID = 0xFFFFFFFF,
	LOBBY_DISCONNECT_CLIENT_DROP = 0x0,
	LOBBY_DISCONNECT_CLIENT_KICK = 0x1,
	LOBBY_DISCONNECT_CLIENT_BADDLC = 0x2,
	LOBBY_DISCONNECT_CLIENT_KICK_PARTY = 0x3,
	LOBBY_DISCONNECT_CLIENT_HOSTRELOAD = 0x4,
	LOBBY_DISCONNECT_CLIENT_NOPARTYCHAT = 0x5,
};

enum LobbyType
{
	LOBBY_TYPE_INVALID = 0xFFFFFFFF,
	LOBBY_TYPE_PRIVATE = 0x0,
	LOBBY_TYPE_GAME = 0x1,
	LOBBY_TYPE_TRANSITION = 0x2,
	LOBBY_TYPE_COUNT = 0x3,
};

struct vec2_t
{
	float x, y;
};

struct vec3_t
{
	float x, y, z;
};

struct bones
{
	vec2_t origin, head, j_headpos_screen, j_neckpos_screen, j_spineupperpos_screen, j_spinelowerpos_screen,
		j_shoulder_lepos_screen, j_shoulder_ripos_screen, j_elbow_lepos_screen,
		j_elbow_ripos_screen, j_wrist_ripos_screen, j_wrist_lepos_screen, j_knee_lepos_screen,
		j_knee_ripos_screen, j_ankle_ripos_screen, j_ankle_lepos_screen;

	vec3_t j_neckpos, j_headpos, j_spineupperpos, j_spinelowerpos, j_shoulder_lepos, j_shoulder_ripos, j_elbow_lepos, j_elbow_ripos, j_wrist_ripos, j_wrist_lepos,
		j_knee_lepos, j_knee_ripos, j_ankle_ripos, j_ankle_lepos, j_kneebuldge_lepos, j_kneebuldge_ripos, j_hip_ripos, j_hip_lepos, j_eyeball_ripos, j_eyeball_lepos,
		j_ball_ripos, j_ball_lepos;
};

struct player
{
	uintptr_t address;

	bool team;

	int health;
	float distance;
	char player_name[16];
	bool targeted;

	bones bone_array;
};

struct bullet_tracer
{
	float opacity;

	vec3_t start;
	vec3_t end;

	vec2_t start_screen;
	vec2_t end_screen;

	int radius;

	std::chrono::time_point<std::chrono::high_resolution_clock> time{};
};

struct usercmd_s
{
public:
	char pad0[0x20];                //0x0
	int serverTime;                 //0x20
	int button_bits[2];             //0x24
	char pad1[0x14];                //0x2C
	int angles[3];                  //0x40
	char pad2[0x1B];                //0x4C
	char forwardmove;               //0x67
	char rightmove;                 //0x68
	char pad3[0x7];                 //0x69
}; //size 0x70

struct clientactive
{
public:
	char _pad0x02[0x25A0];
	usercmd_s usercmds[128];

	usercmd_s* GetUserCmd(int cmdNumber)
	{
		return &usercmds[cmdNumber & 0x7F];
	}
};

struct clientactivet
{
public:
	char _pad0x0[0x2290];
	int cmdNumber;

	char _pad0x01[0x1F8];
	vec3_t viewAngles;
};

struct JoinSessionMessage
{
	IMType mType;
	char pad[0x74];
};

#pragma pack(push, 1)
struct XNADDR
{
	char pad[0x3C];
	union
	{
		std::uint8_t ip[4];
		std::uint32_t inaddr;
	};

	std::uint16_t port;
	char pad2[0x1];
};
#pragma pack(pop)

struct bdSecurityID
{
	std::uint64_t id;
};

struct bdSecurityKey
{
	char ab[16];
};

struct XSESSION_INFO
{
	bdSecurityID sessionID;
	XNADDR hostAddress;
	bdSecurityKey keyExchangeKey;
};

struct netadr_t
{
	union
	{
		std::uint8_t ip[4];
		std::uint32_t inaddr;
	};

	std::uint16_t port;
	netadrtype_t type;
	netsrc_t localNetID;
};

struct msg_t
{
	char* data;
	int maxsize;
	int cursize;
	int readcount;
	char pad[0x21];
	bool overflowed;
	bool readOnly;
	char pad2[0x2];
};

struct LobbyMsg
{
	msg_t msg;
	MsgType type;
	PackageType packageType;
	char encodeFlags;
};

struct LobbyParams
{
	LobbyNetworkMode networkMode;
	LobbyMainMode mainMode;
};

struct SerializedAdr
{
	bool valid;
	XNADDR xnaddr;
};

struct InfoResponseLobby
{
	bool isValid;
	std::uint64_t hostXuid;
	char hostName[32];
	bdSecurityID secId;
	bdSecurityKey secKey;
	SerializedAdr serializedAdr;
	LobbyParams lobbyParams;
};

struct Msg_InfoRequest
{
	std::uint32_t nonce;
};

struct Msg_InfoResponse
{
	std::uint32_t nonce;
	int uiScreen;
	std::uint8_t natType;
	InfoResponseLobby lobby[3];
};

struct SessionInfo
{
	bool inSession;
	netadr_t netAdr;
	time_t lastMessageSentToPeer;
};

struct FixedClientInfo
{
	std::uint64_t xuid;
	char pad2[0xAC];
	char gamertag[32];
};

struct ActiveClient
{
	char pad[0x2C0];
	FixedClientInfo fixedClientInfo;
	SessionInfo sessionInfo[2];
};

struct SessionClient
{
	char pad[0x18];
	ActiveClient* activeClient;
};

struct LobbySession
{
	LobbyModule module;
	LobbyType type;
	LobbyMode mode;
	char pad[0x34];
	SessionActive active;
	char pad2[0x121D4];
};