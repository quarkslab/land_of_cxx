#include <iostream>
#include <string>

static
int usage(char const *progname) {
    std::cerr << "usage: " << progname << " <positive_number>" << std::endl;
    return 1;
}

int main(int argc, char * argv[]) {
    if(argc != 2) {
        return usage(argv[0]);
    }

    /* number of non-tie games */
    int nb_round = std::stoi(argv[1]);
    if(nb_round <= 0) {
        return usage(argv[0]);
    }

    /* main event loop */
    size_t your_score = 0,
           ai_score = 0;
    while(nb_round) {
        /* do some stuff */
        --nb_round;
    }
   std::cout << "Your score: " << your_score << std::endl
             << "AI score:   " << ai_score << std::endl;
    return 0;
}
