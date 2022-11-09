#ifndef SDK_OBJC_NATIVE_API_AUDIO_SAMPLES_H_
#define SDK_OBJC_NATIVE_API_AUDIO_SAMPLES_H_

#import <AVFoundation/AVFoundation.h>

namespace webrtc {
struct AudioSamples {
  int              sampleRate;
  AudioBufferList* bufferList;
};
}
#endif //SDK_OBJC_NATIVE_API_AUDIO_SAMPLES_H_
