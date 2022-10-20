// Copyright 2020 Dolby Laboratories and Dolby International AB
// Use of this source code is governed by the MIT license that can be
// found in the dolby/LICENSE file.

/*
 * This class is inspired by the BufferQueue from WebRTC rtc_base component
 * and tuned for our needs.
 */

#ifndef WEBRTC_DVC_AUDIO_BUFFER_QUEUE_H_
#define WEBRTC_DVC_AUDIO_BUFFER_QUEUE_H_

#include "api/rtp_headers.h"
#include "rtc_base/buffer.h"
#include "rtc_base/time_utils.h"

#include <deque>
#include <mutex>
#include <vector>

namespace dolby_voice_client {
namespace webrtc_integration {

class AudioBuffer {
 public:
  AudioBuffer(size_t size);
  AudioBuffer(size_t size, size_t capacity);
  AudioBuffer(const AudioBuffer& rhs);
  virtual ~AudioBuffer() = default;

  // Sets a wall-time clock timestamp in milliseconds to be used for profiling
  // of time between two points in the audio chain.
  // Example:
  //   t0: UpdateProfileTimeStamp()
  //   t1: ElapsedProfileTimeMs() => t1 - t0 [msec]
  void UpdateProfileTimeStamp();
  // Returns the time difference between now and when UpdateProfileTimeStamp()
  // was last called. Returns -1 if UpdateProfileTimeStamp() has not yet been
  // called.
  int64_t ElapsedProfileTimeMs() const;

  rtc::Buffer payload;
  webrtc::RTPHeader rtp_header_;
  int64_t receive_time_ms_ = 0;
  int64_t profile_timestamp_ms_ = 0;
};

inline void AudioBuffer::UpdateProfileTimeStamp() {
  profile_timestamp_ms_ = rtc::TimeMillis();
}

inline int64_t AudioBuffer::ElapsedProfileTimeMs() const {
  if (profile_timestamp_ms_ == 0) {
    // Profiling has not been activated.
    return -1;
  }
  return rtc::TimeSince(profile_timestamp_ms_);
}

class AudioBufferQueue {
 public:
  // Creates an audio buffer queue with a given capacity and default buffer
  // size.
  AudioBufferQueue(size_t capacity, size_t default_size);
  virtual ~AudioBufferQueue();

  size_t size() const;
  void Clear();

  bool GetEnqueuedAudioBuffers(std::vector<AudioBuffer>& buffers);
  bool WriteBack(const webrtc::RTPHeader& rtp_header,
                 int64_t receive_time_ms,
                 const void* data,
                 size_t bytes,
                 size_t* bytes_written);

 private:
  size_t capacity_;
  size_t default_size_;
  mutable std::mutex mutex_;
  std::deque<AudioBuffer*> queue_;
  std::vector<AudioBuffer*> free_list_;

  AudioBufferQueue(const AudioBufferQueue&) = delete;
  AudioBufferQueue& operator=(const AudioBufferQueue&) = delete;
};

}  // namespace webrtc_integration
}  // namespace dolby_voice_client

#endif  // WEBRTC_DVC_AUDIO_BUFFER_QUEUE_H_
