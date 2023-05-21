#pragma once
#include "os/input.h"

class Pawn
{
public:
    EXPORT Pawn();
    EXPORT ~Pawn();

    EXPORT void removeAllInputs();
    EXPORT void addInput(Input<Pawn> *input);

    template <class T, typename std::enable_if<std::is_base_of<Pawn, T>::value>::type * = nullptr>
    EXPORT Input<Pawn> *registerAxisCallback(void *owner, void (T::*callback)(InputType, int, int, float))
    {
        Input<Pawn> *input = new Input<Pawn>(owner, (void(Pawn::*)(InputType, int, int, float))callback, nullptr);
        addInput(input);
        return input;
    }

    template <class T, typename std::enable_if<std::is_base_of<Pawn, T>::value>::type * = nullptr>
    EXPORT Input<Pawn> *registerButtonCallback(void *owner, void (T::*callback)(InputType, int, int, bool))
    {
        Input<Pawn> *input = new Input<Pawn>(owner, nullptr, (void(Pawn::*)(InputType, int, int, bool))callback);
        addInput(input);
        return input;
    }

    static std::vector<Input<Pawn> *> *getInputs()
    {
        return &inputs;
    }

protected:
    static std::vector<Input<Pawn> *> inputs;
};
