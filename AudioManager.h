#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <map>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

class AudioManager
{
public:

    ~AudioManager();
    static AudioManager* getInstance();

    void playMusic(string fileName);
    void pauseMusic();
    void stopMusic();
    void playFX(string fileName);
    Mix_Chunk* GetChunkPlay(string fileName);

    inline bool isPaused() const
    {
        return currentState == PAUSED;
    }

    inline bool isStopped() const
    {
        return currentState == STOPPED;
    }

    inline bool isPlaying() const
    {
        return currentState == PLAYING;
    }

    inline bool inErrorState() const
    {
        return currentState == ERROR;
    }

protected:
    AudioManager();

    AudioManager(const AudioManager&)
    {
    }

    AudioManager& operator=(const AudioManager&)
    {
    }

private:
    static AudioManager* pInstance;

    enum AudioState
    {
        ERROR = 0,
        WAITING,
        PAUSED,
        STOPPED,
        PLAYING
    };
    static AudioState currentState;

    map<string, Mix_Chunk*>	AudioMap;
};
#endif
