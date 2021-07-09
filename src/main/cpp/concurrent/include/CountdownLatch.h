#pragma once

#include "absl/base/thread_annotations.h"
#include "absl/strings/string_view.h"
#include "absl/synchronization/mutex.h"
#include <string>

namespace rocketmq {
class CountdownLatch {
public:
  explicit CountdownLatch(int32_t count) : CountdownLatch(count, "anonymous") {}
  CountdownLatch(int32_t count, absl::string_view name) : count_(count), name_(name.data(), name.length()) {}

  void await() LOCKS_EXCLUDED(mtx_);

  void countdown() LOCKS_EXCLUDED(mtx_);

  void increaseCount() LOCKS_EXCLUDED(mtx_);

private:
  int32_t count_ GUARDED_BY(mtx_);

  absl::Mutex mtx_; // protects count_
  absl::CondVar cv_;

  std::string name_;
};
} // namespace rocketmq