#ifndef ENUMINPUTTYPES_HPP
#define ENUMINPUTTYPES_HPP

/* Input Types get later ranslated to commands.
 * One Input can generate several commands.
 * How a InputType come about is platform depentend.
 * (Various platforms has different input options
 */

enum class InputTypes
{
    WINDOW_RESIZED,
    LOST_FOCUS,
    GAINED_FOCUS,
    CURSOR_POS,
    CURSOR_RIGHT_POS,
    CURSOR_LEFT_POS,
    TRANSLATED_CURSOR_POS,
    LEFT,
    RIGHT,
    UP,
    DOWN,
    UP_LEFT,
    UP_RIGHT,
    DOWN_LEFT,
    DOWN_RIGHT,
    LEFT_CLICK,
    LCONTROL_LEFT_CLICK,
    RIGHT_CLICK,
    RIGHT_CLICK_START,
    RIGHT_CLICK_STILL,
    RIGHT_CLICK_STOPED,
    PAUSE,
    BACK,
    CONSOLE,
    
    // Debug
    D1,
    D2,
    D3,
    D4,



    // TMP
    LEFT_A,
    RIGHT_A,
    UP_A,
    DOWN_A
};

#endif // ENUMINPUTTYPES_HPP
