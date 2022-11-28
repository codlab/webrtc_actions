/*
 *  Copyright 2015 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

package org.webrtc;

import android.graphics.SurfaceTexture;
import android.view.Surface;
import androidx.annotation.Nullable;
import java.util.ArrayList;
import javax.microedition.khronos.egl.EGL10;

/**
 * Holds EGL state and utility methods for handling an egl 1.0 EGLContext, an EGLDisplay,
 * and an EGLSurface.
 */
public interface EglBase {

  void createSurface(Surface surface);

  // Create EGLSurface from the Android SurfaceTexture.
  void createSurface(SurfaceTexture surfaceTexture);

  // Create dummy 1x1 pixel buffer surface so the context can be made current.
  void createDummyPbufferSurface();

  void createPbufferSurface(int width, int height);

  EglBaseInteracts.Context getEglBaseContext();

  boolean hasSurface();

  int surfaceWidth();

  int surfaceHeight();

  void releaseSurface();

  void release();

  void makeCurrent();

  // Detach the current EGL context, so that it can be made current on another thread.
  void detachCurrent();

  void swapBuffers();

  void swapBuffers(long presentationTimeStampNs);
}
