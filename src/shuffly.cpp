/*
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <cstdlib>
#include <expected>
#include <filesystem>
#include <iostream>
#include <random>
#include <string_view>
#include <utility>
#include <vector>

using path = std::filesystem::path;
using rd_iterator = std::filesystem::recursive_directory_iterator;

namespace
{
std::expected<std::pair<path, path>, std::string_view> parse_arguments(int argc, char** argv)
{
	if (argc != 3U)
	{
		return std::unexpected{"Usage is: shuffly [input_path] [output_path]"};
	}

	const path input_path{argv[1U]};
	if (!std::filesystem::is_directory(input_path) || std::filesystem::is_empty(input_path))
	{
		return std::unexpected{"Input path must be a non-empty directory"};
	}

	const path output_path{argv[2U]};
	if (!std::filesystem::is_directory(output_path) || !std::filesystem::is_empty(output_path))
	{
		return std::unexpected{"Output path must be an empty directory"};
	}

	return std::pair{input_path, output_path};
}

bool has_mp3_extension(const path& path)
{
	if (!path.has_extension())
	{
		return false;
	}

	static_assert(std::is_same_v<path::string_type::value_type, char>);
	return path.native().ends_with(".mp3");
}

std::vector<path> mp3_input_file_path(const path& input_path)
{
	const std::filesystem::directory_entry dir{input_path};

	std::vector<path> result{};
	for (rd_iterator itr{dir}; itr != rd_iterator{}; ++itr)
	{
		const path& current_path = itr->path();
		if (has_mp3_extension(current_path))
		{
			result.emplace_back(current_path);
		}
	}

	return result;
}

template <typename Rng>
std::string random_prefix(Rng& rng)
{
	constexpr std::string_view allowed_characters{"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"};
	std::string prefix("****_");
	prefix[0U] = allowed_characters[rng() % allowed_characters.size()];
	prefix[1U] = allowed_characters[rng() % allowed_characters.size()];
	prefix[2U] = allowed_characters[rng() % allowed_characters.size()];
	prefix[3U] = allowed_characters[rng() % allowed_characters.size()];
	return prefix;
}

void shuffle_to_output(const std::vector<path>& mp3_file_paths, const path& output_path)
{
	std::mt19937 rng{std::random_device{}()};
	for (const auto& input_mp3_file_path : mp3_file_paths)
	{
		const auto output_mp3_file_path = output_path / (random_prefix(rng) + input_mp3_file_path.filename().native());
		std::cout << input_mp3_file_path.native() << " -> " << output_mp3_file_path.native() << "...\n";
		std::filesystem::copy(input_mp3_file_path, output_mp3_file_path);
	}
}
} // namespace

int main(int argc, char** argv)
{
	const auto arguments = parse_arguments(argc, argv);
	if (!arguments.has_value())
	{
		std::cerr << arguments.error() << '\n';
		return EXIT_FAILURE;
	}

	const auto& [input_path, output_path] = arguments.value();
	const auto input_file_paths = mp3_input_file_path(input_path);
	std::cout << "Starting copy of " << input_file_paths.size() << " files.\n";
	shuffle_to_output(input_file_paths, output_path);

	return EXIT_SUCCESS;
}