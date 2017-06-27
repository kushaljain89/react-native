// Copyright 2004-present Facebook. All Rights Reserved.

#pragma once

#include <memory>
#include <unordered_set>
#include <vector>

#include <cxxreact/JSExecutor.h>
#include <folly/Optional.h>
#include <folly/dynamic.h>

#ifndef RN_EXPORT
#define RN_EXPORT __attribute__((visibility("default")))
#endif

namespace facebook {
namespace react {

class NativeModule;

struct ModuleConfig {
  size_t index;
  folly::dynamic config;
};

class RN_EXPORT ModuleRegistry {
 public:
  // not implemented:
  // onBatchComplete: see https://our.intern.facebook.com/intern/tasks/?t=5279396
  // getModule: only used by views
  // getAllModules: only used for cleanup; use RAII instead
  // notifyCatalystInstanceInitialized: this is really only used by view-related code
  // notifyCatalystInstanceDestroy: use RAII instead

  ModuleRegistry(std::vector<std::unique_ptr<NativeModule>> modules);
  void registerModules(std::vector<std::unique_ptr<NativeModule>> modules);

  std::vector<std::string> moduleNames();

  folly::Optional<ModuleConfig> getConfig(const std::string& name);

  void callNativeMethod(unsigned int moduleId, unsigned int methodId, folly::dynamic&& params, int callId);
  MethodCallResult callSerializableNativeHook(unsigned int moduleId, unsigned int methodId, folly::dynamic&& args);

 private:
  // This is always populated
  std::vector<std::unique_ptr<NativeModule>> modules_;

  // This is used to extend the population of modulesByName_ if registerModules is called after moduleNames
  void updateModuleNamesFromIndex(size_t size);

  // This is only populated if moduleNames() is called.  Values are indices into modules_.
  std::unordered_map<std::string, size_t> modulesByName_;

  // This is populated with modules that are requested via getConfig but are unknown.
  // An error will be thrown if they are subsquently added to the registry.
  std::unordered_set<std::string> unknownModules_;
};

}
}