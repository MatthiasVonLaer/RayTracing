#!/bin/bash
git ls-files \
      | grep -Ev "\.gitignore|\.jpg|\.md" \
      | xargs grep -L "Copyright.*$1.*$2"
