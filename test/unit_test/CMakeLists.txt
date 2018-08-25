#  A raytracer program for rendering photorealistic images
#  Copyright 2018 Matthias von Laer
#
#  This program is released under the GNU GPLv3.
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.1)

find_package(Boost 1.61.0 COMPONENTS unit_test_framework)

include_directories(../..)

add_executable(raytracing_unit_test
    ../../model/plane_test.cc
    ../../model/shapes/ball_test.cc
    main.cc
)
target_link_libraries(raytracing_unit_test
  raytracing_model
  ${Boost_LIBRARIES}
)
set_property(TARGET raytracing_unit_test PROPERTY CXX_STANDARD 17)

add_test(raytracing_unit_test raytracing_unit_test)
