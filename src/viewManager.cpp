#include "viewManager.h"
#include "SDL.h"

InputManager *ViewManager::inputManager = nullptr;

ViewManager::ViewManager()
{
    printf("Gamepad amount %i\n", SDL_NumJoysticks());
    int amount = SDL_NumJoysticks();
    for (int i = 0; i < amount; i++)
    {
        SDL_Joystick *gGameController = SDL_JoystickOpen(i);
        if (gGameController == NULL)
            printf("Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError());
        else
            gamePads.push_back(GamepadDevice({i, gGameController}));
    }
}

View *ViewManager::createViewFullscreen(std::string name)
{
    return createView(name, 0, 0, true);
}

View *ViewManager::createView(std::string name, int resX, int resY, bool isFullscreen)
{
    if (resX == 0)
        resX = getPrimaryScreenWidth() / (isFullscreen ? 1 : 2);
    if (resY == 0)
        resY = getPrimaryScreenHeight() / (isFullscreen ? 1 : 2);

    View *view = new View(resX, resY, isFullscreen);
    view->windowName = name;
    view->makeWindow();
    views.push_back(view);
    return view;
}

void ViewManager::processEvents()
{
    SDL_Event event;
    for (auto it = views.begin(); it != views.end(); it++)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                isExitIntended = true;
                break;

            case SDL_KEYDOWN:
                // printf("Key press detected %i\n", event.key.keysym.scancode);
                inputManager->provideInput(InputType::KEYBOARD, -1, event.key.keysym.scancode, 1.0f);
                break;

            case SDL_KEYUP:
                // printf("Key release detected %i\n", event.key.keysym.scancode);
                inputManager->provideInput(InputType::KEYBOARD, -1, event.key.keysym.scancode, 0.0f);
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button >= 1 && event.button.button <= 5)
                    inputManager->provideInput(InputType::MOUSE, -1, event.button.button, 1.0f);
                break;

            case SDL_MOUSEBUTTONUP:
                if (event.button.button >= 1 && event.button.button <= 5)
                    inputManager->provideInput(InputType::MOUSE, -1, event.button.button, 0.0f);
                break;

            case SDL_JOYDEVICEADDED:
            {
                SDL_Joystick *gGameController = SDL_JoystickOpen(event.jdevice.which);
                if (gGameController == NULL)
                    printf("Warning: Unable to open game controller!");
                else
                    gamePads.push_back(GamepadDevice({event.jdevice.which, gGameController}));
                break;
            }

            case SDL_JOYDEVICEREMOVED:
            {
                for (auto it = gamePads.begin(); it != gamePads.end(); it++)
                    if (it->index == event.jdevice.which)
                    {
                        SDL_JoystickClose((SDL_Joystick *)it->gamePad);
                        gamePads.erase(it);
                        break;
                    }
            }

            case SDL_JOYAXISMOTION:
            {
                float axisValue = float((int)event.jaxis.value) / float(32 * 1024);
                inputManager->provideInput(InputType::GAMEPAD_AXIS, event.jaxis.which, event.jaxis.axis, axisValue);
                break;
            }

            case SDL_JOYBUTTONDOWN:
                inputManager->provideInput(InputType::GAMEPAD_BUTTON, event.jbutton.which, event.jbutton.button, 1.0f);
                break;

            case SDL_JOYBUTTONUP:
                inputManager->provideInput(InputType::GAMEPAD_BUTTON, event.jbutton.which, event.jbutton.button, 0.0f);
                break;
            }
        }
    }
}

bool ViewManager::getIsExitIntended()
{
    return isExitIntended;
}

void ViewManager::setInputManager(InputManager *inputManager)
{
    ViewManager::inputManager = inputManager;
}

int ViewManager::getPrimaryScreenWidth()
{
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    return DM.w;
}

int ViewManager::getPrimaryScreenHeight()
{
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    return DM.h;
}
