#define LOG_TAG  "Flicker/Core"

#include <stdio.h>
#include <string.h>
#include "jni.h"
#include "JNIHelp.h"
#include <sys/socket.h>
#include <net/if.h>
//#include "netinet/In.h"
#include <assert.h>
//#include <android/log.h>
//#include <utils/threads.h>
#include "utils/Log.h"
#include "android_runtime/AndroidRuntime.h"
#include "utils/Errors.h"  // for status_t
#include "player.h"

namespace androidzoo{
using android::Surface;
using android::sp;
//using android::jniThrowException;
using android::AndroidRuntime;

struct fields_t {
    jfieldID    context;
    jfieldID    surface;
    /* actually in android.view.Surface XXX */
    jfieldID    surface_native;

    jmethodID   post_event;
};
static fields_t fields;

static Surface* get_surface(JNIEnv* env, jobject clazz)
{
    return (Surface*)env->GetIntField(clazz, fields.surface_native);
}

static sp<Player> getPlayer(JNIEnv* env, jobject thiz)
{
    Player* const p = (Player*)env->GetIntField(thiz, fields.context);
    return sp<Player>(p);
}

static sp<Player> setPlayer(JNIEnv* env, jobject thiz, const sp<Player>& player)
{
    sp<Player> old = (Player*)env->GetIntField(thiz, fields.context);
    if (player.get()) {
        player->incStrong(thiz);
    }
    if (old != 0) {
        old->decStrong(thiz);
    }
    env->SetIntField(thiz, fields.context, (int)player.get());
    return old;
}

// If exception is NULL and opStatus is not OK, this method sends an error
// event to the client application; otherwise, if exception is not NULL and
// opStatus is not OK, this method throws the given exception to the client
// application.
static void process_media_player_call(JNIEnv *env, jobject thiz, status_t opStatus, const char* exception, const char *message)
{
}

static void
cc_co_androidzoo_jplayer_setDataSource(JNIEnv *env, jobject thiz, jstring path)
{
    sp<Player> mp = getPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }

    if (path == NULL) {
        jniThrowException(env, "java/lang/IllegalArgumentException", NULL);
        return;
    }

    const char *pathStr = env->GetStringUTFChars(path, NULL);
    if (pathStr == NULL) {  // Out of memory
        jniThrowException(env, "java/lang/RuntimeException", "Out of memory");
        return;
    }
    LOGV("setDataSource: path %s", pathStr);
    status_t opStatus = mp->setDataSource(pathStr);

    // Make sure that local ref is released before a potential exception
    env->ReleaseStringUTFChars(path, pathStr);
    process_media_player_call( env, thiz, opStatus, "java/io/IOException", "setDataSource failed." );
}

static void
cc_co_androidzoo_jplayer_setDataSourceFD(JNIEnv *env, jobject thiz, jobject fileDescriptor, jlong offset, jlong length)
{/*
    sp<Player> mp = getPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }

    if (fileDescriptor == NULL) {
        jniThrowException(env, "java/lang/IllegalArgumentException", NULL);
        return;
    }
    int fd = getParcelFileDescriptorFD(env, fileDescriptor);
    LOGV("setDataSourceFD: fd %d", fd);
    process_media_player_call( env, thiz, mp->setDataSource(fd, offset, length), "java/io/IOException", "setDataSourceFD failed." );
    */
}

static void setVideoSurface(const sp<Player>& mp, JNIEnv *env, jobject thiz)
{
    jobject surface = env->GetObjectField(thiz, fields.surface);
    if (surface != NULL) {
        const sp<Surface> native_surface = get_surface(env, surface);
        LOGV("prepare: surface=%p (id=%d)",
             native_surface.get(), native_surface->ID());
        mp->setVideoSurface(native_surface);
    }
}

static void
cc_co_androidzoo_jplayer_setVideoSurface(JNIEnv *env, jobject thiz)
{
    sp<Player> mp = getPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }
    setVideoSurface(mp, env, thiz);
}

static void
cc_co_androidzoo_jplayer_prepare(JNIEnv *env, jobject thiz)
{
    sp<Player> mp = getPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }
    setVideoSurface(mp, env, thiz);
    process_media_player_call( env, thiz, mp->prepare(), "java/io/IOException", "Prepare failed." );
}

static void
cc_co_androidzoo_jplayer_prepareAsync(JNIEnv *env, jobject thiz)
{
    sp<Player> mp = getPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }
    jobject surface = env->GetObjectField(thiz, fields.surface);
    if (surface != NULL) {
        const sp<Surface> native_surface = get_surface(env, surface);
        LOGV("prepareAsync: surface=%p (id=%d)",
             native_surface.get(), native_surface->ID());
        mp->setVideoSurface(native_surface);
    }
    process_media_player_call( env, thiz, mp->prepareAsync(), "java/io/IOException", "Prepare Async failed." );
}

static void
cc_co_androidzoo_jplayer_start(JNIEnv *env, jobject thiz)
{
    LOGV("start");
    sp<Player> mp = getPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }
    process_media_player_call( env, thiz, mp->start(), NULL, NULL );
}

static void
cc_co_androidzoo_jplayer_stop(JNIEnv *env, jobject thiz)
{
    LOGV("stop");
    sp<Player> mp = getPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }
    process_media_player_call( env, thiz, mp->stop(), NULL, NULL );
}

static void
cc_co_androidzoo_jplayer_pause(JNIEnv *env, jobject thiz)
{
    LOGV("pause");
    sp<Player> mp = getPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }
    process_media_player_call( env, thiz, mp->pause(), NULL, NULL );
}

static void
cc_co_androidzoo_jplayer_flush(JNIEnv *env, jobject thiz)
{
    LOGV("flush");
    sp<Player> mp = getPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }
//    process_media_player_call( env, thiz, mp->flush(), NULL, NULL );
}

static jboolean
cc_co_androidzoo_jplayer_isPlaying(JNIEnv *env, jobject thiz)
{
    sp<Player> mp = getPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return false;
    }
    const jboolean is_playing = mp->isPlaying();

    LOGV("isPlaying: %d", is_playing);
    return is_playing;
}

static void
cc_co_androidzoo_jplayer_seekTo(JNIEnv *env, jobject thiz, int msec)
{
    sp<Player> mp = getPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }
    LOGV("seekTo: %d(msec)", msec);
    process_media_player_call( env, thiz, mp->seekTo(msec), NULL, NULL );
}

static int
cc_co_androidzoo_jplayer_getVideoWidth(JNIEnv *env, jobject thiz)
{
    sp<Player> mp = getPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return 0;
    }
    int w;
	/*
    if (0 != mp->getVideoWidth(&w)) {
        LOGE("getVideoWidth failed");
        w = 0;
    }*/
    LOGV("getVideoWidth: %d", w);
    return w;
}

static int
cc_co_androidzoo_jplayer_getVideoHeight(JNIEnv *env, jobject thiz)
{
    sp<Player> mp = getPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return 0;
    }
    int h;/*
    if (0 != mp->getVideoHeight(&h)) {
        LOGE("getVideoHeight failed");
        h = 0;
    }*/
    LOGV("getVideoHeight: %d", h);
    return h;
}


static int
cc_co_androidzoo_jplayer_getCurrentPosition(JNIEnv *env, jobject thiz)
{
    sp<Player> mp = getPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return 0;
    }
    int msec;
    process_media_player_call( env, thiz, mp->getCurrentPosition(&msec), NULL, NULL );
    LOGV("getCurrentPosition: %d (msec)", msec);
    return msec;
}

static int
cc_co_androidzoo_jplayer_getDuration(JNIEnv *env, jobject thiz)
{
    sp<Player> mp = getPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return 0;
    }
    int msec;
    process_media_player_call( env, thiz, mp->getDuration(&msec), NULL, NULL );
    LOGV("getDuration: %d (msec)", msec);
    return msec;
}

static void
cc_co_androidzoo_jplayer_reset(JNIEnv *env, jobject thiz)
{
    LOGV("reset");
    sp<Player> mp = getPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }
    process_media_player_call( env, thiz, mp->reset(), NULL, NULL );
}

static void
cc_co_androidzoo_jplayer_setAudioStreamType(JNIEnv *env, jobject thiz, int streamtype)
{
    LOGV("setAudioStreamType: %d", streamtype);
    sp<Player> mp = getPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }
//    process_media_player_call( env, thiz, mp->setAudioStreamType(streamtype) , NULL, NULL );
}

static void
cc_co_androidzoo_jplayer_setLooping(JNIEnv *env, jobject thiz, jboolean looping)
{
    LOGV("setLooping: %d", looping);
    sp<Player> mp = getPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }
    process_media_player_call( env, thiz, mp->setLooping(looping), NULL, NULL );
}

static jboolean
cc_co_androidzoo_jplayer_isLooping(JNIEnv *env, jobject thiz)
{
    LOGV("isLooping");
    sp<Player> mp = getPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return false;
    }
	return false;
    //return mp->isLooping();
}

static void
cc_co_androidzoo_jplayer_setVolume(JNIEnv *env, jobject thiz, float leftVolume, float rightVolume)
{
    LOGV("setVolume: left %f  right %f", leftVolume, rightVolume);
    sp<Player> mp = getPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }
//    process_media_player_call( env, thiz, mp->setVolume(leftVolume, rightVolume), NULL, NULL );
}

// FIXME: deprecated
static jobject
cc_co_androidzoo_jplayer_getFrameAt(JNIEnv *env, jobject thiz, jint msec)
{
    return NULL;
}


// Sends the request and reply parcels to the media player via the
// binder interface.
static jint
cc_co_androidzoo_jplayer_invoke(JNIEnv *env, jobject thiz,
                                 jobject java_request, jobject java_reply)
{

	return 0;
}

// Sends the new filter to the client.
static jint
cc_co_androidzoo_jplayer_setMetadataFilter(JNIEnv *env, jobject thiz, jobject request)
{
	return 0;
}

static jboolean
cc_co_androidzoo_jplayer_getMetadata(JNIEnv *env, jobject thiz, jboolean update_only,
                                      jboolean apply_filter, jobject reply)
{
	return 0;
}

// This function gets some field IDs, which in turn causes class initialization.
// It is called from a static block in MediaPlayer, which won't run until the
// first time an instance of this class is used.
static void
cc_co_androidzoo_jplayer_native_init(JNIEnv *env)
{
    jclass clazz;

    clazz = env->FindClass("cc/co/androidzoo/jplayer/Player");
    if (clazz == NULL) {
        jniThrowException(env, "java/lang/RuntimeException", "Can't find android/media/MediaPlayer");
        return;
    }

    fields.context = env->GetFieldID(clazz, "mNativeContext", "I");
    if (fields.context == NULL) {
        jniThrowException(env, "java/lang/RuntimeException", "Can't find MediaPlayer.mNativeContext");
        return;
    }

    fields.post_event = env->GetStaticMethodID(clazz, "postEventFromNative",
                                               "(Ljava/lang/Object;IIILjava/lang/Object;)V");
    if (fields.post_event == NULL) {
        jniThrowException(env, "java/lang/RuntimeException", "Can't find MediaPlayer.postEventFromNative");
        return;
    }

    fields.surface = env->GetFieldID(clazz, "mSurface", "Landroid/view/Surface;");
    if (fields.surface == NULL) {
        jniThrowException(env, "java/lang/RuntimeException", "Can't find MediaPlayer.mSurface");
        return;
    }

    jclass surface = env->FindClass("android/view/Surface");
    if (surface == NULL) {
        jniThrowException(env, "java/lang/RuntimeException", "Can't find android/view/Surface");
        return;
    }

    fields.surface_native = env->GetFieldID(surface, "mSurface", "I");
    if (fields.surface_native == NULL) {
        jniThrowException(env, "java/lang/RuntimeException", "Can't find Surface.mSurface");
        return;
    }
}

static void
cc_co_androidzoo_jplayer_native_setup(JNIEnv *env, jobject thiz, jobject weak_this)
{
    LOGV("native_setup");
    sp<Player> mp = new Player();
    if (mp == NULL) {
        jniThrowException(env, "java/lang/RuntimeException", "Out of memory");
        return;
    }
/*
    // create new listener and give it to MediaPlayer
    sp<JNIMediaPlayerListener> listener = new JNIMediaPlayerListener(env, thiz, weak_this);
    mp->setListener(listener);
*/
    // Stow our new C++ MediaPlayer in an opaque field in the Java object.
    setPlayer(env, thiz, mp);
}

static void
cc_co_androidzoo_jplayer_release(JNIEnv *env, jobject thiz)
{
    LOGV("release");
    sp<Player> mp = setPlayer(env, thiz, 0);
    if (mp != NULL) {
        // this prevents native callbacks after the object is released
//        mp->setListener(0);
 //       mp->disconnect();
    }
}

static void
cc_co_androidzoo_jplayer_native_finalize(JNIEnv *env, jobject thiz)
{
    LOGV("native_finalize");
    cc_co_androidzoo_jplayer_release(env, thiz);
}

static jint
cc_co_androidzoo_jplayer_snoop(JNIEnv* env, jobject thiz, jobject data, jint kind) {
	return 0;
}

static JNINativeMethod gMethods[] = {
    {"setDataSource",       "(Ljava/lang/String;)V",            (void *)cc_co_androidzoo_jplayer_setDataSource},
    {"setDataSource",       "(Ljava/io/FileDescriptor;JJ)V",    (void *)cc_co_androidzoo_jplayer_setDataSourceFD},
    {"_setVideoSurface",    "()V",                              (void *)cc_co_androidzoo_jplayer_setVideoSurface},
    {"prepare",             "()V",                              (void *)cc_co_androidzoo_jplayer_prepare},
    {"prepareAsync",        "()V",                              (void *)cc_co_androidzoo_jplayer_prepareAsync},
    {"_start",              "()V",                              (void *)cc_co_androidzoo_jplayer_start},
    {"_stop",               "()V",                              (void *)cc_co_androidzoo_jplayer_stop},
    {"getVideoWidth",       "()I",                              (void *)cc_co_androidzoo_jplayer_getVideoWidth},
    {"getVideoHeight",      "()I",                              (void *)cc_co_androidzoo_jplayer_getVideoHeight},
    {"seekTo",              "(I)V",                             (void *)cc_co_androidzoo_jplayer_seekTo},
    {"_pause",              "()V",                              (void *)cc_co_androidzoo_jplayer_pause},
    {"_flush",              "()V",                              (void *)cc_co_androidzoo_jplayer_flush},
    {"isPlaying",           "()Z",                              (void *)cc_co_androidzoo_jplayer_isPlaying},
    {"getCurrentPosition",  "()I",                              (void *)cc_co_androidzoo_jplayer_getCurrentPosition},
    {"getDuration",         "()I",                              (void *)cc_co_androidzoo_jplayer_getDuration},
    {"_release",            "()V",                              (void *)cc_co_androidzoo_jplayer_release},
    {"_reset",              "()V",                              (void *)cc_co_androidzoo_jplayer_reset},
    {"setAudioStreamType",  "(I)V",                             (void *)cc_co_androidzoo_jplayer_setAudioStreamType},
    {"setLooping",          "(Z)V",                             (void *)cc_co_androidzoo_jplayer_setLooping},
    {"isLooping",           "()Z",                              (void *)cc_co_androidzoo_jplayer_isLooping},
    {"setVolume",           "(FF)V",                            (void *)cc_co_androidzoo_jplayer_setVolume},
    {"getFrameAt",          "(I)Landroid/graphics/Bitmap;",     (void *)cc_co_androidzoo_jplayer_getFrameAt},
    {"native_invoke",       "(Landroid/os/Parcel;Landroid/os/Parcel;)I",(void *)cc_co_androidzoo_jplayer_invoke},
    {"native_setMetadataFilter", "(Landroid/os/Parcel;)I",      (void *)cc_co_androidzoo_jplayer_setMetadataFilter},
    {"native_getMetadata", "(ZZLandroid/os/Parcel;)Z",          (void *)cc_co_androidzoo_jplayer_getMetadata},
    {"native_init",         "()V",                              (void *)cc_co_androidzoo_jplayer_native_init},
    {"native_setup",        "(Ljava/lang/Object;)V",            (void *)cc_co_androidzoo_jplayer_native_setup},
    {"native_finalize",     "()V",                              (void *)cc_co_androidzoo_jplayer_native_finalize},
    {"snoop",               "([SI)I",                           (void *)cc_co_androidzoo_jplayer_snoop},
};

static int Java_cc_co_android_jplayer_register(JNIEnv* env)
{

    return AndroidRuntime::registerNativeMethods(env,
                "cc/co/androidzoo/jplayer/Player", gMethods, NELEM(gMethods));
}


jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    JNIEnv* env = NULL;
    jint result = -1;

    LOGV("VERBOSE: JNI_OnLoad\n");
	
    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
      LOGV("ERROR: GetEnv failed\n");
      return result;
    }
    assert(env != NULL);

    if(0 > Java_cc_co_android_jplayer_register(env))
      {
        return result;
      }

    /* success -- return valid version number */
    result = JNI_VERSION_1_4;

    return result;

}
}
