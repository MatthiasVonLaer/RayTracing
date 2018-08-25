//  A raytracer program for rendering photorealistic images
//  Copyright 2018 Matthias von Laer
//
//  This program is released under the GNU GPLv3.
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <model/controller.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>

static void RayTraceTestScenes(const std::filesystem::path& path);
static void CompareActualToExpected(const std::filesystem::path& path);
static void CheckAllTested(const std::filesystem::path& path);

int main(int argc, char *argv[])
{
  try
  {
    if(argc < 2)
    {   
      throw std::invalid_argument(
          "No test files directory passed as input parameter.");
    }
    RayTraceTestScenes(argv[1]);
    CompareActualToExpected(argv[1]); 
    CheckAllTested(argv[1]);
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

static void RayTraceTestScenes(const std::filesystem::path& path)
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
    Controller controller;
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
  for (const auto& actualScene : std::filesystem::directory_iterator(actualScenes))
  {
    const auto actual = Load(actualScene);
    const auto expected = Load(path / "expected" / actualScene.path().filename());
    if (actual != expected)
    {
      throw std::runtime_error("The test scene output of \""
          + actualScene.path().filename().string() + "\" is not as expected.");
    }
  }
}

static void CheckAllTested(const std::filesystem::path& path)
{
  const auto expectedScenes = path / "expected";
  for (const auto& expectedScene : std::filesystem::directory_iterator(expectedScenes))
  {
    const auto actualScene = path / "actual" / expectedScene.path().filename();
    if (!std::filesystem::exists(actualScene))
    {
      throw std::runtime_error("The expected scene "
          + expectedScene.path().filename().string() + " was not tested.");
    }
  }
}
