#include "nxnekotrace_client.h"

namespace NxNekoTrace {
    NxNekoTraceClient::NxNekoTraceClient(const std::string &address, uint16_t port) : io_context_() {
        endpoint_ = udp::endpoint(asio::ip::make_address(address), port);

        auto random_endpoint = udp::endpoint(endpoint_);
        random_endpoint.port(rand());
        socket_ = new asio::ip::udp::socket(io_context_, random_endpoint);

        std::vector<uint8_t> heyo_there;
        heyo_there.push_back(MSG_ID_HEYO_THERE);
        heyo_there.push_back((sequence_id & 0xFF000000) >> 24);
        heyo_there.push_back((sequence_id & 0x00FF0000) >> 16);
        heyo_there.push_back((sequence_id & 0x0000FF00) >> 8);
        heyo_there.push_back(sequence_id & 0x000000FF);
        socket_->send_to(asio::buffer(heyo_there), endpoint_);
    }

    void
    NxNekoTraceClient::send_net_log(uint8_t msg_id, uint16_t msg_len, const std::string &app_name, uint32_t address,
                                    uint16_t port, bool outbound, uint8_t app_flags, uint8_t flags, const std::vector<uint8_t> &data) {
        std::vector<uint8_t> net_log_data;

        net_log_data.push_back(MSG_ID_NET_LOG);
        net_log_data.push_back((sequence_id & 0xFF000000) >> 24);
        net_log_data.push_back((sequence_id & 0x00FF0000) >> 16);
        net_log_data.push_back((sequence_id & 0x0000FF00) >> 8);
        net_log_data.push_back(sequence_id & 0x000000FF);

        net_log_data.push_back(msg_id);

        net_log_data.push_back((msg_len >> 8) & 0xFF);
        net_log_data.push_back(msg_len & 0xFF);

        uint32_t appNameSize = app_name.size();
        net_log_data.push_back((appNameSize & 0xFF000000) >> 24);
        net_log_data.push_back((appNameSize & 0x00FF0000) >> 16);
        net_log_data.push_back((appNameSize & 0x0000FF00) >> 8);
        net_log_data.push_back(appNameSize & 0x000000FF);

        for (size_t i = 0; i < appNameSize; i++) {
            net_log_data.push_back(app_name[i]);
        }

        net_log_data.push_back((appNameSize & 0xFF000000) >> 24);
        net_log_data.push_back((appNameSize & 0x00FF0000) >> 16);
        net_log_data.push_back((appNameSize & 0x0000FF00) >> 8);
        net_log_data.push_back(appNameSize & 0x000000FF);

        net_log_data.push_back((port >> 8) & 0xFF);
        net_log_data.push_back(port & 0xFF);

        net_log_data.push_back(outbound);

        net_log_data.push_back(app_flags);

        net_log_data.push_back(flags);

        net_log_data.insert(net_log_data.end(), data.begin(), data.end());

        socket_->send_to(asio::buffer(net_log_data), endpoint_);

        sequence_id++;
    }

    void NxNekoTraceClient::send_alias(uint8_t msg_id, std::string name) {
        std::vector<uint8_t> alias_data;

        alias_data.push_back(MSG_ID_NAME_ALIAS);
        alias_data.push_back((sequence_id & 0xFF000000) >> 24);
        alias_data.push_back((sequence_id & 0x00FF0000) >> 16);
        alias_data.push_back((sequence_id & 0x0000FF00) >> 8);
        alias_data.push_back(sequence_id & 0x000000FF);

        alias_data.push_back(msg_id);

        uint32_t aliasSize = name.size();
        alias_data.push_back((aliasSize & 0xFF000000) >> 24);
        alias_data.push_back((aliasSize & 0x00FF0000) >> 16);
        alias_data.push_back((aliasSize & 0x0000FF00) >> 8);
        alias_data.push_back(aliasSize & 0x000000FF);

        for (size_t i = 0; i < aliasSize; i++) {
            alias_data.push_back(name[i]);
        }

        socket_->send_to(asio::buffer(alias_data), endpoint_);

        sequence_id++;
    }
}