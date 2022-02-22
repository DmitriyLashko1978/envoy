#pragma once

#include <list>

#include "source/common/common/logger.h"
#include "source/common/singleton/threadsafe_singleton.h"
#include "envoy/network/dns.h"
#include "dns_filter.h"

namespace Envoy {
namespace Network {

class DnsFilterFactory;

using DnsFilterFactorySingleton = Envoy::ThreadSafeSingleton<DnsFilterFactory>;

class DnsFilterFactory : Envoy::Logger::Loggable<Envoy::Logger::Id::dns>
{
public:
  DnsFilterFactory() {};
  DnsFilterFactory(const DnsFilterFactory&) = delete;  
  DnsFilterFactory& operator=(DnsFilterFactory& ) = delete;

  void addFilter(DnsFilter_guard &filter);
  int filterCount();

  bool processRequest(const std::string& url, Envoy::Network::DnsLookupFamily family, Envoy::Network::DnsResolver::ResolveCb callback, OUT std::string &filterName);
  bool processRequestInternal(const std::string& url, Envoy::Network::DnsLookupFamily family, Envoy::Network::DnsResolver::ResolveCb callback, OUT std::string &filterName);

private:
  static DnsFilterFactory *dnsfilterfactory_;
  std::list<DnsFilter_guard> filters_;
  absl::Mutex mutex_;
};

} // namespace Network
} // namespace Envoy
