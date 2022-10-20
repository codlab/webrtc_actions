#include "dvc/dvc_utils.h"

#include "rtc_base/logging.h"

namespace dolby_voice_client {

RtpAudio::RtpAudio(const void* data,
                   unsigned int len,
                   int64_t capture_timestamp_us) {
  if (data != nullptr && len > 0) {
    data_.insert(data_.begin(), static_cast<const char*>(data),
                 static_cast<const char*>(data) + len);
    if (capture_timestamp_us > 0)
      created_timestamp_us_ = capture_timestamp_us;
    else
      created_timestamp_us_ = rtc::SystemTimeNanos() / 1000;
  }
}

RtpAudio::~RtpAudio() {
}

ScopedTimeMeasurementMs::ScopedTimeMeasurementMs(std::string measurement_name)
    : measurement_name_(measurement_name) {
  start_time_ = rtc::SystemTimeMillis();
}

ScopedTimeMeasurementMs::~ScopedTimeMeasurementMs() {
  int64_t elapsed_time_ms = rtc::TimeSince(start_time_);
  RTC_DLOG(LS_INFO) << "ScopedTimeMeasurementMs: " << measurement_name_
              << ", value: " << elapsed_time_ms << " ms";
}

namespace {
constexpr bool kEventManualReset = true;
constexpr bool kEventInitiallySignaled = false;
}  // namespace

ScopedCompletionEvent::ScopedCompletionEvent()
    : event_(kEventManualReset, kEventInitiallySignaled) {
  static_assert(kEventManualReset == true,
                "The manual reset policy must be set to true");
  static_assert(kEventInitiallySignaled == false,
      "The initially signaled state must be set to false");
}

ScopedCompletionEvent::~ScopedCompletionEvent() {
  event_.Wait(rtc::Event::kForever);
}

void ScopedCompletionEvent::SignalCompleted() {
  event_.Set();
}

void ScopedCompletionEvent::SignalCompleted(
    webrtc_integration::DvcStatus status) {
  status_ = std::move(status);
  event_.Set();
}

const webrtc_integration::DvcStatus& ScopedCompletionEvent::Wait() {
  event_.Wait(rtc::Event::kForever);
  return status_;
}

}  // namespace dolby_voice_client
