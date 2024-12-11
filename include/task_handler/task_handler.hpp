#pragma once

#include <argparse/argparse.hpp>
#include <functional>
#include <string>
#include <unordered_map>

class TaskHandler
{
public:
  explicit TaskHandler(argparse::ArgumentParser &&);
  void run_task();

private:
  argparse::ArgumentParser arguments_;
  std::unordered_map<std::string, std::function<void()>> task_map_;
};
