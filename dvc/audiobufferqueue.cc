// Copyright 2020 Dolby Laboratories and Dolby International AB
// Use of this source code is governed by the MIT license that can be
// found in the dolby/LICENSE file.

#include "dvc/audiobufferqueue.h"

#include <algorithm>

#include "dvc/logging.h"

namespace dolby_voice_client {
namespace webrtc_integration {

AudioBuffer::AudioBuffer(size_t size) : AudioBuffer(size, size) {}

AudioBuffer::AudioBuffer(size_t size, size_t capacity)
    : payload(size, size), receive_time_ms_(0), profile_timestamp_ms_(0) {}

AudioBuffer::AudioBuffer(const AudioBuffer& rhs) : payload(rhs.payload.size()) {
  this->payload.SetData(rhs.payload);
  this->rtp_header_ = rhs.rtp_header_;
  this->receive_time_ms_ = rhs.receive_time_ms_;
  this->profile_timestamp_ms_ = rhs.profile_timestamp_ms_;
}

AudioBufferQueue::AudioBufferQueue(size_t capacity, size_t default_size)
    : capacity_(capacity), default_size_(default_size) {
  DVC_LOG;
}

AudioBufferQueue::~AudioBufferQueue() {
  DVC_LOG;
  std::lock_guard<std::mutex> lock(mutex_);

  for (AudioBuffer* buffer : queue_) {
    delete buffer;
  }
  for (AudioBuffer* buffer : free_list_) {
    delete buffer;
  }
}

size_t AudioBufferQueue::size() const {
  std::lock_guard<std::mutex> lock(mutex_);
  return queue_.size();
}

void AudioBufferQueue::Clear() {
  std::lock_guard<std::mutex> lock(mutex_);
  while (!queue_.empty()) {
    free_list_.push_back(queue_.front());
    queue_.pop_front();
  }
}

bool AudioBufferQueue::GetEnqueuedAudioBuffers(
    std::vector<AudioBuffer>& buffers) {
  size_t queue_size = 0;
  {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_size = queue_.size();
  }

  if (queue_size == 0) {
    return false;
  }

  while (queue_size > 0) {
    std::lock_guard<std::mutex> lock(mutex_);
    AudioBuffer* packet = queue_.front();
    queue_.pop_front();
    AudioBuffer audio_buffer(*packet);
    buffers.push_back(std::move(audio_buffer));
    free_list_.push_back(packet);
    queue_size--;
  }
  return true;
}

bool AudioBufferQueue::WriteBack(const webrtc::RTPHeader& rtp_header,
                                 int64_t receive_time_ms,
                                 const void* buffer,
                                 size_t bytes,
                                 size_t* bytes_written) {
  std::lock_guard<std::mutex> lock(mutex_);
  if (queue_.size() == capacity_) {
    return false;
  }

  AudioBuffer* packet;
  if (!free_list_.empty()) {
    packet = free_list_.back();
    free_list_.pop_back();
  } else {
    packet = new AudioBuffer(bytes, default_size_);
  }

  packet->rtp_header_ = rtp_header;
  packet->receive_time_ms_ = receive_time_ms;
  packet->payload.SetData(static_cast<const uint8_t*>(buffer), bytes);
  packet->UpdateProfileTimeStamp();
  if (bytes_written) {
    *bytes_written = bytes;
  }
  queue_.push_back(packet);
  return true;
}

}  // namespace webrtc_integration
}  // namespace dolby_voice_client
