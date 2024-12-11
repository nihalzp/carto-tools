#include "task_handler/task_handler.hpp"
#include "task/compute_area_error.hpp"
#include "task/compute_similarity.hpp"
#include "task/generate_target_csv.hpp"
#include "task/report_intersections.hpp"
#include <argparse/argparse.hpp>

TaskHandler::TaskHandler(argparse::ArgumentParser &&args)
    : arguments_(std::move(args))
{
  task_map_ = {
    {"--similarity",
     [this]() {
       compute_similarity(arguments_);
     }},
    {"--create_csv",
     [this]() {
       generate_target_csv(arguments_);
     }},
    {"--area_error",
     [this]() {
       compute_area_error(arguments_);
     }},
    {"--intersection", [this]() {
       report_intersections(arguments_);
     }}};
}

void TaskHandler::run_task()
{
  for (const auto &task : task_map_) {
    if (arguments_.get<bool>(task.first)) {
      task.second();
      return;
    }
  }
  throw std::runtime_error("No valid task requested.");
}
