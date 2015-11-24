# All The Opts

Wouldn't it be nice if didn't have to use getopt anymore?

`pure-getopt` which is designed to make GNU getopt's features available everywhere.

This is a wrapper to make it super easy to parse switches.

- validates the inputs
- auto-generate `--help` message
- simple access to switch values
- context aware arguments


## Glossary

TODO: Write a glossary of terms

- option
- option_argument
- operand
- context
- optional_argument
- required_argument
- no_argument


## Functionality

- ato_add:             context, short, long, description (without last three, show)
- ato_context:         show set contexts or add a context
- ato_description:     set or show the description
- ato_help:            generate a help message
- ato_operands:        the things that are not arguments (what's the real name?)
- ato_read:            should read the option argument
- ato_setup:           do this first (may not be necessary)
- ato_options:         a list of the known options

## A Usage Example

``` bash
    #!/usr/bin/env bash
    source ./all-the-opts.inc

    ato_description          "this is an example application"
    ato_context_add          "global"

    ato_add global n name    "Set your name"
    ato_add global v version "Display the version information"

    ato_setup $@

    ato_set --name "the default value of name"

    function version() {
        echo "version 1.0.0"
        return 0
    }

    function name() {
        name="$(ato_value --name)"
        echo "my name is ${name}"
        return 0
    }

    for arg in ${ato_options}; do
        case $arg in
            "-n"|"--name")
                ato_set --name $(ato_value ${arg})
                name
            ;;
            "-v"|"--version") version ;;
        esac
    done
```
