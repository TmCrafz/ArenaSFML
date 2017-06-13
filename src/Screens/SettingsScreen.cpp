#include "Screens/SettingsScreen.hpp"
#include "Screens/ScreenStack.hpp"
#include "Helpers.hpp"

SettingsScreen::SettingsScreen(ScreenStack *screenStack, Context &context)
: Screen(screenStack, context)
, m_guiEnvironment{ *context.window }
, m_config{ m_context.config }
, m_settingChanged{ false }
, m_checkBoxSound{ nullptr }
, m_checkBoxFullscreen{ nullptr }
, m_checkBoxFramelimit{ nullptr }
, m_checkBoxVsync{ nullptr }
, m_comboBoxResolution{ nullptr }
{
    buildScene();
}

void SettingsScreen::buildScene()
{
    m_screenStack->pushScreen(ScreenID::GAME);
    sf::View oldView{ m_context.window->getView() };
    m_context.window->setView(m_context.guiView);
    m_guiEnvironment.createScene("assets/gui/settings_menu.xml");
    m_context.window->setView(oldView);
    
    m_checkBoxSound = static_cast<gsf::CheckBoxWidget*>(
        m_guiEnvironment.getWidgetByID("checkBoxWidget_sound" ));
    m_checkBoxFullscreen = static_cast<gsf::CheckBoxWidget*>(
        m_guiEnvironment.getWidgetByID("checkBoxWidget_fullscreen" ));
    m_checkBoxFramelimit = static_cast<gsf::CheckBoxWidget*>(
        m_guiEnvironment.getWidgetByID("checkBoxWidget_framelimit" ));
    m_checkBoxVsync = static_cast<gsf::CheckBoxWidget*>(
        m_guiEnvironment.getWidgetByID("checkBoxWidget_vsync" ));
    m_comboBoxResolution = static_cast<gsf::ComboBoxWidget*>(
        m_guiEnvironment.getWidgetByID("comboBoxWidget_resolution" ));
    m_comboBoxInputP1 = static_cast<gsf::ComboBoxWidget*>(
        m_guiEnvironment.getWidgetByID("comboBoxWidget_inputPlayer1" ));
    m_comboBoxInputP2 = static_cast<gsf::ComboBoxWidget*>(
        m_guiEnvironment.getWidgetByID("comboBoxWidget_inputPlayer2" ));

    loadSettings();
    /*
    std::function<void(gsf::Widget *widget, sf::Vector2f pos)> {
        [this](gsf::Widget *widget, sf::Vector2f pos)
    {
        m_settingChanged = true;
    } };
    */
    m_checkBoxSound->setOnLeftClickListener(
            [this](gsf::Widget *widget, sf::Vector2f pos)
    {
        m_settingChanged = true;
    });
    m_checkBoxFullscreen->setOnLeftClickListener(
            [this](gsf::Widget *widget, sf::Vector2f pos)
    {
        m_settingChanged = true;
    });
    m_checkBoxFramelimit->setOnLeftClickListener(
            [this](gsf::Widget *widget, sf::Vector2f pos)
    {
        m_settingChanged = true;
    });
    m_checkBoxVsync->setOnLeftClickListener(
            [this](gsf::Widget *widget, sf::Vector2f pos)
    {
        m_settingChanged = true;
    });
    m_comboBoxResolution->setOnSelectionChangedListener(
            [this](gsf::Widget *widget, int index)
    {
        m_settingChanged = true;     
    });
    m_comboBoxInputP1->setOnSelectionChangedListener(
            [this](gsf::Widget *widget, int index)
    {
        m_settingChanged = true;     
    });
    m_comboBoxInputP2->setOnSelectionChangedListener(
            [this](gsf::Widget *widget, int index)
    {
        m_settingChanged = true;     
    });


    gsf::TextButtonWidget* backBtn{ static_cast<gsf::TextButtonWidget*>(
            m_guiEnvironment.getWidgetByID("textButtonWidget_back")) };
    backBtn->setOnLeftClickListener(
            [this](gsf::Widget *widget, sf::Vector2f pos)
    {
        m_screenStack->popScreen();
        m_screenStack->pushScreen(ScreenID::MAINMENU);
    });
    gsf::TextButtonWidget* saveBtn{ static_cast<gsf::TextButtonWidget*>(
            m_guiEnvironment.getWidgetByID("textButtonWidget_save")) };
    saveBtn->setOnLeftClickListener(
            [this](gsf::Widget *widget, sf::Vector2f pos)
    {
        if (m_settingChanged)
        {
            std::cout << "Changed \n";
            m_config->set("music_on", m_checkBoxSound->isChecked());
            m_config->set("fullscreen", m_checkBoxFullscreen->isChecked());
            m_config->set("framerate_limit", m_checkBoxFramelimit->isChecked());
            m_config->set("vertical_sync", m_checkBoxVsync->isChecked());
            
            std::string resStr{ 
                m_comboBoxResolution->currentText().toAnsiString() };
            std::vector<std::string> res{
                Helpers::splitString(resStr, 'x') };
            if (res.size() >= 2)
            {
                std::string screenWidth{ res[0] };
                std::string screenHeight{ res[1] };
                m_config->set("screen_width", screenWidth);
                m_config->set("screen_height", screenHeight);
            };
            m_config->set("input_player1", getInputEntry(m_comboBoxInputP1));
            m_config->set("input_player2", getInputEntry(m_comboBoxInputP2));
            m_config->saveCurrentConfigToFile();
        }
        m_screenStack->popScreen();
        m_screenStack->pushScreen(ScreenID::MAINMENU);
    });
}

void SettingsScreen::loadSettings()
{
    m_checkBoxSound->setIsChecked(m_config->getBool("music_on", true));
    m_checkBoxFullscreen->setIsChecked(m_config->getBool("fullscreen", true));
    m_checkBoxFramelimit->setIsChecked(m_config->getBool("framerate_limit", true));
    m_checkBoxVsync->setIsChecked(m_config->getBool("vertical_sync", true));
    std::string screenWidth{ m_config->getString("screen_width", "1024") };
    std::string screenHeight{ m_config->getString("screen_height", "768") };
    std::string res{ screenWidth + "x" + screenHeight };
    if(!m_comboBoxResolution->selectElement(res))
    {
        // Resolution is not in given entries, so add the custom resolution to
        // combobox
        m_comboBoxResolution->addElement(res);
        m_comboBoxResolution->selectElement(res);
    }
    std::string inputP1{ m_config->getString("input_player1", "keyboard_mouse") };
    std::string inputP2{ m_config->getString("input_player2", "joystick_0") };
    selectInputEntry(m_comboBoxInputP1, inputP1);    
    selectInputEntry(m_comboBoxInputP2, inputP2);    
}

void SettingsScreen::selectInputEntry(
        gsf::ComboBoxWidget *widget, const std::string &value)
{
    int select{ 0 };
    if (value == "keyboard_mouse")
    {
        select = 0;
    }
    else if (value == "joystick_0")
    {
        select = 1;
    }
    else if (value == "joystick_1")
    {
        select = 2;
    }
    widget->selectElement(select);
}

std::string SettingsScreen::getInputEntry(gsf::ComboBoxWidget *widget) const
{
    std::string input{ "keyboard_mouse" };
    int currentSelection{ widget->currentIndex() };
    switch(currentSelection)
    {
        case 0: input = "keyboard_mouse"; break;
        case 1: input = "joystick_0"; break;
        case 2: input = "joystick_1"; break;
    }
    return input;
}

void SettingsScreen::windowSizeChanged()
{
    sf::View oldView{ m_context.window->getView() };
    m_context.window->setView(m_context.guiView);
    m_guiEnvironment.replaceWidgets();
    m_context.window->setView(oldView);
}

bool SettingsScreen::handleInput(Input &input, float dt)
{
    switch (input.getInputType())
    {
        case InputTypes::BACK :
        {
            m_screenStack->popScreen();
            m_screenStack->pushScreen(ScreenID::MAINMENU);
            return false;
            break;
        }
        default:
            break;
    }
    return false;
}

bool SettingsScreen::handleEvent(sf::Event &event, float dt)
{
    sf::View oldView{ m_context.window->getView() };
    m_context.window->setView(m_context.guiView);
    m_guiEnvironment.handleEvent(event);
    m_context.window->setView(oldView);
    return false;
}

bool SettingsScreen::update(float dt)
{
    sf::View oldView{ m_context.window->getView() };
    m_context.window->setView(m_context.guiView);
    m_guiEnvironment.update(dt);
    m_context.window->setView(oldView);
    return false;
}

void SettingsScreen::render()
{
    sf::View oldView{ m_context.window->getView() };
    m_context.window->setView(m_context.guiView);
    m_context.window->draw(*m_context.background);
    m_context.window->draw(m_guiEnvironment);
    m_context.window->setView(oldView);
}

