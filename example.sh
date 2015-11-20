#!/usr/bin/env bash
source "$(dirname $0)/lib/pure-getopt-plus.inc"

pgop_description          "This is an example application"
pgop_add global h  help   "Set your name"
pgop_add global n: name:  "Set your name"
pgop_add global v version "Display the version information"

pgop_setup global $@ || exit 1

echo "operands are:"
pgop_operands

pgop_argument global version && echo "version is set"

name="$(pgop_argument global name)"
if [[ ! -z ${name} ]]; then
  echo "name is ${name}"
fi
