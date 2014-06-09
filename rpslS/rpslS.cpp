#include <iostream>

static
int usage(char const *progname) {
    std::cerr << "usage: " << progname << " <positive_number>" << std::endl;
    return 1;
}

int main(int argc, char * argv[]) {
    if(argc != 2) {
        return usage(argv[0]);
    }
    return 0;
}
