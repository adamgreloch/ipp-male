#!/bin/bash

GREEN='\033[0;32m'
RED='\033[0;31m'
WHITE='\033[0;37m'
RESET='\033[0m'

for f in "$2"/*.in
do
echo    ./"$1" < "$f" > "${f%in}outp"
done

for f in "$2"/*.in
do
    echo -n "${f#*/} ";
    if [ -s "${f%in}err" ]
    then
        if diff "${f%in}err" "${f%in}outp" > /dev/null 1>&1
        then
            echo -e "${GREEN}passed${RESET}";
        else
            echo -e "${RED}failed${RESET}"
            diff -y -W 30 "${f%in}outp" "${f%in}err"
        fi
    else
        if diff "${f%in}out" "${f%in}outp" > /dev/null 2>&1
        then
            echo -e "${GREEN}passed${RESET}";
        else
            echo -e "${RED}failed${RESET}"
            diff -y -W 30 "${f%in}outp" "${f%in}out"
        fi
    fi
done
