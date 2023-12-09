#pragma once
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <string>
using namespace std;

class SoundManager
{
public:
    static SoundManager& GetInstance();
    SoundManager(SoundManager const&) = delete;
    void operator=(SoundManager const&) = delete;

    void PlaySoundFromFile(const string& soundFilePath, bool loop = false);
    void StopSound(const string& soundFilePath);
    void Cleanup();

private:
    SoundManager() = default;
    unordered_map<string, sf::SoundBuffer> m_soundBuffers;
    list<sf::Sound> m_sounds;
};