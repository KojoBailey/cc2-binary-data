#ifndef KOJO_CC2_ASBR_JSON_PLAYERCOLORPARAM
#define KOJO_CC2_ASBR_JSON_PLAYERCOLORPARAM

#include <cc2/json_serializer.hpp>
#include <cc2/asbr/player_color_param.hpp>

#include <kojo/logger.hpp>

namespace cc2 {

template<>
class json_serializer<asbr::player_color_param> {
public:
    static asbr::player_color_param read(const nlohmann::ordered_json& json) {
        asbr::player_color_param result;
        kojo::logger log{"PlayerColorParam JSON Reader"};

        for (const auto& [key, value] : json.items()) {
            if (key == "Version" || key == "Filetype") continue;

            if (!value.is_string()) log.error(
                kojo::logger::status::bad_value,
                std::format("JSON data for entry \"{}\" is not a valid hex code.", key),
                "Ensure all hex codes are strings with the format \"#RRGGBB\"."
            );
            if (value.template get<std::string>().length() != 7) log.error(
                kojo::logger::status::bad_value,
                std::format("JSON data for entry \"{}\" is not a valid hex code.", key),
                "Ensure all hex codes are strings with the format \"#RRGGBB\". Alpha channel is not supported."
            );

            nucc::rgb color;
            result.entries[key] = color.from_hex_code(value);
        }

        return result;
    }
    
    static nlohmann::ordered_json write(const asbr::player_color_param& param) {
        nlohmann::ordered_json result;

        result["Version"] = 250729;
        result["Filetype"] = "PlayerColorParam";

        for (auto& [key, entry] : param.entries) {
            result[key] = entry.to_hex_code();
        }

        return result;
    }
};

}

#endif // KOJO_CC2_ASBR_JSON_PLAYERCOLORPARAM