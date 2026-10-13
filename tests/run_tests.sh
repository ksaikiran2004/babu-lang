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

run_file_case() {
    local name=$1
    local file=$2
    local expected=$3
    local actual
    actual=$("$binary" "$file") || fail "$name: interpreter exited with an error"
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

run_case "typed function and multiple arguments" \
    $'fun add(left: integer, right: integer) -> integer {\n return left + right;\n}\nchupi(add(3, 4));' \
    '7'

run_case "inferred return, nested call, and local scope" \
    $'outer = 90;\nfun increment(value: integer) { local = value + 1; outer = 7; return local; }\nfun twice(value: integer) { return increment(increment(value)); }\nchupi(twice(3));\nchupi(outer);' \
    "$(printf '5\n90')"

run_case "typed string function" \
    'fun greet(name: string) { return "Namaskaram, " + name; } chupi(greet("Babu"));' \
    'Namaskaram, Babu'

run_case "return inference ignores sample-only division by zero" \
    'fun divide(value: integer) { return 10 / (value - 1); } chupi(divide(3));' \
    '5'

run_case "recursive function with declared return type" \
    'fun recur(value: integer) -> integer { return recur(value); } chupi("loaded");' \
    'loaded'

run_error_case "incorrect function argument count" \
    'fun add(left: integer, right: integer) -> integer { return left + right; } chupi(add(1));' \
    "error: function 'add' expects 2 arguments, got 1"

run_error_case "incompatible function argument type" \
    'fun identity(value: integer) -> integer { return value; } chupi(identity("one"));' \
    "error: argument 1 of 'identity' must be integer, got string"

run_error_case "incompatible declared return type" \
    'fun wrong() -> integer { return "one"; }' \
    'error: function must return integer, got string'

run_error_case "recursive function needs explicit return type" \
    'fun recur(value: integer) { return recur(value); }' \
    "error: recursive function 'recur' needs an explicit return type"

run_error_case "function body type error has source line" \
    $'fun invalid(value: integer) {\n return value + "bad";\n}' \
    ':2:15: error: operator'

run_file_case "greeting demo" "$(dirname "$0")/../examples/greeting.babu" \
    'Namaste Hyderabad'

run_file_case "variable addition demo" "$(dirname "$0")/../examples/addition.babu" \
    '7 + 5 = 12'

run_case "approved function and return keywords" \
    'kriya add(left: integer, right: integer) -> integer { astiram total = left + right; pampu total; } chupi(add(7, 5));' \
    '12'

run_case "constant declaration" \
    'stiram greeting = "namaste"; chupi(greeting);' \
    'namaste'

run_error_case "constant cannot be reassigned" \
    'stiram answer = 42; answer = 7;' \
    "error: cannot reassign constant 'answer'"

printf 'All 20 tests passed.\n'