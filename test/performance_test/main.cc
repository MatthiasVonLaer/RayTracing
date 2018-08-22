//  Copyright 2018 Matthias von Laer
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "controller.h"
#include "mpi_manager.h"

#include <QApplication>

#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>

static void RunPerformanceTests(const std::filesystem::path& path, QApplication& app);
static bool CheckPerformanceTests(const std::filesystem::path& testSceneLog);

int main(int argc, char *argv[])
{
  try
  {
    if(argc < 2)
    {   
      throw std::invalid_argument(
          "No test files directory passed as input parameter.");
    }
    mpi().init(argc,argv);
    QApplication app(argc, argv);
    RunPerformanceTests(argv[1], app);
    const auto success = CheckPerformanceTests(argv[1]);
    return success ? EXIT_SUCCESS : EXIT_FAILURE;
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
}

static void RayTrace(const std::filesystem::path& path, QApplication& app)
{
  Controller controller(app);
  std::ifstream in(path);
  controller.parse(in);
}

static void LogElapsedTime(const std::filesystem::path& path,
    const std::chrono::duration<double>& elapsedTime)
{
  std::ofstream out(path, std::ios::app);
  const auto time = std::time(nullptr);
  const auto localtime = *std::localtime(&time);
  out << std::put_time(&localtime, "%d-%m-%Y_%H:%M:%S") << "   ";
  out << elapsedTime / std::chrono::seconds(1) << std::endl;
}

static void RunPerformanceTests(const std::filesystem::path& path, QApplication& app)
{
  const auto timerLogs = path / "timer_log";
  std::filesystem::create_directory(timerLogs);
  const auto testScenes = path / "test_scenes";
  for (const auto& testScene : std::filesystem::directory_iterator(testScenes))
  {
    if (testScene.path().extension() != ".scn")
    {
      continue;
    }
    const auto start = std::chrono::steady_clock::now();
    RayTrace(testScene.path(), app);
    const auto end = std::chrono::steady_clock::now();

    const auto testSceneLog
      = timerLogs / testScene.path().filename().replace_extension(".txt");

    LogElapsedTime(testSceneLog, end - start);
  }
}

static std::vector<double> ReadTimes(std::istream& in)
{
  std::string date;
  double time;
  std::vector<double> allTimes;
  while(in >> date && in >> time)
  {
    allTimes.push_back(time);
  }
  return allTimes;
}

static bool CheckPerformanceTests(const std::filesystem::path& path)
{
  const auto timerLogs = path / "timer_log";
  bool success = true;
  for (const auto& logFile : std::filesystem::directory_iterator(timerLogs))
  {
    std::ifstream in(logFile.path());
    const auto allTimes = ReadTimes(in);

    const auto average = std::accumulate(
        std::cbegin(allTimes), std::cend(allTimes), 0.) / std::size(allTimes);

    const auto minmax
      = std::minmax_element(std::cbegin(allTimes), std::cend(allTimes));

    const auto min = *minmax.first;
    const auto max = *minmax.second;
    const auto actual = allTimes.back();
    const auto ok = (actual < 1.25 * min);

    std::cout << logFile.path() << ": ";
    std::cout << "actual(" << actual << ") ";
    std::cout << "average(" << average << ") ";
    std::cout << "min(" << min << ") ";
    std::cout << "max(" << max << ") ";
    std::cout << "ok(" << ok << ").\n";

    success &= ok;
  }
  std::cout << std::flush;
  return success;
}
