load(
    "@envoy//bazel:envoy_build_system.bzl",
    "envoy_cc_library",
    "envoy_package",
)

envoy_package()

envoy_cc_library(
    name = "dns_filter_lib",
    srcs = ["dns_filter_factory.cc"],
    hdrs = ["dns_filter.h", "dns_filter_factory.h"],
    visibility = ["//visibility:public"],
    deps = [
        "@envoy//source/common/common:utility_lib",
    ],
    repository = "@envoy",
)
