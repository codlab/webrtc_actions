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

import android.content.Context;
import android.graphics.Rect;
import android.graphics.SurfaceTexture;
import android.hardware.camera2.CameraCharacteristics;
import android.hardware.camera2.CameraManager;
import android.hardware.camera2.CameraMetadata;
import android.hardware.camera2.params.StreamConfigurationMap;
import android.os.Build;
import android.os.SystemClock;
import android.util.AndroidException;
import android.util.Range;
import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import org.webrtc.CameraEnumerationAndroid.CaptureFormat;

public class Camera2Enumerator implements CameraEnumerator {
  private final static String TAG = "Camera2Enumerator";
  private final static double NANO_SECONDS_PER_SECOND = 1.0e9;

  // Each entry contains the supported formats for a given camera index. The formats are enumerated
  // lazily in getSupportedFormats(), and cached for future reference.
  private static final Map<String, List<CaptureFormat>> cachedSupportedFormats =
      new HashMap<String, List<CaptureFormat>>();

  // Each entry contains the supported sizes for a given camera index. The sizes are enumerated
  // lazily in getSupportedSizes(), and cached for future reference.
  private static final Map<String, List<Size>> cachedSupportedSizes = new HashMap<>();

  @NonNull private String[] validCameras;

  final Context context;
  @Nullable final CameraManager cameraManager;

  public Camera2Enumerator(Context context) {
    this.context = context;
    this.cameraManager = (CameraManager) context.getSystemService(Context.CAMERA_SERVICE);
    this.validCameras = computeFilteredDeviceNames();
  }

  @Override
  public String[] getDeviceNames() {
    if (validCameras.length > 0) return validCameras;
    String[] names = getRawDevicesName();
    return null != names ? names : new String[0];
  }

  @Override
  public boolean isFrontFacing(String deviceName) {
    CameraCharacteristics characteristics = getCameraCharacteristics(deviceName);

    return characteristics != null
        && characteristics.get(CameraCharacteristics.LENS_FACING)
        == CameraMetadata.LENS_FACING_FRONT;
  }

  @Override
  public boolean isBackFacing(String deviceName) {
    CameraCharacteristics characteristics = getCameraCharacteristics(deviceName);

    return characteristics != null
        && characteristics.get(CameraCharacteristics.LENS_FACING)
        == CameraMetadata.LENS_FACING_BACK;
  }

  @Nullable
  @Override
  public List<CaptureFormat> getSupportedFormats(String deviceName) {
    return getSupportedFormats(context, deviceName);
  }

  @Override
  public CameraVideoCapturer createCapturer(
      String deviceName, CameraVideoCapturer.CameraEventsHandler eventsHandler) {
    return new Camera2Capturer(context, deviceName, eventsHandler);
  }

  /**
   * Compute the list of usable cameras into the validCameras list
   */
  private @NonNull String[] computeFilteredDeviceNames() {
    if (null == cameraManager) return new String[0];

    String[] devices = getRawDevicesName();
    ArrayList<String> filtered = new ArrayList();

    for (String device: devices) {
      if(getSupportedFormats(cameraManager, device).size() > 0) {
        filtered.add(device);
      }
    }

    return filtered.toArray(new String[0]);
  }

  private @Nullable CameraCharacteristics getCameraCharacteristics(String deviceName) {
    try {
      return cameraManager.getCameraCharacteristics(deviceName);
      // On Android OS pre 4.4.2, a class will not load because of VerifyError if it contains a
      // catch statement with an Exception from a newer API, even if the code is never executed.
      // https://code.google.com/p/android/issues/detail?id=209129
    } catch (/* CameraAccessException */ AndroidException e) {
      Logging.e(TAG, "Camera access exception", e);
      return null;
    }
  }

  /**
   * Checks if API is supported and all cameras have better than legacy support.
   */
  public static boolean isSupported(Context context) {
    CameraManager cameraManager = (CameraManager) context.getSystemService(Context.CAMERA_SERVICE);
    try {
      String[] cameraIds = cameraManager.getCameraIdList();
      for (String id : cameraIds) {
        CameraCharacteristics characteristics = cameraManager.getCameraCharacteristics(id);
        if (characteristics.get(CameraCharacteristics.INFO_SUPPORTED_HARDWARE_LEVEL)
            == CameraCharacteristics.INFO_SUPPORTED_HARDWARE_LEVEL_LEGACY) {
          return false;
        }
      }
      // On Android OS pre 4.4.2, a class will not load because of VerifyError if it contains a
      // catch statement with an Exception from a newer API, even if the code is never executed.
      // https://code.google.com/p/android/issues/detail?id=209129
    } catch (/* CameraAccessException */ AndroidException | RuntimeException e) {
      Logging.e(TAG, "Failed to check if camera2 is supported", e);
      return false;
    }
    return true;
  }

  /**
   * Get the list of system wide accessible cameras
   * @return the raw list of devices from the camera manager service
   */
  private String[] getRawDevicesName() {
    try {
      return cameraManager.getCameraIdList();
      // On Android OS pre 4.4.2, a class will not load because of VerifyError if it contains a
      // catch statement with an Exception from a newer API, even if the code is never executed.
      // https://code.google.com/p/android/issues/detail?id=209129
    } catch (/* CameraAccessException */ AndroidException e) {
      Logging.e(TAG, "Camera access exception: " + e);
      return new String[] {};
    }
  }

  static int getFpsUnitFactor(Range<Integer>[] fpsRanges) {
    if (fpsRanges.length == 0) {
      return 1000;
    }
    return fpsRanges[0].getUpper() < 1000 ? 1000 : 1;
  }

  static List<Size> getSupportedSizes(@NonNull CameraCharacteristics cameraCharacteristics, @Nullable String optionalCameraId) {
    final StreamConfigurationMap streamMap =
        cameraCharacteristics.get(CameraCharacteristics.SCALER_STREAM_CONFIGURATION_MAP);
    final Integer supportLevel =
        cameraCharacteristics.get(CameraCharacteristics.INFO_SUPPORTED_HARDWARE_LEVEL);

    final android.util.Size[] nativeSizes = streamMap.getOutputSizes(SurfaceTexture.class);
    final List<Size> sizes = convertSizes(nativeSizes);

    if(sizes.size() == 0 && null != optionalCameraId && cachedSupportedSizes.containsKey(optionalCameraId)) {
      List<Size> cached = cachedSupportedSizes.get(optionalCameraId);
      if (null != cached) {
        Logging.d(TAG, "Get supported sizes for camera index " + optionalCameraId + " using cache.");

        return cached;
      } // else never happen but for safety check, this is mandatory
    }

    // Video may be stretched pre LMR1 on legacy implementations.
    // Filter out formats that have different aspect ratio than the sensor array.
    if (Build.VERSION.SDK_INT < Build.VERSION_CODES.LOLLIPOP_MR1
          && null != supportLevel && supportLevel == CameraCharacteristics.INFO_SUPPORTED_HARDWARE_LEVEL_LEGACY) {
      final Rect activeArraySize =
          cameraCharacteristics.get(CameraCharacteristics.SENSOR_INFO_ACTIVE_ARRAY_SIZE);
      final ArrayList<Size> filteredSizes = new ArrayList<Size>();

      for (Size size : sizes) {
        if (activeArraySize.width() * size.height == activeArraySize.height() * size.width) {
          filteredSizes.add(size);
        }
      }

      if(null != optionalCameraId) cachedSupportedSizes.put(optionalCameraId, filteredSizes);
      return filteredSizes;
    } else {
      if(null != optionalCameraId) cachedSupportedSizes.put(optionalCameraId, sizes);
      return sizes;
    }
  }

  @Nullable
  static List<CaptureFormat> getSupportedFormats(Context context, String cameraId) {
    return getSupportedFormats(
        (CameraManager) context.getSystemService(Context.CAMERA_SERVICE), cameraId);
  }

  @Nullable
  static List<CaptureFormat> getSupportedFormats(CameraManager cameraManager, String cameraId) {
    synchronized (cachedSupportedFormats) {
      if (cachedSupportedFormats.containsKey(cameraId)) {
        List<CaptureFormat> cached = cachedSupportedFormats.get(cameraId);

        if (null != cached) {
          return cached;
        } // else never happen but for safety check, this is mandatory
      }

      Logging.d(TAG, "Get supported formats for camera index " + cameraId + ".");
      final long startTimeMs = SystemClock.elapsedRealtime();

      final CameraCharacteristics cameraCharacteristics;
      try {
        cameraCharacteristics = cameraManager.getCameraCharacteristics(cameraId);
      } catch (Exception ex) {
        Logging.e(TAG, "getCameraCharacteristics()", ex);
        return new ArrayList<CaptureFormat>();
      }

      final StreamConfigurationMap streamMap =
          cameraCharacteristics.get(CameraCharacteristics.SCALER_STREAM_CONFIGURATION_MAP);

      Range<Integer>[] fpsRanges =
          cameraCharacteristics.get(CameraCharacteristics.CONTROL_AE_AVAILABLE_TARGET_FPS_RANGES);
      List<CaptureFormat.FramerateRange> framerateRanges =
          convertFramerates(fpsRanges, getFpsUnitFactor(fpsRanges));
      List<Size> sizes = getSupportedSizes(cameraCharacteristics, cameraId);

      int defaultMaxFps = 0;
      for (CaptureFormat.FramerateRange framerateRange : framerateRanges) {
        defaultMaxFps = Math.max(defaultMaxFps, framerateRange.max);
      }

      final List<CaptureFormat> formatList = new ArrayList<CaptureFormat>();
      for (Size size : sizes) {
        long minFrameDurationNs = 0;
        try {
          minFrameDurationNs = streamMap.getOutputMinFrameDuration(
              SurfaceTexture.class, new android.util.Size(size.width, size.height));
        } catch (Exception e) {
          // getOutputMinFrameDuration() is not supported on all devices. Ignore silently.
        }
        final int maxFps = (minFrameDurationNs == 0)
            ? defaultMaxFps
            : (int) Math.round(NANO_SECONDS_PER_SECOND / minFrameDurationNs) * 1000;
        formatList.add(new CaptureFormat(size.width, size.height, 0, maxFps));
        Logging.d(TAG, "Format: " + size.width + "x" + size.height + "@" + maxFps);
      }

      //only set the cached format list if required
      if(formatList.size() > 0) {
        cachedSupportedFormats.put(cameraId, formatList);
      }

      final long endTimeMs = SystemClock.elapsedRealtime();
      Logging.d(TAG, "Get supported formats for camera index " + cameraId + " done."
              + " Time spent: " + (endTimeMs - startTimeMs) + " ms.");
      return formatList;
    }
  }

  // Convert from android.util.Size to Size.
  private static List<Size> convertSizes(android.util.Size[] cameraSizes) {
    if (cameraSizes == null || cameraSizes.length == 0) {
      return Collections.emptyList();
    }
    final List<Size> sizes = new ArrayList<>(cameraSizes.length);
    try {
      for (android.util.Size size : cameraSizes) {
        sizes.add(new Size(size.getWidth(), size.getHeight()));
      }
    } catch(Throwable exception) {
      Logging.e(TAG, "convertSizes exception", exception);
    }
    return sizes;
  }

  // Convert from android.util.Range<Integer> to CaptureFormat.FramerateRange.
  static List<CaptureFormat.FramerateRange> convertFramerates(
      Range<Integer>[] arrayRanges, int unitFactor) {
    final List<CaptureFormat.FramerateRange> ranges = new ArrayList<CaptureFormat.FramerateRange>();
    for (Range<Integer> range : arrayRanges) {
      ranges.add(new CaptureFormat.FramerateRange(
          range.getLower() * unitFactor, range.getUpper() * unitFactor));
    }
    return ranges;
  }
}
