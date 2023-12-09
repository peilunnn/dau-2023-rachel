#include "stdafx.h"
#include "Managers/include/SoundManager.h"

SoundManager& SoundManager::GetInstance()
{
    static SoundManager instance;
    return instance;
}

void SoundManager::PlaySoundFromFile(const std::string& soundFilePath, bool loop)
{
    auto itBuffer = m_soundBuffers.find(soundFilePath);
    if (itBuffer == m_soundBuffers.end()) {
        sf::SoundBuffer buffer;
        if (!buffer.loadFromFile(soundFilePath)) {
            return;
        }
        m_soundBuffers[soundFilePath] = std::move(buffer);
    }

    // Create a sound and store it in the list to keep it alive
    m_sounds.emplace_back();
    sf::Sound& sound = m_sounds.back();
    sound.setBuffer(m_soundBuffers[soundFilePath]);
    sound.setLoop(loop);
    sound.play();
}

void SoundManager::StopSound(const std::string& soundFilePath)
{
    auto itBuffer = m_soundBuffers.find(soundFilePath);
    if (itBuffer != m_soundBuffers.end()) {
        auto itSound = m_sounds.begin();
        while (itSound != m_sounds.end()) {
            // If sound is using buffer, stop sound
            if (itSound->getBuffer() == &itBuffer->second) {
                itSound->stop();
                itSound = m_sounds.erase(itSound);
            }
            else {
                ++itSound;
            }
        }
    }
}

void SoundManager::Cleanup()
{
    for (auto it = m_sounds.begin(); it != m_sounds.end(); ) {
        if (it->getStatus() == sf::Sound::Stopped) {
            it = m_sounds.erase(it);
        }
        else {
            ++it;
        }
    }
}
