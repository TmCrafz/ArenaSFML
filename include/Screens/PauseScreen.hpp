#ifndef PAUSESCREEN_HPP
#define PAUSESCREEN_HPP
#include "Screens/Screen.hpp"
#include <SFML/Graphics.hpp>

class PauseScreen : public Screen
{
    private:

    public:
        PauseScreen(ScreenStack *screenStack, Context context);

        virtual ~PauseScreen();

        virtual void buildScene();

        virtual void handleInput(Input input, float dt);

        virtual void update(float dt);

        virtual void render();
};

#endif // PAUSESCREEN_HPP
