#include "zorus_options_impl.h"

#include "envoy/registry/registry.h"

#include "absl/strings/str_replace.h"
#include "absl/strings/str_split.h"
#include "absl/strings/string_view.h"

namespace Envoy {

ZorusOptionsImpl::ZorusOptionsImpl() {
  // TODO: Replace test configuration
  config_yaml_ = R"(
static_resources:
  listeners:
  - name: listener_0
    address:
      socket_address:
        protocol: TCP
        address: 0.0.0.0
        port_value: 8080
    filter_chains:
    - filters:
      - name: envoy.filters.network.http_connection_manager
        typed_config:
          "@type": type.googleapis.com/envoy.extensions.filters.network.http_connection_manager.v3.HttpConnectionManager
          stat_prefix: ingress_http
          access_log:
          - name: envoy.access_loggers.stdout
            typed_config:
              "@type": type.googleapis.com/envoy.extensions.access_loggers.stream.v3.StdoutAccessLog
          route_config:
            name: local_route
            virtual_hosts:
            - name: local_service
              domains: ["*"]
              routes:
              - match:
                  prefix: "/"
                route:
                  host_rewrite_literal: google.com
                  cluster: service_envoyproxy_io
          http_filters:
          - name: envoy.filters.http.router
  clusters:
  - name: service_envoyproxy_io
    type: LOGICAL_DNS
    # Comment out the following line to test on v6 networks
    dns_lookup_family: V4_ONLY
    lb_policy: ROUND_ROBIN
    load_assignment:
      cluster_name: service_envoyproxy_io
      endpoints:
      - lb_endpoints:
        - endpoint:
            address:
              socket_address:
                address: google.com
                port_value: 443
    transport_socket:
      name: envoy.transport_sockets.tls
      typed_config:
        "@type": type.googleapis.com/envoy.extensions.transport_sockets.tls.v3.UpstreamTlsContext
        sni: google.com
  )";
}

void ZorusOptionsImpl::disableExtensions(const std::vector<std::string>& names) {
  for (const auto& name : names) {
    const std::vector<absl::string_view> parts = absl::StrSplit(name, absl::MaxSplits('/', 1));

    if (parts.size() != 2) {
      ENVOY_LOG_MISC(warn, "failed to disable invalid extension name '{}'", name);
      continue;
    }

    if (Registry::FactoryCategoryRegistry::disableFactory(parts[0], parts[1])) {
      ENVOY_LOG_MISC(info, "disabled extension '{}'", name);
    } else {
      ENVOY_LOG_MISC(warn, "failed to disable unknown extension '{}'", name);
    }
  }
}

} // namespace Envoy
