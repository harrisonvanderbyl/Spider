#ifndef DISPLAY_H
#define DISPLAY_H
#include <SDL2/SDL.h>
#include <string>

class Display
{
public:
    Display(int width, int height,const std::string& title);
    void SwapBuffers();
    bool is_closed();
    void Clear(float r, float g, float b, float a);
    virtual ~Display();
    inline int* getkeys()
    {
        return m_movement;
    }
    SDL_Window* m_window;
protected:

private:

    enum
    {
        UP,
        DOWN,
        LEFT,
        RIGHT,
        MOUSEX,
        MOUSEY,
        WINDOWX,
        WINDOWY,
        WINDOWW,
        WINDOWH,
        MOUSED,
        JUMP,
        PLACE,
        ROTX,
        ROTY,
        ROTZ,
        DISCARD,
        BONEPLACE,
        MOUSERIGHT,
        EXTEND,
        ATTACH,

        NUM_COMMANDS
    };
    int m_movement[NUM_COMMANDS] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


    Display(const Display& other) {}
    void operator=(const Display& other) {}

    SDL_GLContext m_glcontext;
    bool m_isclosed;
};

#endif // DISPLAY_H
