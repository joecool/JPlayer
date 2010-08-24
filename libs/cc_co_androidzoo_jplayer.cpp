#define LOG_TAG  "Flicker/Core"

#include <stdio.h>
#include <string.h>
#include "jni.h"
#include <sys/socket.h>
#include <net/if.h>
//#include "netinet/In.h"
#include <assert.h>
//#include <android/log.h>
#include "utils/Log.h"


static int Java_cc_co_android_jplayer_register(JNIEnv* env)
{



    return 0;


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
