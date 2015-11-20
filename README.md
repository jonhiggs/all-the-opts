# Pure-Getopt-Plus

`pure-getopt` which is designed to make GNU getopt's features available everywhere.

This is a wrapper to make it super easy to parse switches.

- validates the inputs
- auto-generate `--help` message
- simple access to switch values
- context aware arguments

## Define the application description

`${PGOP_DESCRIPTION}` contains a short description of the application. It is used at the beginning of the `--help` messages.


## Define the global switches

`${PGOP_GLOBAL}` contains the switches that are to be accepted in all contexts. It contains three sections.

- short argument (getopt style)
- long argument (getopt style)
- help message (a short string)

``` bash
    export PGOP_GLOBAL=(
        "v  version     Display the version information"
    )
```

## Provided Functions

- pgop_add:             context, short, long, description (without last three, show)
- pgop_context:         show set contexts or add a context
- pgop_description:     set or show the description
- pgop_read:            read the definition of an option
- pgop_set:             set or read the value of an argument
- pgop_operands:        the things that are not arguments (what's the real name?)

- pgop_setup:           do this first (may not be necessary)

## Include in your application

``` bash
    source ./pure-getopt-plus.inc
    pgop_description "this is an example application"
    pgop_add global n name    "Set your name"
    pgop_add global v version "Display the version information"
    pgop_context_add "global"

    pgop_setup $@     # we may be able to get away with not doing this

    pgop_set --name "the default name"

    function version() {
        echo "version 1.0.0"
        return 0
    }

    function name() {
        name="$(pgop_value --name)"
        echo "my name is ${name}"
        return 0
    }

    for arg in ${pgop_options}; do
        case $arg in
            "-n"|"--name")
                pgop_set --name $(pgop_value ${arg})
                name
            ;;
            "-v"|"--version") version ;;
        esac
    done
```
