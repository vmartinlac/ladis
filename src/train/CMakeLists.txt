
add_executable(
    ladis_train
    Trainer.h
    Trainer.cpp
    NeuralNetwork.h
    NeuralNetwork.cpp
    TransitionList.h
    TransitionLoader.h
    TransitionLoader.cpp
    main.cpp)

target_link_libraries(ladis_train PUBLIC ladis_agents torch)

