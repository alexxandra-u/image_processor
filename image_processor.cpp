#include "application.h"
#include <iostream>

int main(int argc, char* argv[]) {
    try {
        Application app;
        app.Run(argc, argv);
    } catch (ImageException& e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "unidentified mistake happened" << std::endl;
    }
    return 0;
}

