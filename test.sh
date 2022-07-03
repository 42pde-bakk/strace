#!/usr/bin/env bash

RED=$'\e[1;31m'
GREEN=$'\e[1;32m'
MAGENTA=$'\e[1;35m'
CYN=$'\e[1;36m'
END=$'\e[0m'
make -s

function test {
  strace "$@" >/dev/null 2>&1
  og_outcome=$?
  ./ft_strace "$@" >/dev/null 2>&1
  ft_outcome=$?

  if [[ $og_outcome -ne $ft_outcome ]]; then
    echo "[${RED}KO${END}] Test '$@'"
    exit 1
  else
    echo "[${GREEN}OK${END}] Test '$@'"
  fi
}

test ls
test ls -la
test -c ls -la
test -c -- ls
test -- -c ls
test -- ls

for i in {1..5}
do
  OUTFILE="correction/test_${i}.out"
  gcc -pthread "correction/test_${i}.c" -o $OUTFILE
  if [[ $i -ne 2 ]]; then
    test $OUTFILE
  fi
done
