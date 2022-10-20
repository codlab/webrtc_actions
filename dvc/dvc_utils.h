#ifndef DOLBY_VOICE_CLIENT_DVC_UTILS_H_
#define DOLBY_VOICE_CLIENT_DVC_UTILS_H_

#include "build/build_config.h"
#include "rtc_base/time_utils.h"
#include "rtc_base/event.h"
#include "dvc/dvc_conference_factory_env.h"

namespace dolby_voice_client {

class RtpAudio {
 public:
  explicit RtpAudio(const void* data,
                    unsigned int len,
                    int64_t capture_timestamp_us = -1);
  virtual ~RtpAudio();

  const char* Data() { return data_.data(); }
  size_t Len() { return data_.size(); }
  int64_t TimeStampUs() { return created_timestamp_us_; }

 protected:
  std::vector<char> data_;
  int64_t created_timestamp_us_;
};

class ScopedTimeMeasurementMs {
 public:
  ScopedTimeMeasurementMs(std::string measurement_name);
  virtual ~ScopedTimeMeasurementMs();

 protected:
  std::string measurement_name_;
  int64_t start_time_;
};

class ScopedCompletionEvent final {
 public:
  ScopedCompletionEvent();
  ~ScopedCompletionEvent();

  void SignalCompleted();
  void SignalCompleted(webrtc_integration::DvcStatus status);

  const webrtc_integration::DvcStatus& Wait();

 private:
  webrtc_integration::DvcStatus status_ {};
  rtc::Event event_;
};

}  // namespace dolby_voice_client

#endif  // DOLBY_VOICE_CLIENT_DVC_UTILS_H_
