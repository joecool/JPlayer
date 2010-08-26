#ifndef CC_CO_ANDROIDZOO_JPLAYER_PLAYER_H
#define CC_CO_ANDROIDZOO_JPLAYER_PLAYER_H

#include <binder/IMemory.h>
#include <ui/Surface.h>
#include <ui/ISurface.h>
#include "utils/Errors.h"  // for status_t
#include <binder/IInterface.h>
#include <utils/RefBase.h>
#include <binder/Parcel.h>
#include <media/MediaPlayerInterface.h>

namespace android {
class Test {
public:
    static const sp<ISurface>& getISurface(const sp<Surface>& s) {
        return s->getISurface();
    }
};
};


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
using android::MediaPlayer;
using android::Test;

class Player:public MediaPlayerInterface
{

public:
    Player();
    ~Player();
    virtual status_t    initCheck();
    //        void            onFirstRef();
    virtual status_t        setDataSource(const char *url);
    virtual status_t        setDataSource(int fd, int64_t offset, int64_t length);
    //        status_t        setVideoSurface(const sp<Surface>& surface);
    virtual status_t    setVideoSurface(const sp<ISurface>& surface);
    virtual status_t        prepare();
    virtual status_t        prepareAsync();
    virtual status_t        start();
    virtual status_t        stop();
    virtual status_t        pause();
    //        status_t        flush();
    virtual bool            isPlaying();
    //        status_t        getVideoWidth(int *w);
    //        status_t        getVideoHeight(int *h);
    virtual status_t        seekTo(int msec);
    vitrual status_t        getCurrentPosition(int *msec);
    vitrual status_t        getDuration(int *msec);
    virtual status_t        reset();
    //        status_t        setAudioStreamType(int type);
    virtual status_t        setLooping(int loop);
    //        bool            isLooping();
    //        status_t        setVolume(float leftVolume, float rightVolume);
    //        void            notify(int msg, int ext1, int ext2);
    //static  int             snoop(short *data, int len, int kind);

    //        status_t        handleKeyEvent(int keyCode, int action);
	//    status_t        handleTouchEvent(int x, int y, int action);
	//    status_t        setInputText(char *pInputStr, int inputLen);
    virtual status_t    invoke(const Parcel& request, Parcel *reply) {return 0;}
    virtual player_type playerType() { return TEST_PLAYER; }

        
    //        void            doFSCommand(char *command, char *args); 
    //        void            getInputText(char* initialText, int initLen, int maxNumChars, int textFlags);
    //        int64_t         convertLocalToUTCTime(int64_t localtime);
	//    int64_t         convertUTCToLocalTime(int64_t utctime);
	//    void            startVibrate(short unsigned int timeOn, short unsigned int timeOff, short unsigned int repeatCount);
	//    void            stopVibrate();
	//    int             getVolumeLevel();
	//    int             getMaxVolumeLevel();
	//    short unsigned int GetScreenOrientation();
	//    char*           getDateTimeFormat();

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

