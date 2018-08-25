#!/bin/bash
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
      | tee /dev/tty \
      | xargs sed -i "s/ $author/, $year $author/"
