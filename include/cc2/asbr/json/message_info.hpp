#ifndef KOJO_CC2_ASBR_JSON_MESSAGEINFO
#define KOJO_CC2_ASBR_JSON_MESSAGEINFO

#include <cc2/json_serializer.hpp>
#include <cc2/asbr/message_info.hpp>

#include <kojo/logger.hpp>

namespace cc2 {

template<>
class json_serializer<asbr::message_info> {
public:
    static asbr::message_info read(const nlohmann::ordered_json& json) {
        asbr::message_info result;
        kojo::logger log{"CC2 Binary Data"};

        

        return result;
    }
    
    static nlohmann::ordered_json write(const asbr::message_info& param) {
        nlohmann::ordered_json result;

        std::unordered_map<std::string, std::string> hashlist;
        kojo::binary hashlist_data{hashlist_path};
        while (!hashlist_data.at_end()) {
            std::string buffer = hashlist_data.read_str();
            hashlist[buffer] = hashlist_data.read_str();
        }
        nlohmann::ordered_json json;

        json["Version"] = 250103;
        json["Filetype"] = "messageInfo";
        json["Language"] = language;
        
        for (auto& [key, value] : entry_order) {
            auto& entry = entries[value];

            std::string hash = std::format("{:08x}", value);
            if (hashlist.contains(hash)) {
                hash = hashlist[hash];
            }
            nlohmann::ordered_json& json_entry = json[hash];

            if (entry.is_ref == 1) {
                hash = std::format("{:08x}", entry.ref_crc32_id);
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

        return json;

        return result;
    }
};

}

#endif // KOJO_CC2_ASBR_JSON_MESSAGEINFO