// Copyright 2020 Dolby Laboratories and Dolby International AB
// Use of this source code is governed by the MIT license that can be
// found in the dolby/LICENSE file.

#pragma once

#include <chrono>
#include <thread>

#include "rtc_base/logging.h"

#define DVC_LOG RTC_LOG(LS_INFO) << "###### ###### ###### DVC ###### ###### ###### [" << __func__ << "] "
#define DVC_LOG_AND_SLEEP DVC_LOG << "before"; std::this_thread::sleep_for(std::chrono::milliseconds{100}); DVC_LOG << "after"
