#ifndef KOJO_CC2_ASBR_PLAYERCOLORPARAMJSON
#define KOJO_CC2_ASBR_PLAYERCOLORPARAMJSON

#include <cc2/asbr/player_color_param.hpp>

using namespace cc2;

template<>
class json_serializer<asbr::player_color_param> {
public:
    static asbr::player_color_param read(const nlohmann::ordered_json& json) {
        asbr::player_color_param result;

        for (const auto& [key, value] : json.items()) {
            if (key == "Version" || key == "Filetype") continue;

            // if (!value.is_string()) error::print(
            //     nucc::status_code::json_value,
            //     std::format("JSON data for entry \"{}\" is not a valid hex code.", key),
            //     "Ensure all hex codes are strings with the format \"#RRGGBB\"."
            // );
            // if (value.template get<std::string>().length() != 7) error::print(
            //     nucc::status_code::json_value,
            //     std::format("JSON data for entry \"{}\" is not a valid hex code.", key),
            //     "Ensure all hex codes are strings with the format \"#RRGGBB\". Alpha channel is not supported."
            // );

            asbr::player_color_param::entry entry_buffer;
            entry_buffer.character_id = key.substr(0, 4) + "0" + key.at(5);
            entry_buffer.costume_index = key.at(4) - '0';
            entry_buffer.color.from_hex_code(value);
            entry_buffer.tint_index = key.at(9);

            result.entries[key] = entry_buffer;
        }

        return result;
    }
    
    static nlohmann::ordered_json write(const asbr::player_color_param& param) {
        nlohmann::ordered_json result;

        result["Version"] = 250718;
        result["Filetype"] = "PlayerColorParam";

        for (auto& [key, entry] : param.entries) {
            result[key] = entry.color.to_hex_code();
        }

        return result;
    }
};

#endif // KOJO_CC2_ASBR_PLAYERCOLORPARAMJSON