#include "task_handler/task_handler.hpp"
#include "task/compute_area_error.hpp"
#include "task/compute_similarity.hpp"
#include "task/generate_target_csv.hpp"
#include "task/report_intersections.hpp"
#include <argparse/argparse.hpp>

TaskHandler::TaskHandler(argparse::ArgumentParser &&args)
    : arguments(std::move(args))
{
  task_map = {
    {"--similarity",
     [this]() {
       compute_similarity(arguments);
     }},
    {"--create_csv",
     [this]() {
       generate_target_csv(arguments);
     }},
    {"--area_error",
     [this]() {
       compute_area_error(arguments);
     }},
    {"--intersection", [this]() {
       report_intersections(arguments);
     }}};
}

void TaskHandler::run_task()
{
  for (const auto &task : task_map) {
    if (arguments.get<bool>(task.first)) {
      task.second();
      return;
    }
  }
  throw std::runtime_error("No valid task requested.");
}
