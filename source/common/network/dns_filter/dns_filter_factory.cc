#include "dns_filter_factory.h"

namespace Envoy {
namespace Network {

DnsFilterFactory *DnsFilterFactory::dnsfilterfactory_;

void DnsFilterFactory::addFilter(DnsFilter_guard &filter)
{
  absl::WriterMutexLock lock(&mutex_);
  filters_.push_back(std::move(filter));
}

int DnsFilterFactory::filterCount()
{
  absl::ReaderMutexLock lock(&mutex_);
  return filters_.size();
}

bool DnsFilterFactory::processRequest(const std::string& url, Envoy::Network::DnsLookupFamily family,
  Envoy::Network::DnsResolver::ResolveCb callback, OUT std::string &filterName)
{
  ENVOY_LOG(debug, "Factory starts searhing for {} dns request.", url);
  filterName = "";
  absl::ReaderMutexLock lock(&mutex_);
  return processRequestInternal(url, family, callback, filterName);;
}

bool DnsFilterFactory::processRequestInternal(const std::string& url, Envoy::Network::DnsLookupFamily family,
  Envoy::Network::DnsResolver::ResolveCb callback, OUT std::string &filterName)
{
  for (auto it = filters_.begin(); it != filters_.end(); ++it) {
    if(it->get()->processRequest(url, family, callback)) {
      filterName = it->get()->getName();
      ENVOY_LOG(debug, "Filter {} has handled {} dns request.", filterName, url);
      return true;
    }
  }

  ENVOY_LOG(debug, "Any filter of the factory didn't handle {} dns request.", url);
  return false;
}

} // namespace Network
} // namespace Envoy
