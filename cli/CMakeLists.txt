#  A raytracer program for rendering photorealistic images
#  Copyright 2018 Matthias von Laer
#
#  This program is released under the GNU GPLv3.
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.1)

INCLUDE_DIRECTORIES(..)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_COMPILER mpic++)

add_executable(raytracing_cli main.cc)
target_link_libraries(raytracing_cli raytracing_model)
set_property(TARGET raytracing_cli PROPERTY CXX_STANDARD 17)
