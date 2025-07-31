#ifndef KOJO_CC2_ASBR_JSON_MESSAGEINFO
#define KOJO_CC2_ASBR_JSON_MESSAGEINFO

#include <cc2/json_serializer.hpp>
#include <cc2/asbr/message_info.hpp>

#include <kojo/logger.hpp>

#include <filesystem>
#include <format>

namespace cc2 {

template<>
class json_serializer<asbr::message_info> {
public:
    static asbr::message_info read(const nlohmann::ordered_json& json) {
        asbr::message_info result;
        kojo::logger log{"CC2 Binary Data"};

        if (json.is_null()) {
            log.error(
                kojo::logger::status::null_data,
                "Provided JSON data is null/invalid.",
                "Ensure the JSON is formatted correctly and without errors."
            );
            return result;
        }

        result.language = json["Language"];
        load_adx2_file_list();

        std::unordered_map<std::string, std::string> colors;
        for (auto& [key, value] : json["Colors"].items()) {
            colors[key] = value.template get<std::string>().substr(1, 8);
        }

        for (const auto& [key, value] : json.items()) {
            if (key == "Language" || key == "Version" || key == "Filetype" || key == "Colors") continue;

            asbr::message_info::entry entry_buffer;

            entry_buffer.crc32_id.load(key);
            if (key != entry_buffer.crc32_id.string()) {
                entry_buffer.id = key;
            }

            if (value.contains("Reference")) {
                std::string reference_str = value["Reference"];
                entry_buffer.ref_crc32_id = nucc::crc32::hash(reference_str);
                entry_buffer.has_ref_crc32_id = true;
                entry_buffer.message = std::format("<text {} text_2 />", reference_str);
                entry_buffer.has_message = true;
            } else if (value.contains("Message")) {
                entry_buffer.message = value["Message"];
                entry_buffer.has_message = true;
                for (auto& [key, value] : colors) {
                    entry_buffer.message = std::regex_replace(entry_buffer.message,
                        std::regex(std::format("<{}>", key)),
                        std::format("<color 0x{}>", value)
                    );
                    entry_buffer.message = std::regex_replace(entry_buffer.message,
                        std::regex(std::format("</{}>", key)),
                        "</color>"
                    );
                }
            } else {
                log.error(
                    kojo::logger::status::missing_field,
                    std::format("JSON data for entry \"{}\" does not contain required field \"Message\".", key),
                    "Add the \"Message\" field, or the \"Reference\" field if a reference."
                );
            }

            if (value.contains("ADX2_File")) {
                entry_buffer.file_index = convert_file_str(value["ADX2_File"]);
                entry_buffer.has_file_index = true;
            }

            if (value.contains("ADX2_Cue_Index")) {
                entry_buffer.cue_index = value["ADX2_Cue_Index"];
                entry_buffer.has_cue_index = true;
            }

            result.entries[entry_buffer.key()] = entry_buffer;
        }

        return result;
    }
    
    static nlohmann::ordered_json write(const asbr::message_info& param) {
        nlohmann::ordered_json result;

        result["Version"] = 250730;
        result["Filetype"] = "messageInfo";
        result["Language"] = param.language;
        
        for (std::uint32_t key : param.sorted_keys) {
            auto& entry = param.entries.at(key);

            std::string id;
            if (entry.id != "") {
                id = entry.id;
            } else {
                id = entry.crc32_id.string();
            }
            nlohmann::ordered_json& json_entry = result[id];

            if (entry.ref_crc32_id.id() != 0) {
                std::string ref_id;
                if (entry.ref_id != "") {
                    ref_id = entry.ref_id;
                } else {
                    ref_id = entry.ref_crc32_id.string();
                }
                json_entry["Reference"] = ref_id;
            } else {
                json_entry["Message"] = entry.message;
            }

            if (entry.file_index != -1)
                json_entry["ADX2_File"] = convert_file_index(entry.file_index);

            if (entry.cue_index != -1)
                json_entry["ADX2_Cue_Index"] = entry.cue_index;
        }

        return result;
    }

private:
    inline static std::unordered_map<std::string_view, int> adx2_file_list;

    static void load_adx2_file_list() {
        if (!adx2_file_list.empty()) return;

        adx2_file_list["v_sys_etc"]     = 1;
        adx2_file_list["v_mob"]         = 2;
        adx2_file_list["v_btl_0bao01"]  = 3;
        adx2_file_list["v_btl_1dio01"]  = 4;
        adx2_file_list["v_btl_1jnt01"]  = 5;
        adx2_file_list["v_btl_1sdw01"]  = 6;
        adx2_file_list["v_btl_1zpl01"]  = 7;
        adx2_file_list["v_btl_2csr01"]  = 8;
        adx2_file_list["v_btl_2esd01"]  = 9;
        adx2_file_list["v_btl_2jsp01"]  = 10;
        adx2_file_list["v_btl_2krs01"]  = 11;
        adx2_file_list["v_btl_2lsa01"]  = 12;
        adx2_file_list["v_btl_2wmu01"]  = 13;
        adx2_file_list["v_btl_3abd01"]  = 14;
        adx2_file_list["v_btl_3dio01"]  = 15;
        adx2_file_list["v_btl_3hhs01"]  = 16;
        adx2_file_list["v_btl_3igy01"]  = 17;
        adx2_file_list["v_btl_3jsp01"]  = 18;
        adx2_file_list["v_btl_3jtr01"]  = 19;
        adx2_file_list["v_btl_3kki01"]  = 20;
        adx2_file_list["v_btl_3mra01"]  = 21;
        adx2_file_list["v_btl_3pln01"]  = 22;
        adx2_file_list["v_btl_3psp01"]  = 23;
        adx2_file_list["v_btl_3vni01"]  = 24;
        adx2_file_list["v_btl_4jsk01"]  = 25;
        adx2_file_list["v_btl_4jtr01"]  = 26;
        adx2_file_list["v_btl_4kir01"]  = 27;
        adx2_file_list["v_btl_4koi01"]  = 28;
        adx2_file_list["v_btl_4kwk01"]  = 29;
        adx2_file_list["v_btl_4oky01"]  = 30;
        adx2_file_list["v_btl_4oti01"]  = 31;
        adx2_file_list["v_btl_4rhn01"]  = 32;
        adx2_file_list["v_btl_4sgc01"]  = 33;
        adx2_file_list["v_btl_4ykk01"]  = 34;
        adx2_file_list["v_btl_5bct01"]  = 35;
        adx2_file_list["v_btl_5dvl01"]  = 36;
        adx2_file_list["v_btl_5fgo01"]  = 37;
        adx2_file_list["v_btl_5gac01"]  = 38;
        adx2_file_list["v_btl_5grn01"]  = 39;
        adx2_file_list["v_btl_5mst01"]  = 40;
        adx2_file_list["v_btl_5nrc01"]  = 41;
        adx2_file_list["v_btl_5prs01"]  = 42;
        adx2_file_list["v_btl_5trs01"]  = 43;
        adx2_file_list["v_btl_6ans01"]  = 44;
        adx2_file_list["v_btl_6elm01"]  = 45;
        adx2_file_list["v_btl_6fit01"]  = 46;
        adx2_file_list["v_btl_6jln01"]  = 47;
        adx2_file_list["v_btl_6pci02"]  = 48;
        adx2_file_list["v_btl_7dio01"]  = 49;
        adx2_file_list["v_btl_7jir01"]  = 50;
        adx2_file_list["v_btl_7jny01"]  = 51;
        adx2_file_list["v_btl_7vtn01"]  = 52;
        adx2_file_list["v_btl_8jsk01"]  = 53;
        adx2_file_list["v_card"]        = 56;
        adx2_file_list["v_gallery"]     = 57;
        adx2_file_list["v_btl_6wet01"]  = 58;
        adx2_file_list["v_sys_6wet01"]  = 59;
        adx2_file_list["v_btl_5ris01"]  = 60;
        adx2_file_list["v_sys_5ris01"]  = 61;
        adx2_file_list["v_btl_6pci01"]  = 62;
        adx2_file_list["v_sys_6pci01"]  = 63;
        adx2_file_list["v_btl_2shm01"]  = 64;
        adx2_file_list["v_sys_2shm01"]  = 65;
        adx2_file_list["v_btl_4kch01"]  = 66;
        adx2_file_list["v_sys_4kch01"]  = 67;
        adx2_file_list["v_btl_7dio02"]  = 68;
        adx2_file_list["v_sys_7dio02"]  = 69;
        adx2_file_list["v_btl_5abc01"]  = 70;
        adx2_file_list["v_sys_5abc01"]  = 71;
        adx2_file_list["v_btl_4fgm01"]  = 72;
        adx2_file_list["v_sys_4fgm01"]  = 73;
        adx2_file_list["v_btl_8wou01"]  = 74;
        adx2_file_list["v_sys_8wou01"]  = 75;
    }

    static int convert_file_str(std::string file) {
        if (!adx2_file_list.contains(file)) return -1;
        return adx2_file_list[file];
    }
    
    static constexpr std::string convert_file_index(int index) {
        switch(index) {
            case 1:     return "v_sys_etc";
            case 2:     return "v_mob";
            case 3:     return "v_btl_0bao01";
            case 4:     return "v_btl_1dio01";
            case 5:     return "v_btl_1jnt01";
            case 6:     return "v_btl_1sdw01";
            case 7:     return "v_btl_1zpl01";
            case 8:     return "v_btl_2csr01";
            case 9:     return "v_btl_2esd01";
            case 10:    return "v_btl_2jsp01";
            case 11:    return "v_btl_2krs01";
            case 12:    return "v_btl_2lsa01";
            case 13:    return "v_btl_2wmu01";
            case 14:    return "v_btl_3abd01";
            case 15:    return "v_btl_3dio01";
            case 16:    return "v_btl_3hhs01";
            case 17:    return "v_btl_3igy01";
            case 18:    return "v_btl_3jsp01";
            case 19:    return "v_btl_3jtr01";
            case 20:    return "v_btl_3kki01";
            case 21:    return "v_btl_3mra01";
            case 22:    return "v_btl_3pln01";
            case 23:    return "v_btl_3psp01";
            case 24:    return "v_btl_3vni01";
            case 25:    return "v_btl_4jsk01";
            case 26:    return "v_btl_4jtr01";
            case 27:    return "v_btl_4kir01";
            case 28:    return "v_btl_4koi01";
            case 29:    return "v_btl_4kwk01";
            case 30:    return "v_btl_4oky01";
            case 31:    return "v_btl_4oti01";
            case 32:    return "v_btl_4rhn01";
            case 33:    return "v_btl_4sgc01";
            case 34:    return "v_btl_4ykk01";
            case 35:    return "v_btl_5bct01";
            case 36:    return "v_btl_5dvl01";
            case 37:    return "v_btl_5fgo01";
            case 38:    return "v_btl_5gac01";
            case 39:    return "v_btl_5grn01";
            case 40:    return "v_btl_5mst01";
            case 41:    return "v_btl_5nrc01";
            case 42:    return "v_btl_5prs01";
            case 43:    return "v_btl_5trs01";
            case 44:    return "v_btl_6ans01";
            case 45:    return "v_btl_6elm01";
            case 46:    return "v_btl_6fit01";
            case 47:    return "v_btl_6jln01";
            case 48:    return "v_btl_6pci02";
            case 49:    return "v_btl_7dio01";
            case 50:    return "v_btl_7jir01";
            case 51:    return "v_btl_7jny01";
            case 52:    return "v_btl_7vtn01";
            case 53:    return "v_btl_8jsk01";
            case 56:    return "v_card";
            case 57:    return "v_gallery";
            case 58:    return "v_btl_6wet01";
            case 59:    return "v_sys_6wet01";
            case 60:    return "v_btl_5ris01";
            case 61:    return "v_sys_5ris01";
            case 62:    return "v_btl_6pci01";
            case 63:    return "v_sys_6pci01";
            case 64:    return "v_btl_2shm01";
            case 65:    return "v_sys_2shm01";
            case 66:    return "v_btl_4kch01";
            case 67:    return "v_sys_4kch01";
            case 68:    return "v_btl_7dio02";
            case 69:    return "v_sys_7dio02";
            case 70:    return "v_btl_5abc01";
            case 71:    return "v_sys_5abc01";
            case 72:    return "v_btl_4fgm01";
            case 73:    return "v_sys_4fgm01";
            case 74:    return "v_btl_8wou01";
            case 75:    return "v_sys_8wou01";
        }
        return std::to_string(index);
    }
};

}

#endif // KOJO_CC2_ASBR_JSON_MESSAGEINFO