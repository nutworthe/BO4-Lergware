#pragma once
#include "../includes.h"

#define FRIENDS_LIST "friends.json"
#define min12(a,b)            (((a) < (b)) ? (a) : (b))

namespace utils
{
	template <typename T>
	static auto atoi(const std::string& str)
	{
		return static_cast<T>(std::atoi(str.data()));
	}
	
	static auto atoi(const std::string& str)
	{
		return atoi<std::uint32_t>(str);
	}

	static auto atoll(const std::string& str)
	{
		return static_cast<std::uint64_t>(std::atoll(str.data()));
	}

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
	
	std::string va(const char* fmt, ...);
	std::string adr_to_string(const netadr_t* address);
}