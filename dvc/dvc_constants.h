// Copyright 2020 Dolby Laboratories and Dolby International AB
// Use of this source code is governed by the MIT license that can be
// found in the dolby/LICENSE file.

#ifndef WEBRTC_DVC_CONSTANTS_H_
#define WEBRTC_DVC_CONSTANTS_H_

namespace dolby_voice_client {
namespace webrtc_integration {

#define DVC_CODEC_NAME_INITIALIZER \
  { 'D', 'V', 'C', '-', '2', '\0' }

static constexpr const char DVC_CODEC_NAME[] = DVC_CODEC_NAME_INITIALIZER;
static const int DVC_PAYLOAD_TYPE = 96;
static const int DVC_SAMPLING_RATES[] = {8000, 48000};
static const int DVC_CHANNELS = 1;
static const int DVC_BITRATE = 0;
static const int DVC_MAX_DELTA_DELAY_MS = 900;
static const int DVC_NUMBER_10MS_FRAMES_IN_PACKET = 2;

#undef DVC_CODEC_NAME_INITIALIZER

}  // namespace webrtc_integration
}  // namespace dolby_voice_client

#endif /* WEBRTC_DVC_CONSTANTS_H_ */
