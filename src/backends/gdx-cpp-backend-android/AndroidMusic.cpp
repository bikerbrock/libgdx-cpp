/*
 *  Copyright 2011 Aevum Software aevum @ aevumlab.com
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  @author Victor Vicente de Carvalho victor.carvalho@aevumlab.com
 *  @author Ozires Bortolon de Faria ozires@aevumlab.com
 *  @author aevum team
 */

#include "AndroidMusic.hpp"
#include <cassert>
#include <gdx-cpp/Gdx.hpp>

using namespace gdx_cpp::backends::android;


gdx_cpp::backends::android::AndroidMusic::AndroidMusic(JNIEnv* env, jobject musicObj)
    : env(env)
    , jniMusicObj(musicObj)
    , disposeJNI(NULL)
    , getPositionJNI(NULL)
    , isLoopingJNI(NULL)
    , isPlayingJNI(NULL)
    , pauseJNI(NULL)
    , playJNI(NULL)
    , setLoopingJNI(NULL)
    , setVolumeJNI(NULL)
    , stopJNI(NULL)
{ 
    jclass cls = env->GetObjectClass(jniMusicObj);

    disposeJNI = env->GetMethodID(cls, "dispose", "()V");
    playJNI = env->GetMethodID(cls, "play", "()V");
    stopJNI = env->GetMethodID(cls, "stop", "()V");
    isLoopingJNI = env->GetMethodID(cls, "isLooping", "()Z");
    isPlayingJNI = env->GetMethodID(cls, "isPlaying", "()Z");
    setLoopingJNI = env->GetMethodID(cls, "setLooping", "(Z)V");
    setVolumeJNI = env->GetMethodID(cls, "setVolume", "(F)V");
    getPositionJNI = env->GetMethodID(cls, "getPosition", "()F");
    pauseJNI = env->GetMethodID(cls, "pause", "()V");
    
    assert(disposeJNI);
    assert(getPositionJNI);
    assert(isLoopingJNI);
    assert(isPlayingJNI);
    assert(pauseJNI);
    assert(playJNI);
    assert(setLoopingJNI);
    assert(setVolumeJNI);
    assert(stopJNI);
}

void gdx_cpp::backends::android::AndroidMusic::dispose()
{
    env->CallVoidMethod(jniMusicObj, disposeJNI);
    env->DeleteGlobalRef(jniMusicObj);
}

float gdx_cpp::backends::android::AndroidMusic::getPosition()
{
    return env->CallFloatMethod(jniMusicObj, getPositionJNI);
}

bool gdx_cpp::backends::android::AndroidMusic::isLooping()
{
    return env->CallBooleanMethod(jniMusicObj, isLoopingJNI);
}

bool gdx_cpp::backends::android::AndroidMusic::isPlaying()
{
    return env->CallBooleanMethod(jniMusicObj, isPlayingJNI);
}

void gdx_cpp::backends::android::AndroidMusic::pause()
{
    env->CallVoidMethod(jniMusicObj, pauseJNI);
}

void gdx_cpp::backends::android::AndroidMusic::play()
{
    env->CallVoidMethod(jniMusicObj, playJNI);
}

void gdx_cpp::backends::android::AndroidMusic::setLooping(bool isLooping)
{
    env->CallVoidMethod(jniMusicObj, setLoopingJNI, isLooping);
}

void gdx_cpp::backends::android::AndroidMusic::setVolume(float volume)
{
    env->CallVoidMethod(jniMusicObj, setVolumeJNI, volume);
}

void gdx_cpp::backends::android::AndroidMusic::stop()
{
    env->CallVoidMethod(jniMusicObj, stopJNI);
}

gdx_cpp::backends::android::AndroidMusic::~AndroidMusic()
{
    dispose();
}



