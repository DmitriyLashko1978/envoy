#pragma once

#include "source/server/admin/config_tracker_impl.h"
#include "source/common/network/utility.h"
#include "source/common/network/listen_socket_impl.h"

namespace Envoy {
namespace Server {

class AdminImplDummy : public Admin {
public:
  AdminImplDummy(const std::string& profile_path, Server::Instance& server,
            bool ignore_global_conn_limit) : server_(server), socket_(nullptr) {}

  const Network::Socket& socket() override {
    if(!socket_) {
      socket_ = std::make_unique<Network::TcpListenSocket>(Network::Utility::getCanonicalIpv4LoopbackAddress(), nullptr, true);
    }
    return *socket_;
  }

  bool addHandler(const std::string& prefix, const std::string& help_text, HandlerCb callback,
    bool removable, bool mutates_server_state) override { return false; }
  bool removeHandler(const std::string& prefix) override { return false; }
  ConfigTracker& getConfigTracker() override {
    if(!config_tracker_) {
      config_tracker_ = std::make_unique<ConfigTrackerImpl>();
    }
      return *config_tracker_;
    }

  void startHttpListener(const std::list<AccessLog::InstanceSharedPtr>& access_logs,
    const std::string& address_out_path,
    Network::Address::InstanceConstSharedPtr address,
    const Network::Socket::OptionsSharedPtr& socket_options,
    Stats::ScopePtr&& listener_scope) override {}
  uint32_t concurrency() const override { return server_.options().concurrency(); }

  Http::Code request(absl::string_view path_and_query, absl::string_view method,
    Http::ResponseHeaderMap& response_headers, std::string& body) override { return Http::Code::OK; }
  void addListenerToHandler(Network::ConnectionHandler* handler) override {}
  void closeSocket() {
    if (socket_) {
      socket_->close();
    }
  }

private:
  Server::Instance& server_;

  Network::SocketSharedPtr socket_;
  std::unique_ptr<ConfigTrackerImpl> config_tracker_;
};

} // namespace Server
} // namespace Envoy
