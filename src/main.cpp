#include <iostream>
#include <spdlog/spdlog.h>

#include "../bin/version.h"

int main(int argc, char** argv) {
    auto console = spdlog::stdout_logger_st("console");
    console->info("Welcome to spdlog!");


    console->info("Goodbye!");

    return 0;
}