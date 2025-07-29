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

        result["Version"] = 250103;
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
};

}

#endif // KOJO_CC2_ASBR_JSON_MESSAGEINFO