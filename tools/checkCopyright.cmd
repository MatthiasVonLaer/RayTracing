#!/bin/bash
#  A raytracer program for rendering photorealistic images
#  Copyright 2018 Matthias von Laer
#
#  This program is released under the GNU GPLv3.
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.

author=$2
year=$1
git ls-files \
      | grep -Ev "\.gitignore|\.jpg|\.md" \
      | xargs grep -L "Copyright.*year.*author"
