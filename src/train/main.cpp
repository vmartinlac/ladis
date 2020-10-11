#include <iostream>
#include "Trainer.h"
#include "TransitionLoader.h"

int main(int num_args, char** args)
{
    TransitionList transitions;
    TransitionLoader::load(transitions);

    Trainer trainer;
    trainer.train(transitions);

    return 0;
}

