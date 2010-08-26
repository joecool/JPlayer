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
        System.loadLibrary("jplayer_jni");
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
    private static native final void native_init();
    private native final void native_setup(Object mediaplayer_this);
    private native void _setVideoSurface();
    private native void _start();

}
