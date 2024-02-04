#include "lc3.h"
#include <fmt/core.h>
#include <span>
#include <string>

int main(int argc, char** argv) {
    std::string image_path;
    if (argc < 2) {
        fmt::print("lc3 [image-file1] ...\n");
        return 2;
    }

    auto args = std::span(argv, size_t(argc));
    image_path = std::string(args[1]);

    auto lc3 = Lc3{};
    lc3.setup();
    lc3.load_image(image_path);

    while (true) {
        lc3.run();
    }

    return 0;
}
