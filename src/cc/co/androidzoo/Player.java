package cc.co.androidzoo.jplayer;

import android.content.ContentResolver;
import android.content.Context;
import android.content.res.AssetFileDescriptor;
import android.net.Uri;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.os.Parcel;
import android.os.ParcelFileDescriptor;
import android.os.PowerManager;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.graphics.Bitmap;
import android.media.AudioManager;

import java.io.FileDescriptor;
import java.io.IOException;
import java.util.Set;
import java.lang.ref.WeakReference;

public class Player
{
    static {
        System.loadLibrary("Core");
        native_init();
    }

    private int mNativeContext; // accessed by native methods
    private int mListenerContext; // accessed by native methods
    private Surface mSurface; // accessed by native methods

    private SurfaceHolder  mSurfaceHolder;

    public Player() {

        /* Native setup requires a weak reference to our object.
         * It's easier to create it here than in C++.
         */
        native_setup(new WeakReference<Player>(this));
    }

    public void setDisplay(SurfaceHolder sh) {
        mSurfaceHolder = sh;
        if (sh != null) {
            mSurface = sh.getSurface();
        } else {
            mSurface = null;
        }
        _setVideoSurface();
    }

    public  void start() throws IllegalStateException {
        _start();
    }


    /*
     * Update the MediaPlayer ISurface. Call after updating mSurface.
     */
private native void _setVideoSurface();
public native void setDataSource(String path) throws IOException, IllegalArgumentException, IllegalStateException;
public native void setDataSource(FileDescriptor fd, long offset, long length)
public native void prepare() throws IOException, IllegalStateException;
public native void prepareAsync() throws IllegalStateException;
private native void _start() throws IllegalStateException;
private native void _stop() throws IllegalStateException;
private native void _pause() throws IllegalStateException;
private native void _flush() throws IllegalStateException;
public native int getVideoWidth();
public native int getVideoHeight();
public native boolean isPlaying();
public native void seekTo(int msec) throws IllegalStateException;
public native int getCurrentPosition();
public native int getDuration();
private native void _release();
private native void _reset();
public native void setAudioStreamType(int streamtype);
public native void setLooping(boolean looping);
public native boolean isLooping();
public native void setVolume(float leftVolume, float rightVolume);
public native Bitmap getFrameAt(int msec) throws IllegalStateException;
private native final int native_invoke(Parcel request, Parcel reply);
private native final boolean native_getMetadata(boolean update_only,
                                                    boolean apply_filter,
                                                    Parcel reply);
private native final int native_setMetadataFilter(Parcel request);
private static native final void native_init();
private native final void native_setup(Object mediaplayer_this);
private native final void native_finalize();
public native static int snoop(short [] outData, int kind);

}
