#ifndef KOJO_CC2_ASBR_BINARY_PLAYERCOLORPARAM
#define KOJO_CC2_ASBR_BINARY_PLAYERCOLORPARAM

#include <cc2/binary_serializer.hpp>
#include <cc2/asbr/player_color_param.hpp>

#include <kojo/logger.hpp>

#include <unordered_map>

namespace cc2 {

template<>
class binary_serializer<asbr::player_color_param> {
public:
    static asbr::player_color_param read(const std::byte* src, const size_t start = 0) {
        asbr::player_color_param result;
        kojo::logger log{"CC2 Binary Data"};
        
        if (src == nullptr) {
                log.fatal(
                kojo::logger::status::null_pointer,
                "Attempted to load data, but received null memory.",
                "Ensure the data provided is valid and not null."
            );
            return result;
        }
        kojo::binary_view data{src, start};

        auto VERSION = data.read<std::uint32_t>(std::endian::little);
        auto entry_count = data.read<std::uint32_t>(std::endian::little);
        auto first_pointer = data.read<std::uint64_t>(std::endian::little);
        data.change_pos(first_pointer - 8);

        std::unordered_map<std::string, size_t> tint_tracker;
        for (int i = 0; i < entry_count; i++) {
            auto character_id_pointer   = data.read<std::uint64_t>(std::endian::little);
            auto character_id           = data.read<std::string_view>(character_id_pointer - 8);
            auto costume_index          = data.read<std::uint32_t>(std::endian::little);
            nucc::rgb color;
            color.red                   = data.read<std::uint32_t>(std::endian::little);
            color.green                 = data.read<std::uint32_t>(std::endian::little);
            color.blue                  = data.read<std::uint32_t>(std::endian::little);

            std::string tint_id = std::format("{}-{}", character_id, costume_index);
            size_t tint_index = tint_tracker[tint_id]++;

            std::string key = std::format("{}{}{}col{}", character_id.substr(0, 4), costume_index, character_id.at(5), tint_index);
            result.entries[key] = color;
        }

        return result;
    }
    
    static kojo::binary write(const asbr::player_color_param& param) {
        kojo::binary output_data;

        const size_t HEADER_SIZE = 16;
        const size_t ENTRY_SIZE = 24;
        const size_t CHARACTER_ID_LENGTH = 8;

        const std::uint32_t entry_count = param.entries.size();
        const std::uint64_t first_pointer = 8;
        output_data.write<std::uint32_t>(asbr::player_color_param::version(), std::endian::little);
        output_data.write<std::uint32_t>(entry_count, std::endian::little);
        output_data.write<std::uint64_t>(first_pointer, std::endian::little);

        size_t i = 0;
        for (auto& [key, entry] : param.entries) {
            const std::uint32_t costume_index = std::stoi(key.substr(4, 1));

            auto character_id_pointer = ENTRY_SIZE * entry_count + i * CHARACTER_ID_LENGTH;
            output_data.write<std::uint64_t>(character_id_pointer, std::endian::little);
            output_data.write<std::uint32_t>(costume_index, std::endian::little);
            output_data.write<std::uint32_t>(entry.red, std::endian::little);
            output_data.write<std::uint32_t>(entry.green, std::endian::little);
            output_data.write<std::uint32_t>(entry.blue, std::endian::little);
        }
        for (auto& [key, entry] : param.entries) {
            const std::string character_id = key.substr(0, 4) + "0" + key.at(5);
            output_data.write<std::string_view>(character_id, CHARACTER_ID_LENGTH);
        }

        return output_data;
    }
};

}

#endif // KOJO_CC2_ASBR_BINARY_PLAYERCOLORPARAM