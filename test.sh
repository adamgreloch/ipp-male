#!/bin/bash

GREEN='\033[0;32m'
RED='\033[0;31m'
RESET='\033[0m'

# Clean up.
for f in "$2"/*.in
do
    rm -f "${f%in}outp" > /dev/null
    rm -f "${f%in}errp" > /dev/null
    rm -f "${f%in}memerr" > /dev/null
done

ans_passed=0
ans_failed=0
err_passed=0
err_failed=0
mem_errors=0

time for f in "$2"/*.in
do
    valgrind --leak-check=full \
             --show-leak-kinds=all \
             --errors-for-leak-kinds=all \
             --quiet \
             --log-file="${f%in}memerr" \
    ./"$1" <"$f" 1>"${f%in}outp" 2>"${f%in}errp"

    echo -en "--- ${f#*/}\n";
    if diff "${f%in}outp" "${f%in}out" > /dev/null 1>&1
    then
        echo -e "answer ${GREEN}OK${RESET}"
        ((ans_passed++))
    else
        echo -e "answer ${RED}failed${RESET}:"
        diff -y -W 50 "${f%in}outp" "${f%in}out"
        ((ans_failed++))
    fi

    if diff "${f%in}errp" "${f%in}err" > /dev/null 1>&1
    then
        echo -e "error ${GREEN}OK${RESET}"
        ((err_passed++))
    else
        echo -e "error ${RED}failed${RESET}:"
        diff -y -W 50 "${f%in}errp" "${f%in}err"
        ((err_failed++))
    fi

    if [ ! -s "${f%in}memerr" ]
    then
        echo -e "memory ${GREEN}OK${RESET}"
    else
        echo -e "${RED}memory problems!${RESET}"
        cat "${f%in}memerr"
        ((mem_errors++))
    fi
done

total=$((ans_passed+ans_failed))

echo -e "\nIn total:"
echo "Number of tests: $total"
echo -e "Correct answers: $ans_passed"
echo "Errors handled correctly: $err_passed"
echo "Tests where memory issues occured: $mem_errors"

