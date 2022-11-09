// Copyright 2020 Dolby Laboratories and Dolby International AB
// Use of this source code is governed by the MIT license that can be
// found in the dolby/LICENSE file.

#include "dvc/api/audio_codecs/donothingaudioencoderadapter.h"

namespace dolby_voice_client {
namespace webrtc_integration {

DoNothingEncoderAdapter::DoNothingEncoderAdapter(
    std::unique_ptr<AudioEncoder> encoder,
    CodecType codec_type,
    int payload_type)
    : _encoder(std::move(encoder)),
      _codec_type(codec_type),
      _payload_type(payload_type) {}

int DoNothingEncoderAdapter::SampleRateHz() const {
  return _encoder->SampleRateHz();
}

size_t DoNothingEncoderAdapter::NumChannels() const {
  return _encoder->NumChannels();
}

int DoNothingEncoderAdapter::RtpTimestampRateHz() const {
  return _encoder->RtpTimestampRateHz();
}

size_t DoNothingEncoderAdapter::Num10MsFramesInNextPacket() const {
  return _encoder->Num10MsFramesInNextPacket();
}

size_t DoNothingEncoderAdapter::Max10MsFramesInAPacket() const {
  return _encoder->Max10MsFramesInAPacket();
}

int DoNothingEncoderAdapter::GetTargetBitrate() const {
  return _encoder->GetTargetBitrate();
}

void DoNothingEncoderAdapter::Reset() {
  return _encoder->Reset();
}

bool DoNothingEncoderAdapter::SetFec(bool enable) {
  return _encoder->SetFec(enable);
}

bool DoNothingEncoderAdapter::SetDtx(bool enable) {
  return _encoder->SetDtx(enable);
}

bool DoNothingEncoderAdapter::GetDtx() const {
  return _encoder->GetDtx();
}

bool DoNothingEncoderAdapter::SetApplication(Application application) {
  return _encoder->SetApplication(application);
}

void DoNothingEncoderAdapter::SetMaxPlaybackRate(int frequency_hz) {
  _encoder->SetMaxPlaybackRate(frequency_hz);
}

void DoNothingEncoderAdapter::SetTargetBitrate(int target_bps) {
  // DEPRECATED
}

rtc::ArrayView<std::unique_ptr<webrtc::AudioEncoder>>
DoNothingEncoderAdapter::ReclaimContainedEncoders() {
  return _encoder->ReclaimContainedEncoders();
}

bool DoNothingEncoderAdapter::EnableAudioNetworkAdaptor(
    const std::string& config_string,
    webrtc::RtcEventLog* event_log) {
  return _encoder->EnableAudioNetworkAdaptor(config_string, event_log);
}

void DoNothingEncoderAdapter::DisableAudioNetworkAdaptor() {
  _encoder->DisableAudioNetworkAdaptor();
}

void DoNothingEncoderAdapter::OnReceivedUplinkPacketLossFraction(
    float uplink_packet_loss_fraction) {
  _encoder->OnReceivedUplinkPacketLossFraction(uplink_packet_loss_fraction);
}

void DoNothingEncoderAdapter::OnReceivedTargetAudioBitrate(int target_bps) {
  _encoder->OnReceivedTargetAudioBitrate(target_bps);
}

void DoNothingEncoderAdapter::OnReceivedUplinkBandwidth(
    int target_audio_bitrate_bps,
    absl::optional<int64_t> bwe_period_ms) {
  _encoder->OnReceivedUplinkBandwidth(target_audio_bitrate_bps, bwe_period_ms);
}

void DoNothingEncoderAdapter::OnReceivedRtt(int rtt_ms) {
  _encoder->OnReceivedRtt(rtt_ms);
}

void DoNothingEncoderAdapter::OnReceivedOverhead(
    size_t overhead_bytes_per_packet) {
  _encoder->OnReceivedOverhead(overhead_bytes_per_packet);
}

void DoNothingEncoderAdapter::SetReceiverFrameLengthRange(
    int min_frame_length_ms,
    int max_frame_length_ms) {
  _encoder->SetReceiverFrameLengthRange(min_frame_length_ms,
                                        max_frame_length_ms);
}

webrtc::ANAStats DoNothingEncoderAdapter::GetANAStats() const {
  return _encoder->GetANAStats();
}

absl::optional<std::pair<webrtc::TimeDelta, webrtc::TimeDelta>> DoNothingEncoderAdapter::GetFrameLengthRange() const {
  //TODO
  return absl::optional<std::pair<webrtc::TimeDelta, webrtc::TimeDelta>>();
}

webrtc::AudioEncoder::EncodedInfo DoNothingEncoderAdapter::EncodeImpl(
    uint32_t rtp_timestamp,
    rtc::ArrayView<const int16_t> audio,
    rtc::Buffer* encoded) {
  // Audio is offloaded to the DCP, so only pretend to encode
  EncodedInfo info;
  info.payload_type = _payload_type;
  info.send_even_if_empty = false;
  info.speech = true;
  info.encoder_type = _codec_type;
  info.encoded_timestamp = rtp_timestamp;

  size_t encoded_len = audio.size();
  std::vector<uint8_t> fake_encoded_data(encoded_len);
  encoded->AppendData(fake_encoded_data.data(), encoded_len);
  info.encoded_bytes = encoded_len;
  return info;
}

}  // namespace webrtc_integration
}  // namespace dolby_voice_client
