#!/bin/bash
#  A raytracer program for rendering photorealistic images
#  Copyright 2018 Matthias von Laer
#
#  This program is released under the license GNU GPLv3.
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.

author=$2
year=$1
echo "Updating copyright for author '$author' and year '$year'? [Y/n]"
read -n 1 -r
echo
if [[ ! $REPLY =~ ^[Yy]$ ]] && [[ ! $REPLY =~ ^$ ]]
then
  exit 1
fi

echo "Updating those files? [Y/n]"
git ls-files \
      | xargs grep -l "Copyright.*$author" \
      | xargs grep -L "Copyright.*$year"
read -n 1 -r
echo
if [[ ! $REPLY =~ ^[Yy]$ ]] && [[ ! $REPLY =~ ^$ ]]
then
  exit 1
fi

git ls-files \
      | xargs grep -l "Copyright.*$author" \
      | xargs grep -L "Copyright.*$year" \
      | xargs sed -i "s/ $author/, $year $author/"
