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

static void RayTraceTestFiles(const std::filesystem::path& path, QApplication& app)
{
  for (const auto& element : std::filesystem::directory_iterator(path))
  {
    Controller controller(app);
    std::ifstream in(element.path());
    controller.parse(in);
  }
}

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
    RayTraceTestFiles(argv[1], app);
    //Todo: CompareActualToExpected(argv[1]); 
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
