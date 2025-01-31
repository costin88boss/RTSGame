#include "Game.h"
#include <spdlog/spdlog.h>

int main() {
    // Constructor takes care of everything
    // No reason to have a start func

    spdlog::error("Error {}", 69, 2);

    char* c = nullptr;
    
   char z = *c;

    if (z == 3) {
     
    }

    int v;
    double d = 3.14;
    int i = 3 + d;

   try {

        
        //debug::init();



        while (1)
            ;

        /*
        debug::setLogLevel(debug::LogLevel::ALL);

        debug::log(debug::LogLevel::NONE, "ABC abc %d %c", -47, 'a');
        debug::log(debug::LogLevel::INFO, "ABC abc %d %c", -48, 'b');
        debug::log(debug::LogLevel::WARNING, "ABC abc %d %c", -49, 'c');
        debug::log(debug::LogLevel::ERR, "ABC abc %d %c", -50, 'd');
        debug::log(debug::LogLevel::FATAL, "ABC abc %d %c", -51, 'e');
        */
        //game = std::make_unique<Game>();
    } catch (const std::exception& e) {
       return 1;
    }
}
