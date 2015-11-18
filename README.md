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

## Include in your application

``` bash
    source ./pure-getopt-plus.inc
    export PGOP_DESCRIPTION="this is an example application"
    export PGOP_GLOBAL=(
        "v  version     Display the version information"
    )

    PGOP_CONTEXTS="global"
    pgop_setup $@

    pgop_set --name "unnamed"

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
            "-v"|"--version") version ;;
            "-n"|"--name")
                pgop_set --name $(pgop_value ${arg})
                name
            ;;
        esac
    done
```
