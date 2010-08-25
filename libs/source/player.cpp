#define LOG_TAG "MediaPlayer"
#include <utils/Log.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <binder/IServiceManager.h>
#include <binder/IPCThreadState.h>

#include <media/mediaplayer.h>
#include <media/AudioTrack.h>

#include <binder/MemoryBase.h>

Player::Player()
{
    LOGV("constructor");
    mCookie = NULL;
    mDuration = -1;
    mStreamType = AudioSystem::MUSIC;
    mCurrentPosition = -1;
    mSeekPosition = -1;
    mCurrentState = MEDIA_PLAYER_IDLE;
    mPrepareSync = false;
    mPrepareStatus = NO_ERROR;
    mLoop = false;
    mLeftVolume = mRightVolume = 1.0;
    mVideoWidth = mVideoHeight = 0;
    mLockThreadId = 0;
	mSurface = NULL;
}

Player::~Player()
{
    LOGV("destructor");
}

status_t Player::start()
{
    LOGV("start\n");
    MemoryHeapBase* mFrameHeap = new MemoryHeapBase(852 * 480 * 2);
    if (mFrameHeap->heapID() < 0) {
       LOGE("Error creating frame buffer heap");
       delete mFrameHeap;
       return ERROR_ALLOCATE_FAILED;
    }
    ISurface::BufferHeap buffer(852, 480, 852, 480, PIXEL_FORMAT_RGB_565, mFrameHeap);
    mISurface->registerBuffers(buffer);          
#ifdef 1
    pFrameBuf = (unsigned char*)mFrameHeap->base();
#else
    pSurfaceFrameBuf = (unsigned char*)mFrameHeap->base();
    pFrameBuf = (unsigned char *)malloc(852 * 480 * 2);
#endif    

    /*test*/
    memset(pFrameBuf, 0, 852 * 480 * 2);
    mISurface->postBuffer(0);
}

status_t Player::setVideoSurface(const sp<Surface>& surface)
{
    LOGV("setVideoSurface");
	if(null != surface)
    {
		mISurface = surface->getISurface();
	}
}



