#  A raytracer program for rendering photorealistic images
#  Copyright 2018 Matthias von Laer
#
#  This program is released under the GNU GPLv3.
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_COMPILER mpic++)

include_directories(../..)

add_executable(raytracing_performance_test main.cc)
target_link_libraries(raytracing_performance_test
  raytracing_model
  stdc++fs
)
set_property(TARGET raytracing_performance_test PROPERTY CXX_STANDARD 17)

add_test(raytracing_performance_test
    raytracing_performance_test ${CMAKE_CURRENT_SOURCE_DIR}
)
