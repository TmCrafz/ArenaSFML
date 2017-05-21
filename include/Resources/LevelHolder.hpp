#ifndef LEVELHOLDER_HPP
#define LEVELHOLDER_HPP
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <map>
#include "Level/Level.hpp"

class LevelHolder
{
    
    private:
        struct Settings
        {
            std::string levelName;
            int tileWidth;
            int tileHeight;
        };

        std::vector<std::unique_ptr<Level>> m_levels;
    public:
        void load(const std::string &fileName);

        std::vector<std::unique_ptr<Level>>& getLevels();

    private:
        bool loadSettings(const std::string &line, Settings *settings);
        bool loadTileAliases(const std::string &line, 
                std::map<std::string, std::string> *tileAliases);
        bool loadMap(const std::string &line, const Settings &settings, 
                Level *level, const std::map<std::string, std::string> &tileAliases,
                int currentRow);
        bool loadObjects(const std::string &line, const Settings &settings, 
                Level *level, int currentRow);
        
        // Translate the given row and column to a position, depending on the given
        // tile width and height
        sf::Vector2f translateRowColumnToPosition(int column, int row,
                int tileWidth, int tileHeight);

};


#endif // !LEVELHOLDER_HPP
