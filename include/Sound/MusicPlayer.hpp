#ifndef MUSICPLAYER_HPP
#define MUSICPLAYER_HPP
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <string>
#include "Resources/EnumResourceIdentifiers.hpp"

class MusicPlayer : private sf::NonCopyable
{
    private:
        sf::Music m_music;
        // Identifer, filename
        std::map<std::string, std::string> m_fileNames;
        float m_volume;

    public:
        MusicPlayer();

        void play(const std::string &id);
        void stop();

        void setPaused(bool paused);
        void setVolume(float volume);
};

#endif // MUSICPLAYER_HPP
