// Copyright 2020 Dolby Laboratories and Dolby International AB
// Use of this source code is governed by the MIT license that can be
// found in the dolby/LICENSE file.

#include "dvc/audiopackethandler.h"
#include "dvc/dvc_constants.h"
#include "dvc/logging.h"
#include "dvc/rtp_utility.h"

namespace dolby_voice_client {
namespace webrtc_integration {

AudioPacketHandler::AudioPacketHandler(
    std::shared_ptr<DvcConferenceBase> conference)
    : _packets_to_send(kMaxPendingPackets, kMaxRtpPacketLen),
      conference_(std::move(conference)) {
  DVC_LOG;
  conference_->SetRtpSink(*this);
}

AudioPacketHandler::~AudioPacketHandler() {
  DVC_LOG;
  conference_->Stop();
}

void AudioPacketHandler::SendRtp(const char* data,
                                 size_t len,
                                 int64_t system_time_us) {
  HandleRtpPacketToSend(reinterpret_cast<const uint8_t*>(data), len,
                        system_time_us);
}

void AudioPacketHandler::HandleReceivedRtpPacket(const uint8_t* data,
                                                 size_t len) {
  conference_->HandleReceivedRtpPacket(reinterpret_cast<const char*>(data),
                                       static_cast<unsigned int>(len));
}

void AudioPacketHandler::SetMinimumPlayoutDelay(int delay_ms) {
  auto res = conference_->SetMinimumPlayoutDelay(delay_ms);
  if (!res) {
    RTC_LOG(LS_WARNING) << "Setting playout delay " << delay_ms
                        << "ms failed or rounded to limits: " << res.GetError();
  }
}

bool AudioPacketHandler::HandleReceivedRtcpPacket(const uint8_t*, size_t) {
  // Ignore RTCP
  return false;
}

bool AudioPacketHandler::HandleRtpPacketToSend(const uint8_t* data,
                                               size_t len,
                                               int64_t system_time_us) {
  webrtc::RtpUtility::RtpHeaderParser rtp_parser(data, len);
  webrtc::RTPHeader header;
  if (data != nullptr && !rtp_parser.Parse(&header, nullptr, false)) {
    RTC_LOG_F(LS_ERROR) << "Unable to parse RTP header!";
    return false;
  }
  const uint8_t* payload = data + header.headerLength;
  size_t payload_length = len - header.headerLength;

  int64_t receive_time_ms = system_time_us / 1000;

  size_t bytes_written = 0;
  if (!_packets_to_send.WriteBack(header, receive_time_ms, payload,
                                  payload_length, &bytes_written)) {
    RTC_LOG(LS_WARNING) << "Packet queue full, dropping RTP packet!";
  }
  return true;
}

bool AudioPacketHandler::HandleRtcpPacketToSend(const uint8_t* data,
                                                size_t len) {
  // Ignore RTCP
  return false;
}

bool AudioPacketHandler::GetPacketsToSend(
    std::vector<AudioBuffer>& audio_buffers) {
  return _packets_to_send.GetEnqueuedAudioBuffers(audio_buffers);
}

AudioBufferQueue& AudioPacketHandler::packets_to_send() {
  return _packets_to_send;
}

void AudioPacketHandler::SetMute(bool mute) {
  conference_->SetMute(mute, webrtc_integration::DvcDirection::INPUT);
}

}  // namespace webrtc_integration
}  // namespace dolby_voice_client
