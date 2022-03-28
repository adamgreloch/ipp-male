#!/bin/bash

GREEN='\033[0;32m'
RED='\033[0;31m'
#WHITE='\033[0;37m'
RESET='\033[0m'

rm -f "$2"/*.outp
rm -f "$2"/*.errp

ans_passed=0
ans_failed=0
err_passed=0
err_failed=0

time for f in "$2"/*.in
do
    # todo: valgrind $VALARGS $1 < $file 2> /tmp/lab.err > /tmp/lab.out
    ./"$1" < "$f" > "${f%in}outp" 2>"${f%in}errp"

    echo -en "--- ${f#*/}\n";
    if diff "${f%in}outp" "${f%in}out" > /dev/null 1>&1
    then
        echo -e "answer ${GREEN}OK${RESET}"
        ((ans_passed++))
    else
        echo -e "answer ${RED}failed${RESET}:"
        diff -y -W 30 "${f%in}outp" "${f%in}out"
        ((ans_failed++))
    fi

    if diff "${f%in}errp" "${f%in}err" > /dev/null 1>&1
    then
        echo -e "error ${GREEN}OK${RESET}"
        ((err_passed++))
    else
        echo -e "error ${RED}failed${RESET}:"
        diff -y -W 30 "${f%in}errp" "${f%in}err"
        ((err_failed++))
    fi
done

ans_total=$((ans_passed+ans_failed))
err_total=$((err_passed+err_failed))

echo -e "\nIn total:"
echo -e "Correct answers: $ans_passed/$ans_total"
echo "Correct errors: $err_passed/$err_total"

