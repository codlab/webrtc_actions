// Copyright 2020 Dolby Laboratories and Dolby International AB
// Use of this source code is governed by the MIT license that can be
// found in the dolby/LICENSE file.

#ifndef WEBRTC_DVC_COMMON_H_
#define WEBRTC_DVC_COMMON_H_

#include "media/base/codec.h"

namespace dolby_voice_client {
namespace webrtc_integration {

static const size_t kMaxRtpPacketLen = 2048;
static const size_t kMaxPendingPackets = 512;

}  // namespace webrtc_integration
}  // namespace dolby_voice_client

#endif /* WEBRTC_DVC_COMMON_H_ */
