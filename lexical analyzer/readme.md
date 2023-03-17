# Lexcial analyzer for C- language


# Usage

## 1. Change direcotry to the current directory
    
```sh 
    cd repo/lexical_analyzer
```

## 2. Build using Makefile

```sh
make
```
**Note:** running make will create a binary file named `lex` in the current directory and run it for the input file `input.c` and output file `example_output.txt`

## 3. Run

```sh
./lex [--stop-on-error|s] [--compact|c] <input_file> [-o|--output <output_file>]
```

<!-- flags explanation -->

### Flags

- `--stop-on-error` or `-s` : Stop the program on the first error encountered
- `--compact` or `-c` : Print the output in compact form
- `--output` or `-o` : Specify the output file