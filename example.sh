#!/usr/bin/env bash
source "$(dirname $0)/lib/all-the-opts.inc"

ato_description               "This is an example application"
ato_context                   "global"
ato_add           v version   "Display the version information"
ato_add           h help      "Show this help"

ato_setup $@ || exit 0

for option in $(ato_options); do
  value="$(ato_read ${option})"

  case ${option} in
    "version")
      echo "the version of the app is 0.1"
      exit 0
      ;;
    "help")
      ato_help
      exit 0
    ;;
  esac
done

for operand in $(ato_operands); do
  echo "found an operand of ${operand}"
done

#ato_argument global version && echo "version is set"
#
#name="$(ato_argument global name)"
#if [[ ! -z ${name} ]]; then
#  echo "name is ${name}"
#fi
