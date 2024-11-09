c_files=$(find . -type f -name "*.c" | tr '\n' ' ')

clang -o out/lame_db $c_files
