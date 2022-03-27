#!/bin/bash

GREEN='\033[0;32m'
RED='\033[0;31m'
#WHITE='\033[0;37m'
RESET='\033[0m'

rm -f "$2"/*.outp
rm -f "$2"/*.errp

for f in "$2"/*.in
do
    # todo: valgrind $VALARGS $1 < $file 2> /tmp/lab.err > /tmp/lab.out
    ./"$1" < "$f" > "${f%in}outp" 2>"${f%in}errp"

    echo -n "${f#*/} ";
    if diff "${f%in}errp" "${f%in}err" > /dev/null 1>&1 &&
    diff "${f%in}outp" "${f%in}out" > /dev/null 1>&1
    then
        echo -e "${GREEN}passed${RESET}"
    else
        echo -e "${RED}failed${RESET}"
        cat "$f";
        diff -y -W 30 "${f%in}outp" "${f%in}out"
        diff -y -W 30 "${f%in}errp" "${f%in}err"
    fi
done
