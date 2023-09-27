#ifndef NKTRACETESTSERVER_NXNEKOTRACE_CLIENT_H
#define NKTRACETESTSERVER_NXNEKOTRACE_CLIENT_H


#include <cstdint>
#include <string>
#include <vector>
#include <asio.hpp>

using udp = asio::ip::udp;

namespace NxNekoTrace {

    class NxNekoTraceClient {
    public:
        NxNekoTraceClient(const std::string &address, uint16_t port);

        void
        send_net_log(uint8_t msg_id, uint16_t msg_len, const std::string &app_name, uint32_t address, uint16_t port,
                     bool outbound, uint8_t app_flags, uint8_t flags, const std::vector<uint8_t> &data);

        void send_alias(uint8_t msg_id, std::string name);

    private:
        udp::endpoint endpoint_;
        asio::io_context io_context_;
        udp::socket *socket_;
        std::atomic<size_t> sequence_id = 0;

        struct MsgBase {
            uint8_t id;
            uint32_t sequence_id;
        };

        struct MsgNetLog : MsgBase {
            uint8_t msg_id;
            uint16_t msg_len;
            std::string app_name;
            uint32_t address;
            uint16_t port;
            bool outbound;
            uint8_t app_flags;
            uint8_t packet_flags;
            std::vector<uint8_t> data;
        };

        struct MsgMeow : MsgBase {
            uint64_t time;
        };

        struct MsgNameAlias : MsgBase {
            uint8_t msg_id;
            std::string name;
        };

        enum : uint8_t {
            MSG_ID_NET_LOG = 0x00,
            MSG_ID_SAY_WHAT,
            MSG_ID_MEOW,
            MSG_ID_HEYO_THERE,
            MSG_ID_NAME_ALIAS
        };
    };
}

#endif //NKTRACETESTSERVER_NXNEKOTRACE_CLIENT_H
