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

typedef std::unique_ptr<DnsFilterFactory> DnsFilterFactory_guard;

class mutex_guard
{
public:
  mutex_guard(absl::Mutex &mutex) : mutex_(mutex) {mutex_.Lock();}
  ~mutex_guard() {mutex_.Unlock();}

private:
  absl::Mutex &mutex_;
};

class DnsFilterFactory : Envoy::Logger::Loggable<Envoy::Logger::Id::dns>
{
public:
  DnsFilterFactory() {};

  void addFilter(DnsFilter_guard &filter);
  int filterCount();

  bool processRequest(const std::string& url, Envoy::Network::DnsLookupFamily family, Envoy::Network::DnsResolver::ResolveCb callback, OUT std::string &filterName);
  bool processRequestInternal(const std::string& url, Envoy::Network::DnsLookupFamily family, Envoy::Network::DnsResolver::ResolveCb callback, OUT std::string &filterName);

private:
  static DnsFilterFactory_guard dnsfilterfactory_;
  std::list<DnsFilter_guard> filters_;
  absl::Mutex mutex_;

private:
  DnsFilterFactory(const DnsFilterFactory&) = delete;  
  DnsFilterFactory& operator=(DnsFilterFactory& ) = delete;
};

} // namespace Network
} // namespace Envoy
