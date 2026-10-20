#pragma once

#include <any>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

// Stores shared ownership of named game values. A widget receives its own shared_ptr
// when the runtime loader resolves a key.
class ValueRegistry
{
public:
	template <typename T>
	void bind(std::string key, std::shared_ptr<T> value)
	{
		if (!value)
			throw std::invalid_argument("Cannot bind a null value: " + key);

		values.insert_or_assign(std::move(key), std::move(value));
	}

	template <typename T>
	[[nodiscard]] std::shared_ptr<T> get(std::string_view key) const
	{
		const auto found = values.find(std::string(key));
		if (found == values.end())
			throw std::out_of_range("No value is registered with key: " + std::string(key));

		const auto* value = std::any_cast<std::shared_ptr<T>>(&found->second);
		if (!value)
			throw std::bad_any_cast();
		return *value;
	}

	[[nodiscard]] bool contains(std::string_view key) const
	{
		return values.contains(std::string(key));
	}

	void erase(std::string_view key)
	{
		values.erase(std::string(key));
	}

private:
	std::unordered_map<std::string, std::any> values;
};
