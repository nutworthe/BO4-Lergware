#pragma once
#include "../includes.h"

netadr_t get_net_adr(const XSESSION_INFO& info);

namespace friends
{
	struct response_t
	{
		bool valid;
	};

	struct friends_t
	{
		std::uint64_t xuid;
		std::string name;
		response_t response;
		InfoResponseLobby Lobby[3];
		bool auto_kick = { false };
		bool auto_crash = { false };
		bool auto_popup = { false };
		bool sent = { false };

		std::chrono::time_point<std::chrono::high_resolution_clock> last_checked;
	};

	extern std::vector<friends_t> friends;
	extern std::unordered_map<std::uint64_t, XSESSION_INFO> players_session_info;

	void draw();
	void on_frame();
	void write_to_friends();
	void refresh_friends();
	void send_info_request(const std::uint64_t target_id);

	namespace instant_message
	{
		void handle_message(const std::uint64_t sender_id, const char* message, const std::uint32_t message_size);
	}
}