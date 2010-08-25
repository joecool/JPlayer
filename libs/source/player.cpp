#define LOG_TAG "MediaPlayer"
#include <utils/Log.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <binder/MemoryHeapBase.h>
#include "player.h"

namespace androidzoo{

using android::MemoryHeapBase;
using android::NO_ERROR;
using android::NO_MEMORY;
using android::PIXEL_FORMAT_RGB_565;
using namespace android;

Player::Player()
{
    LOGV("constructor");
    mCookie = NULL;
    mDuration = -1;
    mCurrentPosition = -1;
    mSeekPosition = -1;
    mPrepareSync = false;
    mLoop = false;
    mLeftVolume = mRightVolume = 1.0;
    mVideoWidth = mVideoHeight = 0;
	mISurface = NULL;
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
       return NO_MEMORY;
    }
    ISurface::BufferHeap buffer(852, 480, 852, 480, PIXEL_FORMAT_RGB_565, mFrameHeap);
    mISurface->registerBuffers(buffer); 
#define USE_SURFACE_BUFFER
#ifdef USE_SURFACE_BUFFER
    unsigned char *pFrameBuf = (unsigned char*)mFrameHeap->base();
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
	if(NULL != surface)
    {
		mISurface = surface->getISurface();
	}
	return NO_ERROR;
}

status_t Player::setVideoSurface(const sp<ISurface>& surface)
{
    LOGV("setVideoSurface(%p)", surface.get());
    mISurface = surface;
    return NO_ERROR;
}

status_t Player::initCheck()
{
    return NO_ERROR;
}


}
