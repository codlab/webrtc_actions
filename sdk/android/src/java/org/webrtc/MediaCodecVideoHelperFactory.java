package org.webrtc;

import android.media.MediaCodec;
import android.media.MediaCodecInfo;
import android.media.MediaCodecList;
import android.os.Build;
import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;

public final class MediaCodecVideoHelperFactory {
    public final static String TAG = MediaCodecVideoHelperFactory.class.getSimpleName();

    public static IOnHardwareEncoderFailedInitListener hardwareEncoderFailedInitListener;

    private MediaCodecVideoHelperFactory() {

    }

    @NonNull
    public static ArrayList<MediaCodecInfo> getCodecs() {
        ArrayList<MediaCodecInfo> list = new ArrayList<>();
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN) {
            for (int i = 0; i < MediaCodecList.getCodecCount(); ++i) {
                try {
                    list.add(MediaCodecList.getCodecInfoAt(i));
                } catch (IllegalArgumentException e) {
                    Logging.e(TAG, "Cannot retrieve decoder codec info", e);
                }
            }
        }

        // force OMX.google to be the last one
        Collections.sort(list, (left, right) -> {
            if (null == left && null == right) return 0;
            if (null == left) return 1;
            if (null == right) return -1;
            String name_left = left.getName();
            String name_right = left.getName();
            if (null == name_left) name_left = "";
            if (null == name_right) name_right = "";
            if (name_left.indexOf("OMX.google") == 0 && name_right.indexOf("OMX.google") == 0)
                return 0;
            if (name_left.indexOf("OMX.google") == 0) return 1;
            if (name_right.indexOf("OMX.google") == 0) return -1;
            return 0;
        });

        return list;
    }

    //unused for now
    @Nullable
    private static MediaCodecInfo findCodecForType(VideoCodecMimeType type, @NonNull ValidateMediaCodecInfo validate) {
        // HW decoding is not supported on builds before KITKAT.
        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.KITKAT) {
            return null;
        }

        for (int i = 0; i < MediaCodecList.getCodecCount(); ++i) {
            MediaCodecInfo info = null;
            try {
                info = MediaCodecList.getCodecInfoAt(i);
            } catch (IllegalArgumentException e) {
                Logging.e(TAG, "Cannot retrieve decoder codec info", e);
            }

            if (info == null || validate.isValid(info)) {
                continue;
            }

            if (validate.isSupportedCodec(info, type)) {
                return info;
            }
        }

        return null; // No support for this type.
    }

    public static void onHardwareEncoderInitFailed(@NonNull Throwable exception) {
        if (null != hardwareEncoderFailedInitListener) {
            try {
                hardwareEncoderFailedInitListener.onHardwareEncoderInitFailed(exception);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    public interface IOnHardwareEncoderFailedInitListener {
        void onHardwareEncoderInitFailed(@NonNull Throwable exception);
    }

    private abstract class ValidateMediaCodecInfo {
        private int[] color_formats;

        private ValidateMediaCodecInfo() {

        }

        public ValidateMediaCodecInfo(int[] color_formats) {
            this.color_formats = color_formats;
        }

        // Returns true if the given MediaCodecInfo indicates a supported encoder for the given type.
        private boolean isSupportedCodec(MediaCodecInfo info, VideoCodecMimeType type) {
            if (!MediaCodecUtils.codecSupportsType(info, type)) {
                return false;
            }
            // Check for a supported color format.
            if (MediaCodecUtils.selectColorFormat(color_formats, info.getCapabilitiesForType(type.mimeType()))
                    == null) {
                return false;
            }
            return isValid(info, type);
        }

        abstract boolean isValid(@NonNull MediaCodecInfo info);

        abstract boolean isValid(@NonNull MediaCodecInfo info, @NonNull VideoCodecMimeType type);
    }
}
