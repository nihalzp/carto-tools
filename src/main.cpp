#include "parse/parse_arguments.hpp"
#include "task_handler/task_handler.hpp"
#include <argparse/argparse.hpp>
#include <iostream>

int main(int argc, char *argv[])
{
  try {
    argparse::ArgumentParser arguments = parse_arguments(argc, argv);
    TaskHandler handler(std::move(arguments));
    handler.run_task();
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << "\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
