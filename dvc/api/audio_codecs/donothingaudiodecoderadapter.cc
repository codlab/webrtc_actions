// Copyright 2020 Dolby Laboratories and Dolby International AB
// Use of this source code is governed by the MIT license that can be
// found in the dolby/LICENSE file.

#include "dvc/api/audio_codecs/donothingaudiodecoderadapter.h"

namespace dolby_voice_client {
namespace webrtc_integration {

DoNothingDecoderAdapter::DoNothingDecoderAdapter(
    std::unique_ptr<webrtc::AudioDecoder> decoder)
    : _decoder(std::move(decoder)) {}

bool DoNothingDecoderAdapter::HasDecodePlc() const {
  return _decoder->HasDecodePlc();
}

size_t DoNothingDecoderAdapter::DecodePlc(size_t num_frames, int16_t* decoded) {
  return _decoder->DecodePlc(num_frames, decoded);
}

void DoNothingDecoderAdapter::Reset() {
  _decoder->Reset();
}

int DoNothingDecoderAdapter::ErrorCode() {
  return _decoder->ErrorCode();
}

int DoNothingDecoderAdapter::PacketDuration(const uint8_t* encoded,
                                            size_t encoded_len) const {
  return _decoder->PacketDuration(encoded, encoded_len);
}

int DoNothingDecoderAdapter::PacketDurationRedundant(const uint8_t* encoded,
                                                     size_t encoded_len) const {
  return _decoder->PacketDurationRedundant(encoded, encoded_len);
}

bool DoNothingDecoderAdapter::PacketHasFec(const uint8_t* encoded,
                                           size_t encoded_len) const {
  return _decoder->PacketHasFec(encoded, encoded_len);
}

int DoNothingDecoderAdapter::SampleRateHz() const {
  return _decoder->SampleRateHz();
}

size_t DoNothingDecoderAdapter::Channels() const {
  return _decoder->Channels();
}

int DoNothingDecoderAdapter::DecodeInternal(const uint8_t* encoded,
                                            size_t encoded_len,
                                            int /* sample_rate_hz */,
                                            int16_t* decoded,
                                            SpeechType* speech_type) {
  // Audio is offloaded to the DVC, so only pretend to decode.
  // Decoded buffer size in ensured by value returned in PacketDuration()
  // function.
  int decoded_samples = static_cast<int>(
      _decoder->PacketDuration(encoded, encoded_len) * _decoder->Channels());
  size_t decoded_buffer_size =
      static_cast<size_t>(decoded_samples) * sizeof(int16_t);
  std::memset(decoded, 0, decoded_buffer_size);
  *speech_type = kSpeech;
  return decoded_samples;
}

}  // namespace webrtc_integration
}  // namespace dolby_voice_client
