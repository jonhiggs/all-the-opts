# All The Opts

Wouldn't it be nice if didn't have to use getopt anymore?

`pure-getopt` which is designed to make GNU getopt's features available everywhere.

This is a wrapper to make it super easy to parse switches.

- validates the inputs
- auto-generate `--help` message
- simple access to switch values
- context aware arguments

## Define the application description

`${ATO_DESCRIPTION}` contains a short description of the application. It is used at the beginning of the `--help` messages.


## Define the global switches

`${ATO_GLOBAL}` contains the switches that are to be accepted in all contexts. It contains three sections.

- short argument (getopt style)
- long argument (getopt style)
- help message (a short string)

``` bash
    export ATO_GLOBAL=(
        "v  version     Display the version information"
    )
```

## Terminology

- option
- option_argument
- operand
- context
- optional_argument
- required_argument
- no_argument


## Functionality

### ato_add




- ato_add:             context, short, long, description (without last three, show)
- ato_context:         show set contexts or add a context
- ato_description:     set or show the description
- ato_help:            generate a help message
- ato_operands:        the things that are not arguments (what's the real name?)
- ato_read:            should read the option argument
- ato_setup:           do this first (may not be necessary)

## Include in your application

``` bash
    source ./all-the-opts.inc
    ato_description "this is an example application"
    ato_add global n name    "Set your name"
    ato_add global v version "Display the version information"
    ato_context_add "global"

    ato_setup $@     # we may be able to get away with not doing this

    ato_set --name "the default name"

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
