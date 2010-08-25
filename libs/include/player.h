#ifndef CC_CO_ANDROIDZOO_JPLAYER_PLAYER_H
#define CC_CO_ANDROIDZOO_JPLAYER_PLAYER_H

#include <binder/IMemory.h>
#include <ui/Surface.h>
#include "utils/Errors.h"  // for status_t
#include <binder/IInterface.h>
#include <utils/RefBase.h>
#include <binder/Parcel.h>
#include <media/MediaPlayerInterface.h>

namespace androidzoo {
using android::Surface;
using android::ISurface;
using android::sp;
using android::status_t;
using android::IInterface;
using android::MediaPlayerInterface;
using android::player_type;
using android::TEST_PLAYER;
using android::Parcel;

class Player:public MediaPlayerInterface
{

public:
    Player();
    ~Player();
    virtual status_t    initCheck();
            void            onFirstRef();
            status_t        setDataSource(const char *url);
            status_t        setDataSource(int fd, int64_t offset, int64_t length);
            status_t        setVideoSurface(const sp<Surface>& surface);
    virtual status_t    setVideoSurface(const sp<ISurface>& surface);
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
    static  int             snoop(short *data, int len, int kind);

            status_t        handleKeyEvent(int keyCode, int action);
	    status_t        handleTouchEvent(int x, int y, int action);
	    status_t        setInputText(char *pInputStr, int inputLen);
    virtual status_t    invoke(const Parcel& request, Parcel *reply) {return 0;}
    virtual player_type playerType() { return TEST_PLAYER; }

        
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

    void*                       mCookie;
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

};
}
#endif

