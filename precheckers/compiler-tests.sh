#!/bin/bash

# Определяем компиляторы для проверки
COMPILERS=("gcc" "clang")

# Флаги компиляции для "чистого C"
CFLAGS="-std=c18 -Wall -Wextra -Werror -pedantic -Wconversion -lm"      # added -lm             TODO: FIXME:
# CFLAGS="-std=c99 -Wall -Wextra -Wconversion"

# Функция для компиляции файла с указанным компилятором
compile_file() {
    local compiler="$1"
    local file="$2"
    local output="${file%.c}.out"

    printf "\n >--> Компиляция '%s' с помощью %s...\n" "$file" "$compiler"
    
    # Компиляция исходного файла
    if ! "$compiler" $CFLAGS -o "$output" "$file"; then
        printf " >--> Ошибка: файл '%s' не скомпилировался с %s.\n" "$file" "$compiler" >&2
        return 1
    else
        printf " >--> OK.\n"
    fi

    # Удаление скомпилированного исполняемого файла
    # rm -f "$output"
    rm "$output"
    return 0
}

# Основная функция для проверки всех файлов
main() {
    local errors=0

    # Проверка переданных аргументов
    if [[ $# -eq 0 ]]; then
        printf "Использование: %s <файл1.c> <файл2.c> ...\n" "$(basename "$0")" >&2
        exit 1
    fi

    # Проверка каждого файла на компиляцию
    for file in "$@"; do
        # Проверка на существование файла
        if [[ ! -f "$file" ]]; then
            printf " >--> Ошибка: файл '%s' не найден.\n" "$file" >&2
            errors=1
            continue
        fi

        # Компиляция файла с каждым компилятором
        for compiler in "${COMPILERS[@]}"; do
            if ! compile_file "$compiler" "$file"; then
                errors=1
            fi
        done
    done

    # Итоговый результат
    if [[ $errors -eq 0 ]]; then
        printf " >--> Все файлы успешно скомпилировались на gcc и clang.\n"
    else
        printf " >--> Некоторые файлы не прошли проверку компиляции.\n" >&2
    fi

    exit $errors
}

main "$@"

