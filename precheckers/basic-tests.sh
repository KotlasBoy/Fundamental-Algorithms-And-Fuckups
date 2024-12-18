#!/bin/bash

# Запрещённые слова для проверки
declare -a forbidden_words=("goto" "realloc" "stoi" "stol" "static" "exit" "errno" "todo" "TODO" "gets" "scanf")

# Функция для проверки файла на наличие запрещённых слов
check_file() {
    local file="$1"
    local found=0

    # Проверка, существует ли файл
    if [[ ! -f "$file" ]]; then
        printf "Файл '%s' не найден.\n" "$file" >&2
        return 1
    fi

    # Проверка на наличие запрещённых слов
    for word in "${forbidden_words[@]}"; do
        if grep -q -w "$word" "$file"; then
            printf " -> в '%s' найдено слово '%s'.\n" "$file" "$word"
            found=1
        fi
    done

    # Сообщение об отсутствии ошибок
    if [[ $found -eq 0 ]]; then
        printf "Файл '%s' прошёл проверку.\n" "$file"
    fi
    return $found
}

# Основная функция для обработки нескольких файлов
main() {
    local errors=0

    # Проверка переданных аргументов
    if [[ $# -eq 0 ]]; then
        printf "Использование: %s <файл1.c> <файл2.c> ...\n" "$(basename "$0")" >&2
        exit 1
    fi

    # Проверка каждого файла из аргументов
    for file in "$@"; do
        if ! check_file "$file"; then
            errors=1
        fi
    done

    exit $errors
}

main "$@"

