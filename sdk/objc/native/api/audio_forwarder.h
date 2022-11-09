#ifndef SDK_OBJC_NATIVE_API_AUDIO_FORWARDER_H_
#define SDK_OBJC_NATIVE_API_AUDIO_FORWARDER_H_

#include "audio_samples.h"
#include "rtc_base/ref_count.h"

namespace webrtc {
class AudioForwarder : public rtc::RefCountInterface{
public:
  virtual ~AudioForwarder() = default;
  virtual void OnDeliverRecordedData(const AudioSamples& ) = 0;
};
}
#endif  //SDK_OBJC_NATIVE_API_AUDIO_FORWARDER_H_
