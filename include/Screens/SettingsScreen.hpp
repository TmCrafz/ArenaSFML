#ifndef SETTINGSSCREEN_HPP
#define SETTINGSSCREEN_HPP
#include "libs/GUI-SFML/include/GUI-SFML.hpp"
#include "Screens/Screen.hpp"
#include <SFML/Graphics.hpp>

class SettingsScreen : public Screen
{
    private:
        gsf::GUIEnvironment m_guiEnvironment;
        ConfigManager *m_config;
        bool m_settingChanged;
        gsf::CheckBoxWidget *m_checkBoxSound;
        gsf::CheckBoxWidget *m_checkBoxFullscreen;
        gsf::CheckBoxWidget *m_checkBoxFramelimit;
        gsf::CheckBoxWidget *m_checkBoxVsync;
        gsf::ComboBoxWidget *m_comboBoxResolution;
    
    public:
        SettingsScreen(ScreenStack *screenStack, Context &context);

        virtual void buildScene();
        
        virtual void windowSizeChanged();

        virtual bool handleInput(Input &input, float dt) override;
        virtual bool handleEvent(sf::Event &event, float dt) override;

        virtual bool update(float dt);

        virtual void render();

    private:
        void loadSettings();
};

#endif // !SETTINGSSCREEN_HPP