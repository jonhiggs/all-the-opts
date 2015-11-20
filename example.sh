#!/usr/bin/env bash
source "$(dirname $0)/lib/pure-getopt-plus.inc"

pgop_description          "This is an example application"
pgop_add global n: name:  "Set your name"
pgop_add global v version "Display the version information"

for o in $@; do
  if [[ $o == '--help' ]]; then
    pgop_help global
    exit 0
  fi
done

#pgop_context_add "global"

#pgop_setup $@     # we may be able to get away with not doing this
#
#pgop_set --name "the default name"
#
#function version() {
#    echo "version 1.0.0"
#    return 0
#}
#
#function name() {
#    name="$(pgop_value --name)"
#    echo "my name is ${name}"
#    return 0
#}
#
#for arg in ${pgop_options}; do
#    case $arg in
#        "-n"|"--name")
#            pgop_set --name $(pgop_value ${arg})
#            name
#        ;;
#        "-v"|"--version") version ;;
#    esac
#done
