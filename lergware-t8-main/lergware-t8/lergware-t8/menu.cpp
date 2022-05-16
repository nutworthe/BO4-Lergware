#include "menu.h"
using namespace std;

vector<string> split(string str, char delimiter) {
	vector<string> internal;
	stringstream ss(str); // Turn the string into a stream. 
	string tok;

	while (getline(ss, tok, delimiter)) {
		internal.push_back(tok);
	}

	return internal;
}

namespace menu
{

	bool is_open()
	{
		return open;
	}

	void draw()
	{
		if (style_sel == 0)
			runstyle();
		else if (style_sel == 1)
			alternatestyle();
		else if (style_sel == 2)
			blackstyle();
		else if (style_sel == 3)
			rgbstyle();

		if (is_open())
		{

			ImGui::Begin("lergware", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

			ImVec2 screenSize = ImGui::GetIO().DisplaySize;
			FX(ImGui::GetWindowDrawList(), screenSize);

			if (ImGui::BeginTabBar("Main", ImGuiTabBarFlags_NoTooltip))
			{
				if (ImGui::BeginTabItem("Aimbot"))
				{
					ImGui::Dummy(ImVec2(0.0f, 5.0f));

					ImGui::Checkbox("Active", &aimbot_active);

					ImGui::Dummy(ImVec2(0.0f, 3.0f));

					ImGui::Checkbox("Silent Aim", &SilentAim);

					ImGui::Dummy(ImVec2(0.0f, 3.0f));

					ImGui::Checkbox("Auto-Fire", &Auto_Fire);

					ImGui::Dummy(ImVec2(0.0f, 3.0f));

					ImGui::Checkbox("Visible Check", &VisCheck);

					ImGui::Dummy(ImVec2(0.0f, 3.0f));

					ImGui::Checkbox("ADS Check", &ads_check);

					ImGui::Dummy(ImVec2(0.0f, 3.0f));

					ImGui::Checkbox("No Recoil", &no_recoil);

					ImGui::Dummy(ImVec2(0.0f, 3.0f));

					ImGui::SetNextItemWidth(160);

					static std::string previewValue = "";

					if (ImGui::BeginCombo("Bones", previewValue.c_str()))
					{

						previewValue = "";
						std::vector<std::string> vec;
						for (size_t i = 0; i < IM_ARRAYSIZE(boneSel); i++)
						{
							ImGui::Selectable(boneSel[i], &selected[i], ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);

							if (selected[i]) {
								vec.push_back(boneSel[i]);
							}
						}
						for (size_t i = 0; i < vec.size(); i++)
						{

							if (vec.size() == 1)
								previewValue += vec.at(i);
							else if (!(i == vec.size()))
								previewValue += vec.at(i) + ",";
							else
								previewValue += vec.at(i);

						}

						ImGui::EndCombo();
					}

					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Visuals"))
				{

					ImGui::Dummy(ImVec2(0.0f, 6.0f));

					if (ImGui::BeginTabBar("##ESP", ImGuiTabBarFlags_NoTooltip))
					{

						ImGui::Dummy(ImVec2(0.0f, 3.0f));

						if (ImGui::BeginTabItem("ESP"))
						{

							ImGui::Checkbox("Active", &visuals_active);

							ImGui::SameLine();

							ImGui::Dummy(ImVec2(325.f, 0.0f));

							ImGui::SameLine();

							ImGui::Text("Enemies Only");

							ImGui::SameLine();

							ImGui::Checkbox("##EnemiesOnly", &enemies_only);

							ImGui::Dummy(ImVec2(0.0f, 1.49f));

							ImGui::Separator();

							ImGui::Dummy(ImVec2(0.0f, 1.49f));

							ImGui::Checkbox("Filled", &filled_boxes);

							ImGui::Dummy(ImVec2(0.0f, 3.0f));

							ImGui::Checkbox("Unfilled", &unfilled_boxes);

							ImGui::Dummy(ImVec2(0.0f, 3.0f));

							ImGui::Checkbox("Corners", &corner_esp);

							ImGui::Dummy(ImVec2(0.0f, 3.0f));

							ImGui::Checkbox("Skeletons", &bone_esp);

							ImGui::Dummy(ImVec2(0.0f, 3.0f));

							ImGui::Checkbox("Snaplines", &snaplines);

							ImGui::Dummy(ImVec2(0.0f, 3.0f));

							ImGui::Checkbox("Names", &name_esp);

							ImGui::Dummy(ImVec2(0.0f, 3.0f));

							ImGui::Checkbox("Healthbars", &healthbars);

							ImGui::Dummy(ImVec2(0.0f, 3.0f));

							ImGui::Checkbox("RGB Enemies", &rgb_enemy);

							ImGui::SameLine();

							ImGui::Dummy(ImVec2(280.f, 0.0f));

							ImGui::SameLine();

							ImGui::Text("RGB Friendlies");

							ImGui::SameLine();

							ImGui::Checkbox("##RGBFriendlies", &rgb_friendly);

							ImGui::Dummy(ImVec2(0.0f, 3.0f));

							ImGui::SetNextItemWidth(90);

							ImGui::InputFloat("Boldness", &boldness, .25);
							if (boldness > 3)
								boldness = 3;
							else if (boldness < .25)
								boldness = .25;

							ImGui::SameLine();

							ImGui::Dummy(ImVec2(268.f, 0.0f));

							ImGui::SameLine();

							ImGui::Text("Enemies");
							ImGui::SameLine();
							ImGui::ColorEdit3("##EspBoxColorEditor", (float*)&enemy_colors, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

							ImGui::Dummy(ImVec2(0.0f, 3.0f));

							ImGui::SetNextItemWidth(160);

							ImGui::Combo("##RGBSpeed", &speed_sel, speed_selection, IM_ARRAYSIZE(speed_selection));
							if (speed_sel == 0)
								rgb_speed = 0.25f;
							else if (speed_sel == 1)
								rgb_speed = 0.55f;
							else if (speed_sel == 2)
								rgb_speed = 0.95f;
							else if (speed_sel == 3)
								rgb_speed = 1.2f;

							ImGui::SameLine();

							ImGui::Dummy(ImVec2(248.f, 0.0f));

							ImGui::SameLine();

							ImGui::Text("Friendlies");
							ImGui::SameLine();
							ImGui::ColorEdit3("##FriendlyEspBoxColorEditor", (float*)&friendly_colors, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

							ImGui::EndTabItem();

						}

						ImGui::EndTabBar();
					}

					ImGui::Dummy(ImVec2(0.0f, 6.0f));

					if (ImGui::BeginTabBar("##Misc", ImGuiTabBarFlags_NoTooltip))
					{

						ImGui::Dummy(ImVec2(0.0f, 3.0f));

						if (ImGui::BeginTabItem("Misc"))
						{

							ImGui::Checkbox("Anti-Aim", &AntiAim);

							ImGui::SameLine();

							ImGui::Checkbox("Bullet Tracers", &BulletTracers);

							ImGui::Dummy(ImVec2(0.0f, 2.0f));

							ImGui::Separator();

							ImGui::Dummy(ImVec2(0.0f, 3.0f));

							ImGui::SetNextItemWidth(160);

							ImGui::Combo("Tracer Type", &tracer_type, tracerstypes, IM_ARRAYSIZE(tracerstypes));

							ImGui::SetNextItemWidth(160);

							ImGui::Combo("Style", &style_sel, style_selection, IM_ARRAYSIZE(style_selection));

							ImGui::Dummy(ImVec2(0.0f, 3.0f));

							ImGui::EndTabItem();
						}

						ImGui::EndTabBar();
					}

					ImGui::EndTabItem();
				}

				players_list::draw();

				friends::draw();

				if (ImGui::BeginTabItem("Tools"))
				{
					ImGui::Dummy(ImVec2(0.0f, 2.0f));

					if (ImGui::Button("Crash All"))
					{
						Cbuf_AddText(0, "callvote map  \"Crash All Clients\"""\ndisconnect");
					}

					ImGui::SameLine();

					if (ImGui::Button("SL"))
					{
						CL_AddReliableCommand(0, "sl 1488 88");
					}

					ImGui::Dummy(ImVec2(0.0f, 2.0f));

					ImGui::Separator();

					ImGui::Dummy(ImVec2(0.0f, 3.0f));

					static auto join_input = ""s;

					ImGui::Dummy(ImVec2(0.0f, 3.0f));

					ImGui::InputTextWithHint("##JOIN", "JOIN", &join_input);

					ImGui::SameLine();

					if (ImGui::Button("Join##XUID"))
					{
						if (strlen(join_input.data()))
							LobbyVM_JoinEvent(0, std::stoull(join_input), JOIN_TYPE_PARTY);
					}

					static auto cbuf_input = ""s;

					ImGui::Dummy(ImVec2(0.0f, 3.0f));

					ImGui::InputTextWithHint("##CBUF", "CBUF", &cbuf_input);

					ImGui::SameLine();

					if (ImGui::Button("Send##CBUF"))
					{
						if (strlen(cbuf_input.data()))
							Cbuf_AddText(0, cbuf_input.data());
					}

					static auto single_input = ""s;

					ImGui::Dummy(ImVec2(0.0f, 3.0f));

					ImGui::InputTextWithHint("##SINGLE", "SINGLE", &single_input);
					ImGui::SameLine();

					if (ImGui::Button("Send##SINGLE"))
					{
						if (strlen(single_input.data()))
							Cmd_ExecuteSingleCommand(0, 0, single_input.data(), false);
					}

					static auto reliable_input = ""s;

					ImGui::Dummy(ImVec2(0.0f, 3.0f));

					ImGui::InputTextWithHint("##RELIABLE", "RELIABLE", &reliable_input);

					ImGui::SameLine();

					if (ImGui::Button("Send##RELIABLE"))
					{
						if (strlen(reliable_input.data()))
							CL_AddReliableCommand(0, reliable_input.data());
					}

					ImGui::Dummy(ImVec2(0.0f, 3.0f));

					static auto dvar_input = ""s;

					ImGui::InputTextWithHint("##DVAR", "DVAR", &dvar_input);

					ImGui::SameLine();

					if (ImGui::Button("Send##DVAR"))
					{
						vector<string> sep = split(dvar_input, ' ');

						if (strlen(sep[1].c_str()))
							Dvar_SetFromString(sep[0].c_str(), sep[1].c_str(), true);
					}

					ImGui::Dummy(ImVec2(0.0f, 3.0f));

					ImGui::Separator();

					static auto popup_input = ""s;

					ImGui::Dummy(ImVec2(0.0f, 3.0f));

					ImGui::InputTextWithHint("##POP-UP", "POP-UP", &popup_input);
					ImGui::SameLine();

					if (ImGui::Button("Send##POP-UP"))
					{
						if (strlen(popup_input.data()))
							exploit::send_popup(std::stoull(popup_input));
					}

					ImGui::Dummy(ImVec2(0.0f, 3.0f));

					static auto XUID_DATA = ""s;

					ImGui::InputTextWithHint("##XUID_DATA", "CRASH", &XUID_DATA);

					ImGui::SameLine();

					if (ImGui::Button("Send"))
					{
						if (strlen(XUID_DATA.data()))
						{
							friends::send_info_request(std::stoull(XUID_DATA));
							if (friends::players_session_info[std::stoull(XUID_DATA)].hostAddress.inaddr)
							{
								const auto sess_info{ friends::players_session_info[std::stoull(XUID_DATA)] };
								const auto netadr{ get_net_adr(sess_info) };
								exploit::SendToAdr(std::stoull(XUID_DATA.data()), netadr);
							}
						}
					}

					ImGui::Dummy(ImVec2(0.0f, 3.0f));

					static auto XUID_NET = ""s;
					static auto MSG_NET = ""s;

					ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x / 4.5);

					ImGui::InputTextWithHint("##XUID_NET", "XUID", &XUID_NET);

					ImGui::SameLine();

					ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x / 1.78);

					ImGui::InputTextWithHint("##MSG_NET", "OOB/STRING", &MSG_NET);

					ImGui::SameLine();

					ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);

					if (ImGui::Button("Send"))
					{
						if (strlen(MSG_NET.data()) || (strlen(XUID_NET.data())))
						{
							friends::send_info_request(std::stoull(XUID_NET));
							if (friends::players_session_info[std::stoull(XUID_NET)].hostAddress.inaddr)
							{
								const auto sess_info{ friends::players_session_info[std::stoull(XUID_NET)] };
								const auto netadr{ get_net_adr(sess_info) };
								send_oob(netadr, MSG_NET);
							}
						}
					}

					ImGui::Dummy(ImVec2(0.0f, 3.0f));

					static auto XUID = ""s;

					ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x / 4.5);

					ImGui::InputTextWithHint("##XUID", "XUID", &XUID);

					ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x / 2.3317);

					ImGui::SameLine();

					ImGui::Combo("##KICK", &kick_sel, kicks, IM_ARRAYSIZE(kicks));

					ImGui::SameLine();

					if (ImGui::Button("Send##KICKS"))
					{
						if (strlen(XUID.data()))
							LobbyClientMsg_SendDisconnectClient(0, (LobbyType)0x0, std::stoull(XUID), kick[kick_sel]);
						LobbyClientMsg_SendDisconnectClient(0, (LobbyType)0X1, std::stoull(XUID), kick[kick_sel]);
					}

					ImGui::Dummy(ImVec2(0.0f, 3.0f));

					static auto IM = ""s;

					ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x / 4.5);

					ImGui::InputTextWithHint("##IM", "XUID", &IM);

					ImGui::SameLine();

					static auto IM1 = ""s;

					ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x / 6);

					ImGui::InputTextWithHint("##IM1", "ID", &IM1);

					ImGui::SameLine();

					static auto IM2 = ""s;

					ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x / 3.5);

					ImGui::InputTextWithHint("##IM2", "MESSAGE", &IM2);

					ImGui::SameLine();

					static auto IM3 = ""s;

					ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x / 4.469);

					ImGui::InputTextWithHint("##IM3", "SIZE", &IM3);

					ImGui::SameLine();

					if (ImGui::Button("Send##IM"))
					{
						if (strlen(IM.data()) || (strlen(IM1.data()) || (strlen(IM2.data()) || (strlen(IM3.data())))))
							exploit::send_IM(std::stoull(IM), std::stoull(IM1), std::stoull(IM2), std::stoull(IM3));
					}

					ImGui::Dummy(ImVec2(0.0f, 3.0f));

					ImGui::Separator();

					ImGui::Dummy(ImVec2(0.0f, 3.0f));

					if (ImGui::TreeNode("Class Editor"))
					{

						ImGui::Dummy(ImVec2(0.0f, 2.0f));

						static int ItemId = 0;
						static int ClassIndex = 0;

						if (ImGui::Button("Set Class"))
						{

							char Buffer[8];

							__int64 a = Class1();
							__int64 a1 = Class2();
							__int64 a3 = Class3(a1, a);
							Class4(Buffer, 0, a3);
							__int64 a4 = Class5(slots[slotItem]);
							SetClass(Buffer, (unsigned int)ClassIndex, a4, ItemId);
						}

						ImGui::SameLine();

						if (ImGui::Button("Thrust Pack"))
						{
							char Buffer[8];

							__int64 a = Class1();
							__int64 a1 = Class2();
							__int64 a3 = Class3(a1, a);
							Class4(Buffer, 0, a3);
							__int64 a4 = Class5("talent3");
							SetClass(Buffer, (unsigned int)ClassIndex, a4, 27);
						}

						ImGui::SameLine();

						if (ImGui::Button("Super Speed"))
						{
							char Buffer[8];

							__int64 a = Class1();
							__int64 a1 = Class2();
							__int64 a3 = Class3(a1, a);
							Class4(Buffer, 0, a3);
							__int64 a4 = Class5("talent1");
							__int64 a5 = Class5("talent2");
							__int64 a6 = Class5("talent3");
							__int64 a7 = Class5("talent4");
							__int64 a8 = Class5("talent5");
							__int64 a9 = Class5("talent6");
							SetClass(Buffer, (unsigned int)ClassIndex, a4, 11);
							SetClass(Buffer, (unsigned int)ClassIndex, a5, 38);
							SetClass(Buffer, (unsigned int)ClassIndex, a6, 38);
							SetClass(Buffer, (unsigned int)ClassIndex, a7, 38);
							SetClass(Buffer, (unsigned int)ClassIndex, a8, 38);
							SetClass(Buffer, (unsigned int)ClassIndex, a9, 38);
						}

						ImGui::SameLine();

						if (ImGui::Button("Clear Class"))
						{
							char Buffer[8];

							__int64 a = Class1();
							__int64 a1 = Class2();
							__int64 a3 = Class3(a1, a);
							Class4(Buffer, 0, a3);
							__int64 a4 = Class5("primary");
							__int64 a5 = Class5("primaryattachment1");
							__int64 a6 = Class5("primaryattachment2");
							__int64 a7 = Class5("primaryattachment3");
							__int64 a8 = Class5("primaryattachment4");
							__int64 a9 = Class5("primaryattachment5");
							__int64 a10 = Class5("primaryattachment6");
							__int64 a11 = Class5("primaryattachment7");
							__int64 a12 = Class5("secondary");
							__int64 a13 = Class5("secondaryattachment1");
							__int64 a14 = Class5("secondaryattachment2");
							__int64 a15 = Class5("secondaryattachment3");
							__int64 a16 = Class5("secondaryattachment4");
							__int64 a17 = Class5("secondaryattachment5");
							__int64 a18 = Class5("secondaryattachment6");
							__int64 a19 = Class5("secondaryattachment7");
							__int64 a20 = Class5("talent1");
							__int64 a21 = Class5("talent2");
							__int64 a22 = Class5("talent3");
							__int64 a23 = Class5("talent4");
							__int64 a24 = Class5("talent5");
							__int64 a25 = Class5("talent6");
							__int64 a26 = Class5("bonuscard1");
							__int64 a27 = Class5("bonuscard2");
							__int64 a28 = Class5("bonuscard3");
							__int64 a29 = Class5("primarycamo");
							__int64 a30 = Class5("secondarycamo");
							__int64 a31 = Class5("primaryreticle");
							__int64 a32 = Class5("primarypaintjobslot");
							__int64 a33 = Class5("primaryweaponmodelslot");
							__int64 a34 = Class5("primarycharm");
							__int64 a35 = Class5("primarydeathfx");
							__int64 a36 = Class5("secondaryreticle");
							__int64 a37 = Class5("secondarypaintjobslot");
							__int64 a38 = Class5("secondaryweaponmodelslot");
							__int64 a39 = Class5("secondarycharm");
							__int64 a40 = Class5("secondarydeathfx");
							__int64 a41 = Class5("primarygrenade");
							__int64 a42 = Class5("primarygrenadecount");
							__int64 a43 = Class5("specialgrenade");
							__int64 a44 = Class5("specialgrenadecount");
							__int64 a45 = Class5("herogadget");
							__int64 a46 = Class5("specialty1");
							__int64 a47 = Class5("specialty2");
							__int64 a48 = Class5("specialty3");
							__int64 a49 = Class5("specialty4");
							__int64 a50 = Class5("specialty5");
							__int64 a51 = Class5("specialty6");
							__int64 a52 = Class5("talisman1");
							__int64 a53 = Class5("tacticalgear");
							__int64 a54 = Class5("equippedBubbleGumPack");
							SetClass(Buffer, (unsigned int)ClassIndex, a4, 0);
							SetClass(Buffer, (unsigned int)ClassIndex, a5, 0);
							SetClass(Buffer, (unsigned int)ClassIndex, a6, 0);
							SetClass(Buffer, (unsigned int)ClassIndex, a7, 0);
							SetClass(Buffer, (unsigned int)ClassIndex, a8, 0);
							SetClass(Buffer, (unsigned int)ClassIndex, a9, 0);
							SetClass(Buffer, (unsigned int)ClassIndex, a10, 0);
							SetClass(Buffer, (unsigned int)ClassIndex, a11, 0);
							SetClass(Buffer, (unsigned int)ClassIndex, a12, 0);
							SetClass(Buffer, (unsigned int)ClassIndex, a13, 0);
							SetClass(Buffer, (unsigned int)ClassIndex, a14, 0);
							SetClass(Buffer, (unsigned int)ClassIndex, a15, 0);
							SetClass(Buffer, (unsigned int)ClassIndex, a16, 0);
							SetClass(Buffer, (unsigned int)ClassIndex, a17, 0);
							SetClass(Buffer, (unsigned int)ClassIndex, a18, 0);
							SetClass(Buffer, (unsigned int)ClassIndex, a19, 0);
							SetClass(Buffer, (unsigned int)ClassIndex, a20, 0);
							SetClass(Buffer, (unsigned int)ClassIndex, a21, 0);
							SetClass(Buffer, (unsigned int)ClassIndex, a22, 0);
							SetClass(Buffer, (unsigned int)ClassIndex, a23, 0);
							SetClass(Buffer, (unsigned int)ClassIndex, a24, 0);
							SetClass(Buffer, (unsigned int)ClassIndex, a25, 0);
							SetClass(Buffer, (unsigned int)ClassIndex, a26, 0);
							SetClass(Buffer, (unsigned int)ClassIndex, a27, 0);
							SetClass(Buffer, (unsigned int)ClassIndex, a28, 0);
							SetClass(Buffer, (unsigned int)ClassIndex, a29, 0);
							SetClass(Buffer, (unsigned int)ClassIndex, a30, 0);
							SetClass(Buffer, (unsigned int)ClassIndex, a31, 0);
							SetClass(Buffer, (unsigned int)ClassIndex, a32, 0);
							SetClass(Buffer, (unsigned int)ClassIndex, a33, 0);
							SetClass(Buffer, (unsigned int)ClassIndex, a34, 0);
							SetClass(Buffer, (unsigned int)ClassIndex, a35, 0);
							SetClass(Buffer, (unsigned int)ClassIndex, a36, 0);
							SetClass(Buffer, (unsigned int)ClassIndex, a37, 0);
							SetClass(Buffer, (unsigned int)ClassIndex, a38, 0);
							SetClass(Buffer, (unsigned int)ClassIndex, a39, 0);
							SetClass(Buffer, (unsigned int)ClassIndex, a40, 0);
							SetClass(Buffer, (unsigned int)ClassIndex, a41, 0);
							SetClass(Buffer, (unsigned int)ClassIndex, a42, 0);
							SetClass(Buffer, (unsigned int)ClassIndex, a43, 0);
							SetClass(Buffer, (unsigned int)ClassIndex, a44, 0);
							SetClass(Buffer, (unsigned int)ClassIndex, a45, 0);
							SetClass(Buffer, (unsigned int)ClassIndex, a46, 0);
							SetClass(Buffer, (unsigned int)ClassIndex, a47, 0);
							SetClass(Buffer, (unsigned int)ClassIndex, a48, 0);
							SetClass(Buffer, (unsigned int)ClassIndex, a49, 0);
							SetClass(Buffer, (unsigned int)ClassIndex, a50, 0);
							SetClass(Buffer, (unsigned int)ClassIndex, a51, 0);
							SetClass(Buffer, (unsigned int)ClassIndex, a52, 0);
							SetClass(Buffer, (unsigned int)ClassIndex, a53, 0);
							SetClass(Buffer, (unsigned int)ClassIndex, a54, 0);
						}

						ImGui::Dummy(ImVec2(0.0f, 3.0f));

						ImGui::InputInt("Item ID", &ItemId);

						ImGui::Dummy(ImVec2(0.0f, 3.0f));

						ImGui::Combo("Slot Name", &slotItem, slots, IM_ARRAYSIZE(slots));

						ImGui::Dummy(ImVec2(0.0f, 3.0f));

						ImGui::Combo("Class Index", &class_sel, customclass, IM_ARRAYSIZE(customclass));

						if (class_sel == 0)
							ClassIndex = 0;
						else if (class_sel == 1)
							ClassIndex = 1;
						else if (class_sel == 2)
							ClassIndex = 2;
						else if (class_sel == 3)
							ClassIndex = 3;
						else if (class_sel == 4)
							ClassIndex = 4;
						else if (class_sel == 5)
							ClassIndex = 5;
						else if (class_sel == 6)
							ClassIndex = 6;
						else if (class_sel == 7)
							ClassIndex = 7;
						else if (class_sel == 8)
							ClassIndex = 8;
						else if (class_sel == 9)
							ClassIndex = 9;
						else if (class_sel == 10)
							ClassIndex = 10;
						else if (class_sel == 11)
							ClassIndex = 11;

						ImGui::Dummy(ImVec2(0.0f, 3.0f));

						ImGui::TreePop();
					}

					ImGui::Dummy(ImVec2(0.0f, 3.0f));

					ImGui::Separator();

					ImGui::Dummy(ImVec2(0.0f, 3.0f));

					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Misc"))
				{

					ImGui::Checkbox("Third Person", &thirdPerson);

					ImGui::Dummy(ImVec2(0.0f, 2.0f));

					ImGui::Separator();

					ImGui::Dummy(ImVec2(0.0f, 3.0f));

					ImGui::InputTextWithHint("##NAME", "NAME", (char*)nameBuffer, IM_ARRAYSIZE(name_input));

					static auto clan_input = ""s;

					ImGui::Dummy(ImVec2(0.0f, 3.0f));

					ImGui::InputTextWithHint("##CLAN", "CLAN", &clan_input);

					ImGui::SameLine();

					if (ImGui::Button("Set##CLAN"))
					{
						LiveStats_SetClanTagText(0, clan_input.data());
					}

					ImGui::Dummy(ImVec2(0.0f, 3.0f));

					ImGui::Separator();

					ImGui::Dummy(ImVec2(0.0f, 3.0f));

					static auto print_bold = ""s;

					ImGui::InputTextWithHint("##iPrintBold", "PRINT BOLD", &print_bold);

					ImGui::SameLine();

					if (ImGui::Button("Print"))
					{
						CG_GameMessage(0, print_bold.data());
						CG_BoldGameMessageCenter(0, print_bold.data(), 5);
					}

					ImGui::Dummy(ImVec2(0.0f, 3.0f));

					ImGui::Separator();

					ImGui::Dummy(ImVec2(0.0f, 3.0f));

					ImGui::InputTextWithHint("##BattleTag", "USER#0000", lookupName, 254);

					ImGui::SameLine();

					if (ImGui::Button("Lookup")) {
						LiveGroups_SearchPlayerByGamertag(0, (__int64)lookupName);
					}


					if (strlen((char*)(dwProcessBase + 0x59D4458)) > 0) {
						ImGui::Text("BattleTag: %s", (char*)(dwProcessBase + 0x59D4458));
						ImGui::Text("XUID: %lld", *(__int64*)(dwProcessBase + 0x59D4450));
						if (ImGui::IsItemClicked())
						{
							char xuid[255];
							sprintf_s(xuid, "%lld", *(__int64*)(dwProcessBase + 0x59D4450));
							ImGui::SetClipboardText(xuid);
						}
					}

					ImGui::Dummy(ImVec2(0.0f, 3.0f));

					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Host")) {

					ImGui::Dummy(ImVec2(0.0f, 2.0f));

					if (ImGui::Checkbox("GodMode", &godmode))
					{
						pGod = !pGod;
						if (pGod) {

							*(int*)(*(__int64*)(dwProcessBase + 0x9353A00) + 0xE40) = 5;
							CG_BoldGameMessageCenter(0, "GodMode ^2Enabled", 3);
						}
						else {

							*(int*)(*(__int64*)(dwProcessBase + 0x9353A00) + 0xE40) = 4;
							CG_BoldGameMessageCenter(0, "GodMode ^1Disabled", 3);

						}
					}

					ImGui::SameLine();

					if (ImGui::Checkbox("Ammo", &unlimitedammo))
					{

						pAmmo = !pAmmo;
						if (pAmmo) {

							*(int*)(*(__int64*)(dwProcessBase + 0x9353A00) + 0x1324) = MAXINT32;
							*(int*)(*(__int64*)(dwProcessBase + 0x9353A00) + 0x133C) = MAXINT32;
							*(int*)(*(__int64*)(dwProcessBase + 0x9353A00) + 0x1338) = MAXINT32;
							*(int*)(*(__int64*)(dwProcessBase + 0x9353A00) + 0x1340) = MAXINT32;
							CG_BoldGameMessageCenter(0, "Unlimited Ammo ^2Enabled", 3);
						}
						else
						{
							*(int*)(*(__int64*)(dwProcessBase + 0x9353A00) + 0x1324) = 20;
							*(int*)(*(__int64*)(dwProcessBase + 0x9353A00) + 0x133C) = 20;
							*(int*)(*(__int64*)(dwProcessBase + 0x9353A00) + 0x1338) = 20;
							*(int*)(*(__int64*)(dwProcessBase + 0x9353A00) + 0x1340) = 20;
							CG_BoldGameMessageCenter(0, "Unlimited Ammo ^1Disabled", 3);
						}
					}

					ImGui::SameLine();

					if (ImGui::Checkbox("NoTarget", &NoTarget))
					{

						pNoTarget = !pNoTarget;
						if (pNoTarget)
						{
							*(int*)(*(__int64*)(dwProcessBase + 0x9353A08) + 0x348) = 0;
							CG_BoldGameMessageCenter(0, "No Target ^2Enabled", 3);
						}
						else {
							*(int*)(*(__int64*)(dwProcessBase + 0x9353A08) + 0x348) = 200;
							CG_BoldGameMessageCenter(0, "No Target ^1Disabled", 3);
						}

					}

					ImGui::Dummy(ImVec2(0.0f, 3.0f));

					ImGui::Separator();

					ImGui::Dummy(ImVec2(0.0f, 3.0f));

					if (ImGui::Button("Set"))
					{
						pPoints = !pPoints;
						if (pPoints)
						{

							*(float*)(dwProcessBase + 0x8B501E4) = timescale_slider;
							*(int*)(*(__int64*)(dwProcessBase + 0x9353A00) + 0x446C) = setpoints_input;
							std::string setpoints = std::to_string(setpoints_input);
							std::string timescale = std::to_string(timescale_slider);
							char timescaleandpoints[255];
							sprintf_s(timescaleandpoints, "^2%s ^7Points Received & Timescale Set To ^5%s", setpoints, timescale);
							CG_BoldGameMessageCenter(0, timescaleandpoints, 3);
						}
					}

					ImGui::SameLine();

					if (ImGui::Button("Headshots"))
					{
						pHeadshots = !pHeadshots;
						if (pHeadshots)
						{
							*(int*)(*(__int64*)(dwProcessBase + 0x9353A00) + 0x1030) = -1; //Exo and headshot
							CG_BoldGameMessageCenter(0, "Exo and Headshots ^2Enabled", 3);
						}
						else
						{
							*(int*)(*(__int64*)(dwProcessBase + 0x9353A00) + 0x1030) = 0; //Exo and headshot
							CG_BoldGameMessageCenter(0, "Exo and Headshots ^1Disabled", 3);
						}
					}

					ImGui::SameLine();

					if (ImGui::Button("Remove Barriers"))
					{
						pBarriers = !pBarriers;
						if (pBarriers)
						{
							*(int*)(*(__int64*)(dwProcessBase + 0x9353A00) + 0x102C) = -1;
							CG_BoldGameMessageCenter(0, "Barriers ^2Removed", 3);
						}
						else
						{
							*(int*)(*(__int64*)(dwProcessBase + 0x9353A00) + 0x102C) = 0;
							CG_BoldGameMessageCenter(0, "Barriers ^1Replaced", 3);
						}
					}

					ImGui::Dummy(ImVec2(0.0f, 2.0f));

					ImGui::SetNextItemWidth(255);

					ImGui::InputInt("Points", &setpoints_input, 5000);

					ImGui::Dummy(ImVec2(0.0f, 3.0f));

					ImGui::SetNextItemWidth(255);

					ImGui::SliderFloat("Timescale", &timescale_slider, 1, 10);

					ImGui::Dummy(ImVec2(0.0f, 3.0f));

					ImGui::Separator();

					ImGui::Dummy(ImVec2(0.0f, 3.0f));

					char botnum[20];

					ImGui::InputTextWithHint("###botnum", "Number of Bots", botnum, 20);

					ImGui::SameLine();

					if (ImGui::Button("Add"))
					{
						LobbyHostBots_AddBotsToLobby(LobbyType::LOBBY_TYPE_PRIVATE, atoll(botnum), true, (LOBBY_MODULE_CLIENT, LobbyType::LOBBY_TYPE_PRIVATE), 0);
						LobbyHostBots_AddBotsToLobby(LobbyType::LOBBY_TYPE_GAME, atoll(botnum), true, (LOBBY_MODULE_CLIENT, LobbyType::LOBBY_TYPE_GAME), 0);
					}

					ImGui::Dummy(ImVec2(0.0f, 3.0f));

					ImGui::Separator();

					ImGui::Dummy(ImVec2(0.0f, 3.0f));

					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Dev")) {

					ImGui::Dummy(ImVec2(0.0f, 1.49f));

					ImGui::Separator();

					ImGui::Dummy(ImVec2(0.0f, 1.49f));

					if (ImGui::TreeNode("Main"))
					{

						ImGui::Dummy(ImVec2(0.0f, 3.0f));

						char info[255];
						sprintf_s(info, "%p", cgArray);

						ImGui::Text("cgArray : 0x%p", cgArray);
						if (ImGui::IsItemClicked())
							ImGui::SetClipboardText(info);

						ImGui::Dummy(ImVec2(0, 3));

						sprintf_s(info, "%p", (__int64)(cgArray + 0x3E4C86));

						ImGui::Text("clientinfo_t : 0x%p", (__int64)(cgArray + 0x3E4C86));
						if (ImGui::IsItemClicked())
							ImGui::SetClipboardText(info);

						ImGui::Dummy(ImVec2(0, 3));

						clientactive* CL = GetClientActive();

						ImGui::Dummy(ImVec2(0, 3));

						int cmdNumber = CL_GetCurrentCmdNumber(0);

						ImGui::Text("Current Cmd Number : %d", cmdNumber);

						ImGui::TreePop();
					}

					ImGui::Dummy(ImVec2(0.0f, 1.49f));

					ImGui::Separator();

					ImGui::Dummy(ImVec2(0.0f, 1.49f));

					if (ImGui::TreeNode("Spoof"))
					{
						ImGui::Dummy(ImVec2(0.0f, 3.0f));

						ImGui::Checkbox("Spoof Name", &spoof_hostname);

						ImGui::SameLine();

						ImGui::Checkbox("Spoof Xuid", &spoof_hostxuid);

						ImGui::SameLine();

						ImGui::Checkbox("Spoof Netadr", &spoof_netadr);

						ImGui::Dummy(ImVec2(0.0f, 3.0f));

						ImGui::InputTextWithHint("##NAME", "SPOOFED NAME", &info_response_hostname);

						ImGui::Dummy(ImVec2(0.0f, 2.0f));

						ImGui::InputTextWithHint("##XUID", "SPOOFED XUID", &info_response_hostxuid);

						ImGui::TreePop();
					}

					ImGui::Dummy(ImVec2(0.0f, 1.49f));

					ImGui::Separator();

					ImGui::Dummy(ImVec2(0.0f, 1.49f));

					ImGui::EndTabItem();

				}

				ImGui::End();
			}
		}
	}
}