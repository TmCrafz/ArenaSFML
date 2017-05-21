#ifndef GAME_HPP
#define GAME_HPP
#include <SFML/Graphics.hpp>
#include <memory>
#include <chrono>
#include <map>
#include "Components/Warrior.hpp"
#include "Components/EnumWorldObjectTypes.hpp"
#include "Components/SceneNode.hpp"
#include "Input/QueueHelper.hpp"
#include "Input/InputHandler.hpp"
#include "Input/EnumInputTypes.hpp"
#include "Render/RenderManager.hpp"
#include "Resources/ResourceHolder.hpp"
#include "Resources/SpriteSheetMapHolder.hpp"
#include "Resources/LevelHolder.hpp"
#include "Screens/ScreenStack.hpp"
#include "Sound/MusicPlayer.hpp"
#include "libs/GUI-SFML/libs/tinyxml2.h"

typedef std::chrono::high_resolution_clock CLOCK;

class Screen;


class Game
{

    private:
        // Options
        unsigned int m_screenHeight;
        unsigned int m_screenWidth;
        bool m_showStats;
        bool m_isInDebug;
        // The area of the world which should be shown in the window
        // Is used to resize the view correctly depending on the window size.
        unsigned int m_referenceWorldWidth;
        unsigned int m_referenceWorldHeight;
        
        sf::RenderWindow m_window;
        //sf::View m_view;
        MusicPlayer m_music;
        sf::Text m_txtStatFPS;
        Screen::Context m_context;

        bool m_isRunning;
        bool m_isPaused;
        
        SceneNode m_sceneGraph;
        RenderManager m_renderManager;
        
        // Delta time
        float m_dt;
        float m_fps;
        CLOCK::time_point m_timePoint1;
        // Assets
        ResourceHolder<sf::Font> m_fontHolder;
        ResourceHolder<sf::Texture> m_textureHolder;
        ResourceHolder<sf::Shader> m_shaderHolder;
        SpriteSheetMapHolder m_spriteSheetMapHolder;
        LevelHolder m_levelHolder;

        // The game class handle all inputs which get later translated to commands
        QueueHelper<Input> m_inputQueue;
        InputHandler m_inputHandler;

        ScreenStack m_screenStack;
        // Actual shown screen
        //std::unique_ptr<Screen> m_actualScreen;
        //World m_world;

    private:
        void adjustShownWorldToWindowSize(unsigned int windowWidth, unsigned int windowHeight);
        void loadFonts();
        void loadTextures();
        void loadShaders();
        void loadLevels();
        void buildScene();
        
        void determineDeltaTime();
        void handleInput();
        void update();
        void render();

    public:
        Game(const bool showStats, const bool isInDebug, const bool isMusicOn);
        ~Game();
        void run();
};

#endif // GAME_HPP
