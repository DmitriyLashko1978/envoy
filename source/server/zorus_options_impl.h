#pragma once

#include "envoy/common/exception.h"
#include "envoy/server/options.h"

#include "source/common/common/logger.h"

namespace Envoy {
class ZorusOptionsImpl : public Server::Options, protected Logger::Loggable<Logger::Id::config> {
public:
  using HotRestartVersionCb = std::function<std::string(bool)>;

  ZorusOptionsImpl(int argc, const char* const* argv, const HotRestartVersionCb& hot_restart_version_cb,
    spdlog::level::level_enum default_log_level) : ZorusOptionsImpl() {}

  ZorusOptionsImpl(std::vector<std::string> args, const HotRestartVersionCb& hot_restart_version_cb,
              spdlog::level::level_enum default_log_level) : ZorusOptionsImpl() {}

  uint64_t baseId() const override { return base_id_; }
  bool useDynamicBaseId() const override { return use_dynamic_base_id_; }
  const std::string& baseIdPath() const override { return base_id_path_; }
  uint32_t concurrency() const override { return concurrency_; }
  const std::string& configPath() const override { return config_path_; }
  const envoy::config::bootstrap::v3::Bootstrap& configProto() const override { return config_proto_; }
  const std::string& configYaml() const override { return config_yaml_; }
  bool allowUnknownStaticFields() const override { return allow_unknown_static_fields_; }
  bool rejectUnknownDynamicFields() const override { return reject_unknown_dynamic_fields_; }
  bool ignoreUnknownDynamicFields() const override { return ignore_unknown_dynamic_fields_; }
  const std::string& adminAddressPath() const override { return admin_address_path_; }
  Network::Address::IpVersion localAddressIpVersion() const override { return local_address_ip_version_;}

  std::chrono::seconds drainTime() const override { return drain_time_; }
  std::chrono::seconds parentShutdownTime() const override { return parent_shutdown_time_; }
  Server::DrainStrategy drainStrategy() const override { return drain_strategy_; }

  spdlog::level::level_enum logLevel() const override { return log_level_; }
  const std::vector<std::pair<std::string, spdlog::level::level_enum>>&
  componentLogLevels() const override { return component_log_levels_; }
  const std::string& logFormat() const override { return log_format_; }
  bool logFormatEscaped() const override { return log_format_escaped_; }
  bool enableFineGrainLogging() const override { return enable_fine_grain_logging_; }
  const std::string& logPath() const override { return log_path_; }
  uint64_t restartEpoch() const override { return restart_epoch_; }
  Server::Mode mode() const override { return mode_; }
  std::chrono::milliseconds fileFlushIntervalMsec() const override { return file_flush_interval_msec_; }
  const std::string& serviceClusterName() const override { return service_cluster_; }
  const std::string& serviceNodeName() const override { return service_node_; }
  const std::string& serviceZone() const override { return service_zone_; }
  bool hotRestartDisabled() const override { return hot_restart_disabled_; }
  bool signalHandlingEnabled() const override { return signal_handling_enabled_; }
  bool mutexTracingEnabled() const override { return mutex_tracing_enabled_; }
  bool coreDumpEnabled() const override { return core_dump_enabled_; }
  const Stats::TagVector& statsTags() const override { return stats_tags_; }
  Server::CommandLineOptionsPtr toCommandLineOptions() const override {
    Server::CommandLineOptionsPtr command_line_options = std::make_unique<envoy::admin::v3::CommandLineOptions>();
    return command_line_options;
  }
  bool cpusetThreadsEnabled() const override { return cpuset_threads_; }
  const std::vector<std::string>& disabledExtensions() const override {
    return disabled_extensions_;
  }

  const std::string& socketPath() const override { return socket_path_; }
  mode_t socketMode() const override { return socket_mode_; }

  static void disableExtensions(const std::vector<std::string>&);

private:
  ZorusOptionsImpl();

private:
  uint64_t base_id_{0};
  bool use_dynamic_base_id_{false};
  std::string base_id_path_;
  uint32_t concurrency_{1};
  std::string config_path_;
  envoy::config::bootstrap::v3::Bootstrap config_proto_;
  std::string config_yaml_;
  bool allow_unknown_static_fields_{false};
  bool reject_unknown_dynamic_fields_{false};
  bool ignore_unknown_dynamic_fields_{false};
  std::string admin_address_path_;
  Network::Address::IpVersion local_address_ip_version_{Network::Address::IpVersion::v4};

  std::chrono::seconds drain_time_{600};
  std::chrono::seconds parent_shutdown_time_{900};
  Server::DrainStrategy drain_strategy_{Server::DrainStrategy::Gradual};

  spdlog::level::level_enum log_level_{spdlog::level::info};
  std::vector<std::pair<std::string, spdlog::level::level_enum>> component_log_levels_;
  std::string log_format_{Logger::Logger::DEFAULT_LOG_FORMAT};
  bool log_format_escaped_{false};
  bool enable_fine_grain_logging_{false};
  std::string log_path_;
  uint64_t restart_epoch_{0};
  Server::Mode mode_{Server::Mode::Serve};
  std::chrono::milliseconds file_flush_interval_msec_{10000};
  std::string service_cluster_;
  std::string service_node_;
  std::string service_zone_;

  bool hot_restart_disabled_{false};
  bool signal_handling_enabled_{true};
  bool mutex_tracing_enabled_{false};
  bool core_dump_enabled_{false};
  Stats::TagVector stats_tags_;
  bool cpuset_threads_{false};
  std::vector<std::string> disabled_extensions_;

  std::string socket_path_{"@dreadnought_domain_socket"};
  mode_t socket_mode_{0};
};

class NoServingException : public EnvoyException {
public:
  NoServingException() : EnvoyException("NoServingException") {}
};

class MalformedArgvException : public EnvoyException {
public:
  MalformedArgvException(const std::string& what) : EnvoyException(what) {}
};

} // namespace Envoy
