#!/bin/bash

ans_passed=0
ans_failed=0
err_passed=0
err_failed=0
mem_errors=0

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

echo -e "\n### Program correctness test ###\n"

time for f in "$2"/*.in
do
    ./"$1" <"$f" 1>"${f%in}outp" 2>"${f%in}errp"
    echo "--- ${f#*/}"
    if diff "${f%in}outp" "${f%in}out" > /dev/null 1>&1
    then
        echo -e "Answer ${GREEN}OK${RESET}"
        rm -f "${f%in}outp" > /dev/null
        ((ans_passed++))
    else
        echo -e "Answer ${RED}failed${RESET}!"
        echo "Expected:"
        comm -13 "${f%in}outp" "${f%in}out"
        echo "Got:"
        comm -23 "${f%in}outp" "${f%in}out"
        ((ans_failed++))
    fi

    if diff "${f%in}errp" "${f%in}err" > /dev/null 1>&1
    then
        echo -e "Error ${GREEN}OK${RESET}"
        rm -f "${f%in}errp" > /dev/null
        ((err_passed++))
    else
        echo -e "Error ${RED}failed${RESET}!"
        echo "Expected:"
        comm -13 "${f%in}errp" "${f%in}err"
        echo "Got:"
        comm -23 "${f%in}errp" "${f%in}err"
        ((err_failed++))
    fi
done

echo -e "\n### Memory integrity test ###\n"

for f in "$2"/*.in
do
    valgrind --leak-check=full \
             --show-leak-kinds=all \
             --errors-for-leak-kinds=all \
             --quiet \
             --log-file="${f%in}memerr" \
    ./"$1" <"$f" 1>/dev/null 2>/dev/null

    echo "--- ${f#*/}";
    if [ ! -s "${f%in}memerr" ]
    then
        echo -e "Memory ${GREEN}OK${RESET}"
    else
        echo -e "${RED}Memory problems!${RESET}"
        cat "${f%in}memerr"
        ((mem_errors++))
    fi
done

total=$((ans_passed+ans_failed))

echo -e "\nIn total:"
echo "Correct answers: $ans_passed/$total"
echo "Errors handled correctly: $err_passed/$total"
echo "Tests where memory issues occured: $mem_errors"
