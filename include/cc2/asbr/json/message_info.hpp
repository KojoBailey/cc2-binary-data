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

        

        return result;
    }
    
    static nlohmann::ordered_json write(const asbr::message_info& param, std::filesystem::path hashlist_path = "") {
        nlohmann::ordered_json result;

        kojo::binary hashlist_store{hashlist_path};
        kojo::binary_view hashlist_data{hashlist_store};
        std::unordered_map<std::string, std::string> hashlist;
        while (hashlist_data.get_pos() < hashlist_store.size()) {
            std::string buffer = hashlist_data.read<std::string_view>().data();
            hashlist[buffer] = hashlist_data.read<std::string_view>();
        }

        result["Version"] = 250730;
        result["Filetype"] = "messageInfo";
        result["Language"] = param.language;
        
        for (auto& [key, value] : param.entry_order) {
            auto& entry = param.entries.at(value);

            std::string hash = std::format("{:08x}", value);
            if (hashlist.contains(hash)) {
                hash = hashlist[hash];
            }
            nlohmann::ordered_json& json_entry = result[hash];

            if (entry.is_ref == 1) {
                hash = std::format("{:08x}", entry.ref_crc32_id.id());
                if (hashlist.contains(hash)) {
                    hash = hashlist[hash];
                }
                json_entry["Reference"] = hash;
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