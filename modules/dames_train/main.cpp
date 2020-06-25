#include <iostream>
#include <typeinfo>
#include "ActionDataset.h"

int main(int num_args, char** args)
{
    const QString root_directory = "/home/victor/Dépotoire/aa";

    ActionDataset dataset;
    dataset.load(root_directory);

    return 0;
}

