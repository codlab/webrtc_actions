
/*
 *  Copyright 2011 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef PC_PEER_CONNECTION_FACTORY_PROVIDER_H_
#define PC_PEER_CONNECTION_FACTORY_PROVIDER_H_

#include <memory>
#include <string>

#include "peer_connection_factory.h"
#include "peer_connection.h"
#include "api/scoped_refptr.h"

namespace webrtc {

class RtcEventLog;

class PeerConnectionFactoryProviderBase {

public:

  virtual ~PeerConnectionFactoryProviderBase() { }
  virtual rtc::scoped_refptr<PeerConnection> createPc(rtc::scoped_refptr<ConnectionContext> context,
                                                      const PeerConnectionFactoryInterface::Options& options,
                                                      bool is_unified_plan,
                                                      std::unique_ptr<RtcEventLog> event_log,
                                                      std::unique_ptr<Call> call,
                                                      PeerConnectionDependencies& dependencies,
                                                      bool dtls_enabled) = 0;
};

class PeerConnectionFactoryProvider : public webrtc::PeerConnectionFactoryInterface {
public:

  static webrtc::PeerConnectionFactoryProviderBase* provider;
  static rtc::scoped_refptr<PeerConnection> createPc(rtc::scoped_refptr<ConnectionContext> context,
                                                     const PeerConnectionFactoryInterface::Options& options,
                                                     bool is_unified_plan,
                                                     std::unique_ptr<RtcEventLog> event_log,
                                                     std::unique_ptr<Call> call,
                                                     PeerConnectionDependencies& dependencies,
                                                     bool dtls_enabled) {
    if (provider) {
     return provider->createPc(context,
                               options,
                               is_unified_plan,
                               std::move(event_log),
                               std::move(call),
                               dependencies,
                               dtls_enabled);
    }
    return nullptr;
  }
};

}  // namespace webrtc

#endif  // PC_PEER_CONNECTION_FACTORY_H_
