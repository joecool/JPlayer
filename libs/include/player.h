#ifndef CC_CO_ANDROIDZOO_JPLAYER_PLAYER_H
#define CC_CO_ANDROIDZOO_JPLAYER_PLAYER_H

#include <binder/IMemory.h>
#include <ui/Surface.h>
#include "utils/Errors.h"  // for status_t

namespace androidzoo {
using android::Surface;
using android::sp;

class Player
{

public:
    Player();
    ~Player();
            void            onFirstRef();
            status_t        setDataSource(const char *url);
            status_t        setDataSource(int fd, int64_t offset, int64_t length);
            status_t        setVideoSurface(const sp<Surface>& surface);
            status_t        prepare();
            status_t        prepareAsync();
            status_t        start();
            status_t        stop();
            status_t        pause();
            status_t        flush();
            bool            isPlaying();
            status_t        getVideoWidth(int *w);
            status_t        getVideoHeight(int *h);
            status_t        seekTo(int msec);
            status_t        getCurrentPosition(int *msec);
            status_t        getDuration(int *msec);
            status_t        reset();
            status_t        setAudioStreamType(int type);
            status_t        setLooping(int loop);
            bool            isLooping();
            status_t        setVolume(float leftVolume, float rightVolume);
            void            notify(int msg, int ext1, int ext2);
    static  sp<IMemory>     decode(const char* url, uint32_t *pSampleRate, int* pNumChannels, int* pFormat);
    static  sp<IMemory>     decode(int fd, int64_t offset, int64_t length, uint32_t *pSampleRate, int* pNumChannels, int* pFormat);
    static  int             snoop(short *data, int len, int kind);
            status_t        invoke(const Parcel& request, Parcel *reply);
            status_t        setMetadataFilter(const Parcel& filter);
            status_t        getMetadata(bool update_only, bool apply_filter, Parcel *metadata);

            status_t        handleKeyEvent(int keyCode, int action);
	    status_t        handleTouchEvent(int x, int y, int action);
	    status_t        setInputText(char *pInputStr, int inputLen);
	    status_t        handleData(char* data, int len, int streamID);                                                 
	    status_t        handleEndData(int streamID);                                                                   
	    status_t        handleStatus(int major_version, int minor_version, int code,char* reason_phrase, int streamID);
	    status_t        handleError(int id,char* description, int streamID);                                           
	    status_t        handleHeaders(long contentLength, int streamID);

        
            void            doFSCommand(char *command, char *args); 
            void            getInputText(char* initialText, int initLen, int maxNumChars, int textFlags);
            int64_t         convertLocalToUTCTime(int64_t localtime);
	    int64_t         convertUTCToLocalTime(int64_t utctime);
	    void            startVibrate(short unsigned int timeOn, short unsigned int timeOff, short unsigned int repeatCount);
	    void            stopVibrate();
	    int             getVolumeLevel();
	    int             getMaxVolumeLevel();
	    short unsigned int GetScreenOrientation();
	    char*           getDateTimeFormat();

    thread_id_t                 mLockThreadId;
    Mutex                       mLock;
    Mutex                       mNotifyLock;
    Condition                   mSignal;
    void*                       mCookie;
    media_player_states         mCurrentState;
    int                         mDuration;
    int                         mCurrentPosition;
    int                         mSeekPosition;
    bool                        mPrepareSync;
    status_t                    mPrepareStatus;
    int                         mStreamType;
    bool                        mLoop;
    float                       mLeftVolume;
    float                       mRightVolume;
    int                         mVideoWidth;
    int                         mVideoHeight;
    sp<ISurface>                mISurface;
    static  Mutex                           sServiceLock;
};
}
#endif

