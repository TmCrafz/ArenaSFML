#ifndef ENUMINPUTTYPES_HPP
#define ENUMINPUTTYPES_HPP

/* Input Types get later ranslated to commands.
 * One Input can generate several commands.
 * How a InputType come about is platform depentend.
 * (Various platforms has different input options
 */

enum class InputTypes
{
    CURSOR_POS,
    TRANSLATED_CURSOR_POS,
    LEFT,
    RIGHT,
    UP,
    DOWN,
    UP_LEFT,
    UP_RIGHT,
    DOWN_LEFT,
    DOWN_RIGHT,
    // Debug
    D1,
    D2,
    D3,
    D4,
    LEFT_CLICK,
    RIGHT_CLICK,
    // TMP
    LEFT_A,
    RIGHT_A,
    UP_A,
    DOWN_A
};

#endif // ENUMINPUTTYPES_HPP
