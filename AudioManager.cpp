#include "AudioManager.h"
#include "SDL_mixer.h"

AudioManager* AudioManager::pInstance = NULL;
AudioManager::AudioState AudioManager::currentState = ERROR;

AudioManager::~AudioManager()
{
    Mix_CloseAudio();
}

AudioManager* AudioManager::getInstance()
{
    if (pInstance == NULL) {
        pInstance = new AudioManager();
    }


    return pInstance;
}

void AudioManager::playMusic(string fileName)
{
    if (currentState != ERROR)
    {
        // If no music is playing, play it
        if (Mix_PlayingMusic() == 0)
        {
            // Load music
            Mix_Music* music = Mix_LoadMUS(fileName.c_str());

            if (music == NULL)
            {
                // Display a debug error, but remain in the state we were in
                // since nothing has changed.
                cerr << "Error loading music file: " << fileName << endl;
            }
            else
            {
                //Play music
                Mix_PlayMusic(music, -1);
                currentState = PLAYING;
            }
        }
        else
        {
            // If music is playing, pause it
            this->pauseMusic();
        }
    }
}

void AudioManager::pauseMusic()
{
    if (currentState != ERROR)
    {
        // If music is playing, handle the pause request
        if (Mix_PlayingMusic() == 1)
        {
            if (Mix_PausedMusic() == 1)
            {
                // If we receive a pause request and the music is already paused, resume it.
                Mix_ResumeMusic();
                currentState = PLAYING;
            }
            else
            {
                // Otherwise, pause the music
                Mix_PauseMusic();
                currentState = PAUSED;
            }
        }
    }
}

void AudioManager::stopMusic()
{
    if (currentState != ERROR)
    {
        Mix_HaltMusic();
        currentState = STOPPED;
    }
}

void AudioManager::playFX(string fileName)
{
    //TODO: Memory leaks, MixChunk<>
    //SOLUTION:
    /*
        Tindre un vector de MixChunks
        Al cridar un nou fx, comprovar si ja existeix un al vector amb aquell audio
        Si es que si, carrego el existent, sino el creo
    */
    if (currentState != ERROR)
    {
        bool isRepeated = false;

            map<string, Mix_Chunk*>::iterator it;
            for (it = AudioMap.begin(); it != AudioMap.end(); it++)
            {
                if (fileName == it->first) {
                    isRepeated = true;
                    break;
                }
            }

            if (isRepeated == false) {
                AudioMap.insert(pair<string, Mix_Chunk*>(fileName, Mix_LoadWAV(fileName.c_str())));
            }

            Mix_PlayChannel(-1, GetChunkPlay(fileName), 0);
    }
}

Mix_Chunk* AudioManager::GetChunkPlay(string fileName) {
    map<string, Mix_Chunk*>::iterator it;
    for (it = AudioMap.begin(); it != AudioMap.end(); it++)
    {
        if (fileName == it->first) {
            return it->second;
        }
    }
    return 0;
}

AudioManager::AudioManager()
{
    if (SDL_Init(SDL_INIT_AUDIO) != -1)
    {
        if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
        {
            cerr << "Error initializing audio device...\n";
            currentState = ERROR;
        }
        else
        {
            currentState = WAITING;
            AudioMap.clear();
            SDL_InitSubSystem(SDL_INIT_AUDIO);
            Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
            Mix_AllocateChannels(128);
        }
    }
    else
    {
        cerr << "Error initializing SDL audio subsystem...\n";
        currentState = ERROR;
    }
}