#!/bin/bash
#  A raytracer program for rendering photorealistic images
#  Copyright 2018 Matthias von Laer
#
#  This program is released under the GNU GPLv3.
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.

nCores=$(nproc --all)
cmake --build . --config release --target raytracing_unit_test -- -j$nCores \
        && ctest -R raytracing_unit_test --output-on-failure | grep --color -E '^|Failed'
