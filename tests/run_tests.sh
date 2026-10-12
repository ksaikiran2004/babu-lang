#!/usr/bin/env bash
set -euo pipefail

binary=${1:-./babu}
temporary_directory=$(mktemp -d)
trap 'rm -rf "$temporary_directory"' EXIT
source_file="$temporary_directory/case.babu"

fail() {
    printf 'FAIL: %s\n' "$1" >&2
    exit 1
}

run_case() {
    local name=$1
    local source=$2
    local expected=$3
    printf '%s\n' "$source" > "$source_file"
    local actual
    actual=$("$binary" "$source_file") || fail "$name: interpreter exited with an error"
    [[ "$actual" == "$expected" ]] || {
        printf 'FAIL: %s\nExpected:\n%s\nActual:\n%s\n' "$name" "$expected" "$actual" >&2
        exit 1
    }
    printf 'PASS: %s\n' "$name"
}

run_error_case() {
    local name=$1
    local source=$2
    local expected=$3
    printf '%s\n' "$source" > "$source_file"
    local actual
    if actual=$("$binary" "$source_file" 2>&1); then
        fail "$name: expected the interpreter to reject the source"
    fi
    [[ "$actual" == *"$expected"* ]] || {
        printf 'FAIL: %s\nExpected diagnostic containing:\n%s\nActual:\n%s\n' \
            "$name" "$expected" "$actual" >&2
        exit 1
    }
    printf 'PASS: %s\n' "$name"
}

run_case "output and UTF-8 text" \
    'chupi("Namaskaram Babu"); chupi("నమస్కారం బాబు");' \
    "$(printf 'Namaskaram Babu\nనమస్కారం బాబు')"

run_case "arithmetic and precedence" \
    'chupi(2 + 3 * 4); chupi((2 + 3) * 4); chupi(7 / 2); chupi(9 % 4);' \
    "$(printf '14\n20\n3.5\n1')"

run_case "inferred variables, reassignment, strings, and braces" \
    '{ count = 3; count = count + 4; greeting = "Babu"; greeting = greeting + "!"; chupi(count); chupi(greeting); }' \
    "$(printf '7\nBabu!')"

run_error_case "incompatible assignment with source location" \
    $'count = 1;\ncount = "one";' \
    ':2:1: error: cannot assign string to integer variable'

run_error_case "missing semicolon diagnostic" \
    'chupi("missing semicolon")' \
    ":2:1: error: expected ';' after statement"

printf 'All 5 tests passed.\n'