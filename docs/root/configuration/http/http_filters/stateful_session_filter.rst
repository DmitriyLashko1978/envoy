.. _config_http_filters_stateful_session:

Stateful session
================

Stateful session is an HTTP filter which overrides the upstream host based on extensible session state
and updates the session state based on the final selected upstream host. The override host will
eventually overwrites the load balancing result. This filter implements session stickiness without using
a hash-based load balancer.

And by extending the session state, this filter also allows more flexible control over the results of
the load balancing.

.. note::

  Stateful sessions can result in imbalanced load across upstreams and allow external actors to direct
  requests to specific upstream hosts. Operators should carefully consider the security and reliability
  implications of stateful sessions before enabling this feature.

Overview
--------

Session stickiness allows requests belonging to the same session to be consistently routed to a specific
upstream host.

HTTP session stickiness in Envoy is generally achieved through hash-based load balancing.
The stickiness of hash-based sessions can be regarded as 'weak' since the upstream host may change when the
host set changes. This filter implements 'strong' stickiness. It is intended to handle the following cases:

* The case where more stable session stickiness is required. For example, when a host is marked as degraded
  but it is desirable to continue routing requests for existing sessions to that host.
* The case where a non hash-based load balancer (Random, Round Robin, etc.) is used and session stickiness
  is still required. If stateful sessions are enabled in this case, requests for new sessions will be routed
  to the corresponding upstream host based on the result of load balancing. Requests belonging to existing
  sessions will be routed to the session's upstream host.

Configuration
-------------

* :ref:`v3 API reference <envoy_v3_api_msg_extensions.filters.http.stateful_session.v3.StatefulSession>`
* This filter should be configured with the name *envoy.filters.http.stateful_session*.

How it works
------------

The most important configuration for this filter is an :ref:`extensible session state
<envoy_v3_api_field_extensions.filters.http.stateful_session.v3.StatefulSession.session_state>`.

While processing the request, the stateful session filter will search for the corresponding session and
host based on the request. The results of the search will be used to influence the final load balancing
results.

If no existing session is found, the filter will create a session to store the selected upstream host.
Please note that the session here is an abstract concept. The details of the storage are based on the
session state implementation.

One example
___________

Currently, only :ref:`cookie-based session state
<envoy_v3_api_msg_extensions.http.stateful_session.cookie.v3.CookieBasedSessionState>` is supported.
So let's take this as an example.

.. code-block:: yaml

  name: envoy.filters.http.stateful_session
  typed_config:
    "@type": type.googleapis.com/envoy.extensions.filters.http.stateful_session.v3.StatefulSession
    session_state:
      name: envoy.http.stateful_session.cookie
      typed_config:
        "@type": type.googleapis.com/envoy.extensions.http.stateful_session.cookie.v3.CookieBasedSessionState
        cookie:
          name: global-session-cookie
          path: /path
          ttl: 120s


In the above configuration, the cookie-based session state obtains the overridden host of the current session
from the cookie named `global-session-cookie` and if the corresponding host exists in the upstream cluster, the
request will be routed to that host.

If there is no valid cookie, the load balancer will choose a new upstream host. When responding, the address
of the selected upstream host will be stored in the cookie named `global-session-cookie`.
