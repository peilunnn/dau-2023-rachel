#include "stdafx.h"
#include "Managers/include/SoundManager.h"

SoundManager& SoundManager::GetInstance()
{
    static SoundManager instance;
    return instance;
}

void SoundManager::PlaySoundFromFile(const string& soundFilePath, bool loop)
{
    auto itBuffer = m_soundBuffers.find(soundFilePath);
    if (itBuffer == m_soundBuffers.end()) 
    {
        sf::SoundBuffer buffer;
    
        if (!buffer.loadFromFile(soundFilePath))
            return;
    
        m_soundBuffers[soundFilePath] = move(buffer);
    }

    // Create a sound and store it in the list to keep it alive
    m_sounds.emplace_back();
    auto it = prev(m_sounds.end());
    sf::Sound& sound = *it;
    sound.setBuffer(m_soundBuffers[soundFilePath]);
    sound.setLoop(loop);
    sound.play();

    m_soundMap[soundFilePath].push_back(it);
}

void SoundManager::StopSound(const string& soundFilePath)
{
    auto itBuffer = m_soundBuffers.find(soundFilePath);
    if (itBuffer != m_soundBuffers.end()) 
    {
        auto itSound = m_sounds.begin();
        while (itSound != m_sounds.end()) 
        {
            // If sound is using buffer, stop sound
            if (itSound->getBuffer() == &itBuffer->second) 
            {
                itSound->stop();
                itSound = m_sounds.erase(itSound);
            }
            else 
            {
                ++itSound;
            }
        }
    }
}

void SoundManager::PauseSound(const string& soundFilePath)
{
    auto& soundIters = m_soundMap[soundFilePath];
    for (auto& it : soundIters) 
    {
        if (it->getStatus() == sf::Sound::Playing)
            it->pause();
    }
}

void SoundManager::ResumeSound(const string& soundFilePath)
{
    auto& soundIters = m_soundMap[soundFilePath];
    for (auto& it : soundIters) 
    {
        if (it->getStatus() == sf::Sound::Paused)
            it->play();
    }
}

void SoundManager::Cleanup()
{
    for (auto it = m_sounds.begin(); it != m_sounds.end(); )
    {
        if (it->getStatus() == sf::Sound::Stopped)
            it = m_sounds.erase(it);
        else
            ++it;
    }
}

void SoundManager::ResetSounds()
{
    // Stop and remove all sounds
    for (auto& sound : m_sounds)
    {
        sound.stop();
    }
    m_sounds.clear();

    // Clear the sound buffers
    m_soundBuffers.clear();

    // Clear the sound map
    m_soundMap.clear();
}