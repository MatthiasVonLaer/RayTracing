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

#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>

static void RayTraceTestScenes(const std::filesystem::path& path, QApplication& app);
static void CompareActualToExpected(const std::filesystem::path& path);

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
    RayTraceTestScenes(argv[1], app);
    CompareActualToExpected(argv[1]); 
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

static void RayTraceTestScenes(const std::filesystem::path& path, QApplication& app)
{
  std::filesystem::remove_all(path / "actual");
  std::filesystem::create_directory(path / "actual");
  const auto testScenes = path / "test_scenes";
  for (const auto& testScene : std::filesystem::directory_iterator(testScenes))
  {
    if (testScene.path().extension() != ".scn")
    {
      continue;
    }
    Controller controller(app);
    std::ifstream in(testScene.path());
    controller.parse(in);
  }
}

static std::vector<char> Load(const std::filesystem::path& path)
{
  std::ifstream in(path, std::ios::binary);
  std::vector<char> result(
      (std::istreambuf_iterator<char>(in)), (std::istreambuf_iterator<char>()));
  return result;
}

static void CompareActualToExpected(const std::filesystem::path& path)
{
  const auto actualScenes = path / "actual";
  for (const auto& element : std::filesystem::directory_iterator(actualScenes))
  {
    const auto actual = Load(element);
    const auto expected = Load(path / "expected" / element.path().filename());
    if (actual != expected)
    {
      throw std::runtime_error("The test scene output of \""
          + element.path().filename().string() + "\" is not as expected.");
    }
  }
}

