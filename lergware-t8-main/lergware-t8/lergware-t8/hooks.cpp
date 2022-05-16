#include "hooks.h"

vec3_t best = { 0 };
bool visible = false;
bool target = false;

std::vector<std::string> legit_packets = { "connectResponse", "statresponse", "loadingnewmap", "LM", "disconnect", "loadoutResponse", "LMgetinfo", "infoResponse", "statusResponse", "keyAuthorize", "error", "print", "echo", "fastrestart", "ping", "pinga " };

std::vector<int> legit_config_str = { 15, 8652, 8666, 12, 8651, 14, 9295, 9068, 9166, 8661, 8650, 8661, 326, 9067 };
std::vector<int> crashing_config_str = { 16, 17, 18, 19 };

uintptr_t saved_zombiePlayerUsesUtilityClip;
namespace Hook
{
	char ClampMove(char value)
	{
		if (value < -128)
			return -128;

		if (value > 127)
			return 127;

		return value;
	}

	bool is_in_game()
	{
		return Live_IsUserInGame(0);
	}

	__int64 hkWritePacket(int arg)
	{
		auto packet = CL_WritePacket(arg);

		if (!is_in_game()) return packet;

		auto clientactive = GetClientActive();

		int cmdNumber = GetClientActivet()->cmdNumber;

		usercmd_s* cmd = clientactive->GetUserCmd(cmdNumber);

		if (AntiAim)
		{
			float forwardmove = cmd->forwardmove;
			float sidemove = cmd->rightmove;
			auto angles = cmd->angles;
			
			vec2_t vAntiAimAngles;
			/*
			vAntiAimAngles.x = *(float*)(cgArray + 0x166FD0);

			if (vAntiAimAngles.x > 85.0f) vAntiAimAngles.x = 85.0f;
			if (vAntiAimAngles.x < -85.0f) vAntiAimAngles.x = -85.0f;

			vAntiAimAngles.y = *(float*)(cgArray + 0x166FD4) - 170.0f;

			while (vAntiAimAngles.y > 180.0f) vAntiAimAngles.y -= 360.0f;
			while (vAntiAimAngles.y < -180.0f) vAntiAimAngles.y += 360.0f;

			vAntiAimAngles.x -= *(float*)(cgArray + 0x164E48);
			vAntiAimAngles.y -= *(float*)(cgArray + 0x164E4C);*/

			//angles[0] = AngleToShort(vAntiAimAngles.x);
			//angles[1] = AngleToShort(vAntiAimAngles.y);

			cmd->rightmove = 0;
			cmd->forwardmove = 0;
		}

		return packet;
	}

	__int64 hkCG_PredictPlayerState(unsigned int arg)
	{

		if (SilentAim || Auto_Fire)
		{
			auto clientactive = GetClientActive();

			int cmdNumber = GetClientActivet()->cmdNumber;

			usercmd_s* current = clientactive->GetUserCmd(cmdNumber);
			usercmd_s* old = clientactive->GetUserCmd(cmdNumber - 1);
			usercmd_s* new_cmd = clientactive->GetUserCmd(cmdNumber + 1);

			*new_cmd = *current;
			++GetClientActivet()->cmdNumber;

			static int backup[3];

			old->angles[0] = backup[0];
			old->angles[1] = backup[1];
			old->angles[2] = backup[2];

			backup[0] = current->angles[0];
			backup[1] = current->angles[1];
			backup[2] = current->angles[2];

			old->serverTime++;
			current->serverTime--;

			if (SilentAim && target)
			{
				auto angles = aim::CalcAngles(best);

				old->angles[0] += AngleToShort(angles.x);
				old->angles[1] += AngleToShort(angles.y);

				target = false;

				if (Auto_Fire)
					aim::need_shoot = true;
			}

			if (aim::need_shoot == true && Auto_Fire)
			{
				current->button_bits[0] |= 0x80000000;
				current->button_bits[1] |= 0x20000000;

				aim::need_shoot = false;
			}
		}

		return CG_PredictPlayerState(arg);
	}

	void hkDraw2D()
	{
		players.clear();

		if ((cgArray + 0x3E4C54) <= 0 || !cg_entitiesArray || !is_in_game()) return; //draw2DInternal(a1);

		for (auto& tracer : tracers)
		{
			if (const auto delta{ std::chrono::high_resolution_clock::now() - tracer.time };
				delta <= 3s)
			{
				tracer.start_screen = WorldToScreen(tracer.start, 1);
				tracer.end_screen = WorldToScreen(tracer.end, 1);
				auto dist = round(sqrtf((tracer.end.x - *(float*)(cgArray + 1479192)) * (tracer.end.x - *(float*)(cgArray + 1479192)) + (tracer.end.y - *(float*)(cgArray + 1479196)) * (tracer.end.y - *(float*)(cgArray + 1479196)) + (tracer.end.z - *(float*)(cgArray + 1479200)) * (tracer.end.z - *(float*)(cgArray + 1479200)))) / 100;
				if (dist <= 10)
				{
					tracer.radius = 6;
				}
				else
				{
					tracer.radius = 3;
				}
				
			}
		}

		if (thirdPerson)
		{
			*(bool*)(cgArray + 0x3CFF58) = 1;
		}

		if (no_recoil) {
			float* recoilSpeed = (float*)(cgArray + 1458160);
			recoilSpeed[0] = 0;
			recoilSpeed[1] = 0;
			float* kickAVel = (float*)(cgArray + 4780744);
			kickAVel[0] = 0;
			kickAVel[1] = 0;
			kickAVel[2] = 0;
		}

		int min = INT_MAX, bestTarget;

		RECT desktop;
		const HWND hDesktop = GetDesktopWindow();
		GetWindowRect(hDesktop, &desktop);
		vec2_t res = { desktop.right, desktop.bottom };
		best = { 0, 0, 0 };

		//auto clientinfo = cgArray + 0x3E4C54;
		for (int i = 0; i < 1024; i++)
		{
			uintptr_t centity = cg_entitiesArray + (3560 * i);
			auto type = *(__int16*)(centity + 0x66A);
			auto origin = *(vec3_t*)(centity + 0x34);
			auto clientNum = *(DWORD*)(centity + 1080);
			auto valid = *(BYTE*)(centity + 3548);

			if (type == 1 && valid && clientNum != *(int*)cgArray)
			{
				//auto client_info = clientinfo + (0x10A8 * clientNum);

				player p{};
				p.bone_array = updateBones(i);
				vec3_t head_pos = origin;
				head_pos.z += 70.f;
				p.bone_array.head = WorldToScreen(head_pos);
				p.team = IsEntityFriendly(centity);
				p.address = centity;
				p.bone_array.origin = WorldToScreen(origin);
				/*
				p.health = *(int*)(client_info + 0x158);
				for (int j = 0; j < 16; j++) {
					p.player_name[j] = *(char*)((client_info + 0x32) + (0x1 * j));
				}
				*/
				if (p.bone_array.origin.x != 0 && p.bone_array.origin.y != 0)
					players.push_back(p);

				if (p.team) continue;

				auto BonePos = aim::customScan(p);
				auto pos = WorldToScreen(BonePos);
				if (BonePos.x == 0) continue;

				float dist1 = sqrt(pow(desktop.right / 2 - pos.x, 2) + pow(desktop.bottom / 2 - pos.y, 2));
				if (dist1 < min)
				{
					best = BonePos;

					bestTarget = i;
					min = dist1;

					target = true;
				} 
			}
		}
		if (target && best.x != 0 && best.y != 0 && !SilentAim && aimbot_active)
		{
			aim::Standard(best);
		}



		return; //draw2DInternal(a1);
	}

	LONG CALLBACK hkCL_ConnectionlessCMD(LPEXCEPTION_POINTERS ex)
	{

		time_t now = time(0);
		#pragma warning(suppress : 4996)
		char* dt = ctime(&now);

		char* message = (char*)ex->ContextRecord->Rdi;
		std::string sender = utils::adr_to_string((netadr_t*)ex->ContextRecord->R15);

		if (is_in_array(message, legit_packets))
		{
			ex->ContextRecord->Rip += 3;
			ex->ContextRecord->Rcx = (__int32)ex->ContextRecord->R14;
			return EXCEPTION_CONTINUE_EXECUTION;
		}

		std::string str = std::string(message) + " received " + sender;

		std::string no_sender = std::string(message) + " received";

		ImGui::InsertNotification({ ImGuiToastType_OOB, 8000, str.data() });

		std::ofstream myfile;
		myfile.open("lergware\\logs\\logs.txt""", std::ios_base::app);
		myfile << dt << message << " Received from " << sender << "\n";
		myfile.close();

		ex->ContextRecord->Rip = (dwProcessBase + 0x2857BFA);

		return EXCEPTION_CONTINUE_EXECUTION;

	}

	LONG CALLBACK hkMsg_InfoResponsePackage(LPEXCEPTION_POINTERS ex)
	{

		Msg_InfoResponse* info_response = (Msg_InfoResponse*)ex->ContextRecord->Rcx;
		spoofing::spoof_info_response(info_response);

		ex->ContextRecord->R8 = ex->ContextRecord->Rcx;
		ex->ContextRecord->Rip += 0x3;
		return EXCEPTION_CONTINUE_EXECUTION;
	}

	void hkLobbyVM_OnDisconnect(LobbySession* session, __int64 XUID, LobbyDisconnectClient client)
	{

		time_t now = time(0);
		#pragma warning(suppress : 4996)
		char* dt = ctime(&now);

		std::string kick = std::to_string(client);

		kick = kicks[client];

		ImGui::InsertNotification({ ImGuiToastType_Disconnect, 3000, "%s Blocked", kick });

		std::ofstream myfile;
		myfile.open("lergware\\logs\\logs.txt""", std::ios_base::app);
		myfile << dt << kick << " Received From " << "\n";
		myfile.close();

		return;
	}

	__int64 hkdwInstantDispatchMessage(__int64 sender_id, unsigned int controllerIndex, __int64 msg, unsigned int messageSize)
	{
		const auto message{ reinterpret_cast<const char*>(msg) };
		const auto message_size{ messageSize };

		friends::instant_message::handle_message(sender_id, message, message_size);

		return 0;
	}

	const char* __fastcall hkCL_GetConfigString(std::int32_t configStringIndex)
	{
		if (is_in_number_array(configStringIndex, legit_config_str)) return CL_GetConfigString(configStringIndex);

		if (is_in_number_array(configStringIndex, crashing_config_str))
		{
			CL_StoreConfigString(configStringIndex, "");

			ImGui::InsertNotification({ ImGuiToastType_Warning, 8000, "Config string prevented" });

			return CL_GetConfigString(configStringIndex);
		}

		return CL_GetConfigString(configStringIndex);
	}

	__int64 hkLiveInventory_GetItemQuantity(int controllerIndex, int itemId)
	{
		return 1;
	}

	LONG CALLBACK hookHandler(LPEXCEPTION_POINTERS ex)
	{
		if (ex->ExceptionRecord->ExceptionCode == STATUS_SINGLE_STEP)
		{
			if (ex->ContextRecord->Rip == (uintptr_t)(dwProcessBase + 0x836AEB))
			{
				cgArray = ex->ContextRecord->Rcx;

				ex->ContextRecord->Rbx += ex->ContextRecord->Rcx;
				ex->ContextRecord->Rip += 3;
				ex->ContextRecord->Dr0 = (uintptr_t)(dwProcessBase + 0x7CA923);

				//hkDraw2D();
			}
			else if (ex->ContextRecord->Rip == (uintptr_t)(dwProcessBase + 0x7CA923))
			{
				cg_entitiesArray = ex->ContextRecord->Rcx;

				ex->ContextRecord->Rdi += ex->ContextRecord->Rcx;
				ex->ContextRecord->Rip += 3;
				ex->ContextRecord->Dr0 = (uintptr_t)(dwProcessBase + 0x836AEB);
			}

			else if (ex->ContextRecord->Rip == (uintptr_t)(dwProcessBase + 0x38DD1BB))
			{
				return hkMsg_InfoResponsePackage(ex);
			}
			else if (ex->ContextRecord->Rip == (uintptr_t)LobbyVM_OnDisconnect)
			{
				ex->ContextRecord->Rip = (uintptr_t)hkLobbyVM_OnDisconnect;
			}
			else if (ex->ContextRecord->Rip == (uintptr_t)dwInstantDispatchMessage)
			{
				ex->ContextRecord->Rip = (uintptr_t)hkdwInstantDispatchMessage;
			}
			else if (ex->ContextRecord->Rip == (uintptr_t)(dwProcessBase + 0x37F6FD0))
			{
				ex->ContextRecord->Rip = (uintptr_t)hkLiveInventory_GetItemQuantity;
			}
			

			return EXCEPTION_CONTINUE_EXECUTION;
		}
		else if (ex->ContextRecord->Rip == (uintptr_t)(dwProcessBase + 0x3D238D))
		{
			ex->ContextRecord->Rcx = saved_draw2D;
			
			hkDraw2D();
		}
		else if (ex->ContextRecord->Rip == (uintptr_t)(dwProcessBase + 0x3E7549)) {
			ex->ContextRecord->Rsi = saved_packetDup;
		}
		else if (ex->ContextRecord->Rip == (uintptr_t)(dwProcessBase + 0x3DA1D2)) {

			ex->ContextRecord->Rcx = saved_packetDup;
			if (AntiAim && !target)
			{
				auto clientactive = GetClientActive();
				int cmdNumber = GetClientActivet()->cmdNumber;
				usercmd_s* cmd = clientactive->GetUserCmd(cmdNumber);

				vec2_t vAntiAimAngles{};
				static float flAngle = 0.0f;

				if (flAngle > 180.0f)
					flAngle -= 360.0f;

				vAntiAimAngles.y = flAngle - *(float*)(cgArray + 0x164E48);

				flAngle += 45.0f;

				float oldYaw = ShortToAngle(cmd->angles[1]);
				auto forward_move = cmd->forwardmove;
				auto right_move = cmd->rightmove;

				cmd->angles[0] = AngleToShort(vAntiAimAngles.x);
				cmd->angles[1] = AngleToShort(vAntiAimAngles.y);

				auto Yaw = ShortToAngle(cmd->angles[1]);

				float flDelta = DegreesToRadians(Yaw - oldYaw);

				cmd->forwardmove = ClampMove((char)(cosf(flDelta) * forward_move - sinf(flDelta) * right_move));
				cmd->rightmove = ClampMove((char)(sinf(flDelta) * forward_move + cosf(flDelta) * right_move));

			}
			return EXCEPTION_CONTINUE_EXECUTION;
		}
		else {
			std::ofstream myfile;
			myfile.open("lergware\\InvalidPointers.txt", std::ios_base::app);
			myfile << "Exception at: " << std::hex << ex->ContextRecord->Rip  << " *" << std::hex << ex->ContextRecord->Rip  - dwProcessBase << "* "  << "\n";
			myfile << "dwProcessBase: " << std::hex << (DWORD64)GetModuleHandleA(NULL) << "\n";
			myfile.close();
		}

		return EXCEPTION_CONTINUE_EXECUTION;
	}

	void initalize()
	{
		SetUnhandledExceptionFilter(hookHandler);

		HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
		THREADENTRY32 te32;
		hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

		if (hThreadSnap)
		{
			te32.dwSize = sizeof(THREADENTRY32);

			if (!Thread32First(hThreadSnap, &te32))
			{
				CloseHandle(hThreadSnap);
				return;
			}

			do
			{
				if (te32.th32OwnerProcessID == GetCurrentProcessId() && te32.th32ThreadID != GetCurrentThreadId())
				{
					HANDLE hThread = OpenThread(THREAD_GET_CONTEXT | THREAD_SET_CONTEXT | THREAD_SUSPEND_RESUME, 0, te32.th32ThreadID);
					if (hThread)
					{
						CONTEXT context;
						context.ContextFlags = CONTEXT_DEBUG_REGISTERS;
						SuspendThread(hThread);

						if (GetThreadContext(hThread, &context))
						{
							//context.Dr0 = (uintptr_t)(dwProcessBase + 0x37F6FD0); //LiveInventory_GetItemQuantity
							context.Dr0 = (uintptr_t)(dwProcessBase + 0x7CA923); //cg_entitiesArray
							context.Dr1 = (uintptr_t)(dwProcessBase + 0x38DD1BB); //Munipulation;
							context.Dr2 = (uintptr_t)LobbyVM_OnDisconnect;
							context.Dr3 = (uintptr_t)dwInstantDispatchMessage;

							context.Dr7 = (1 << 0) | (1 << 2) | (1 << 4) | (1 << 6);

							SetThreadContext(hThread, &context);
						}

						ResumeThread(hThread);
						CloseHandle(hThread);
					}
				}
			} while (Thread32Next(hThreadSnap, &te32));
			CloseHandle(hThreadSnap);
		}

		saved_packetDup = *(__int64*)(dwProcessBase + 0xF9FA310);
		saved_draw2D = *(__int64*)(dwProcessBase + 0x6894FA8);
	}
	void on_frame()
	{
		static bool enabled;

		if (is_in_game() && !enabled)
		{

			*(__int64*)(dwProcessBase + 0xF9FA310) = 1;
			*(__int64*)(dwProcessBase + 0x6894FA8) = 2;
			//MH_CreateHook((void*)(dwProcessBase + 0x20CB670), hkCG_BulletHitEvent, (void**)&CG_BulletHitEvent);

			//MH_CreateHook((void*)(dwProcessBase + 0x834C90), hkDraw2D, (void**)&draw2DInternal);
			//MH_CreateHook((void*)(dwProcessBase + 0x27D2590), hkWritePacket, (void**)&CL_WritePacket);
			MH_CreateHook((void*)(dwProcessBase + 0xC148F0), hkCG_PredictPlayerState, (void**)&CG_PredictPlayerState);
			MH_CreateHook((void*)(dwProcessBase + 0x277B280), hkCL_GetConfigString, (void**)&CL_GetConfigString);
			MH_EnableHook(MH_ALL_HOOKS);

			enabled = true;

			ImGui::InsertNotification({ ImGuiToastType_Info, 5000, "ENABLED HOOKS" });
		}
		if (!is_in_game() && enabled)
		{
			*(__int64*)(dwProcessBase + 0xF9FA310) = saved_packetDup;
			*(__int64*)(dwProcessBase + 0x6894FA8) = saved_draw2D;

			MH_RemoveHook((void*)(dwProcessBase + 0x27D2590));
			MH_RemoveHook((void*)(dwProcessBase + 0xC148F0));
			MH_RemoveHook((void*)(dwProcessBase + 0x277B280));
			MH_DisableHook((void*)(dwProcessBase + 0x27D2590));
			MH_DisableHook((void*)(dwProcessBase + 0xC148F0));
			MH_DisableHook((void*)(dwProcessBase + 0x277B280));

			players.clear();

			enabled = false;
		}
	}
}