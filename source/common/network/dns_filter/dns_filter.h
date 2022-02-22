#pragma once

#include <string>

#include "source/common/common/logger.h"
#include "envoy/network/dns.h"

namespace Envoy {
namespace Network {

class DnsFilter
{
public:
  virtual const std::string &getName() = 0;
  virtual bool processRequest(const std::string& url, Envoy::Network::DnsLookupFamily family,
    Envoy::Network::DnsResolver::ResolveCb callback) = 0;

  virtual ~DnsFilter() {};
};

typedef std::unique_ptr<DnsFilter> DnsFilter_guard;

} // namespace Network
} // namespace Envoy
