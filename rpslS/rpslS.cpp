/* .~= rock-paper-scissors-lizard-Spock =~.
 *
 * This is a famous (and fair) version the classical rock-paper-scissors game.
 *
 * The winning table is the following (wow, so 80's)
 *
 *              Rock  Paper Scissors  Lizard  Spock
 *    Rock      D     L     W         W       L
 *    Paper     W     D     L         L       W
 *    Scissors  L     W     D         W       L
 *    Lizard    L     W     L         D       W
 *    Spock     W     L     W         L       D
 *
 *    D=Draw W=Win L=Loose
 *
 * The goal is to implement an interactive version of this game. The winner (the player or the AI) wins once she wins a given number of round. 
 *
 * Here is a sample game:
 *
 * >>> ./rpslS 3
 * (r)ock-(p)aper-(s)cissors-(l)izard-(S)pock? r
 * AI: rock
 * Your score: 0
 * AI score:   0
 * (r)ock-(p)aper-(s)cissors-(l)izard-(S)pock? r
 * AI: rock
 * Your score: 0
 * AI score:   0
 * (r)ock-(p)aper-(s)cissors-(l)izard-(S)pock? r
 * AI: paper
 * Your score: 0
 * AI score:   1
 * (r)ock-(p)aper-(s)cissors-(l)izard-(S)pock? r
 * AI: rock
 * Your score: 0
 * AI score:   1
 * (r)ock-(p)aper-(s)cissors-(l)izard-(S)pock? r
 * AI: lizard
 * Your score: 1
 * AI score:   1
 * (r)ock-(p)aper-(s)cissors-(l)izard-(S)pock? r
 * AI: lizard
 * Your score: 2
 * AI score:   1
 *
 * to complete this exercise, you can use:
 *
 * - method calls
 * - try / catch block
 * - throw keyword
 * - std::stoi(char const*)
 * - constexpr keyword
 *
 */

#include <iostream>
#include <string>
#include <array>
#include <random>
#include <cassert>

enum class Weapon {
    ROCK,
    PAPER,
    SCISSOR,
    LIZARD,
    SPOCK
};

/* make a Weapon from a character read on the standard input
 */
std::istream& operator>>(std::istream& is, Weapon& w) {
    // FIXME: set `w' to the right value depending on the content of `c'
    w = Weapon::ROCK; // rock is the strongest weapon anyway ^_-
    // TODO: handle incorrect values too
    return is;
}
std::ostream& operator<<(std::ostream& os, Weapon w) {
    // FIXME: prints a string representation of the weapon
    return os;
}

enum class Status {
    LOSE,
    DRAW,
    WIN
};

// TODO: make sure no one will ever modify this data
Status Matrix[][5] = {
    /* Rock */      { Status::DRAW, Status::LOSE, Status::WIN, Status::WIN, Status::LOSE },
    /* Paper */     { Status::WIN, Status::DRAW, Status::LOSE, Status::LOSE, Status::WIN },
    /* Scissors */  { Status::LOSE, Status::WIN, Status::DRAW, Status::WIN, Status::LOSE },
    /* Lizard */    { Status::LOSE, Status::WIN, Status::LOSE, Status::DRAW, Status::WIN },
    /* Spock */     { Status::WIN, Status::LOSE, Status::WIN, Status::LOSE, Status::DRAW }
};

class AI {
    std::default_random_engine rengine_;
    std::uniform_int_distribution<int> uniform_dist_;

    public:

    AI() :
        rengine_(std::random_device()()),
        uniform_dist_(0,4) // TODO: do not use a hard-coded constant
    {
    }

    Weapon weapon() {
      // FIXME: choose a random value
      return Weapon::PAPER;
    }
};

/* prints some information concerning the usage of this (great) program and
 * return an error code
 */
// FIXME: accept a string argument that contains the name of the program
static
int usage() {
    // TODO: prints a more informative string to the standard error, that includes the name of the program
    std::cout << "Take this K. Rool, is ain't cool" << std::endl;
    return 1;
}

int main(int argc, char * argv[]) {
    if(argc != 2) {
        return usage();
    }

    /* number of non-tie games */
    // FIXME: read the value from argv[1]
    int nb_round = 3;

    // TODO: check the value is positive, and exit with usage otherwise

    /* main event loop */
    size_t your_score = 0, // number of round you won
           ai_score = 0;   // number of round the AI won

    AI ai;

    while(nb_round) {
        /* do some stuff */
        Weapon your_weapon;
        bool ok = false;
        do {
          // TODO: add some error handling there in case of invalid input
          std::cout << "(r)ock-(p)aper-(s)cissors-(l)izard-(S)pock? ";
          std::cin >> your_weapon;
          std::cin.ignore(1); // needed to skip the newline
          ok = true;
        } while(not ok);

        Weapon ai_weapon = ai.weapon();
        std::cout << "AI: " << ai_weapon << std::endl;

        Status status = Matrix[static_cast<int>(your_weapon)][static_cast<int>(ai_weapon)];

        // FIXME: update `your_score', `ai_score' and `nb_round' according to `status'
        --nb_round;
        std::cout << "Your score: " << your_score << std::endl
                  << "AI score:   " << ai_score << std::endl;
    }
    return 0;
}
