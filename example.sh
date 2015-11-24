#!/usr/bin/env bash
source "$(dirname $0)/lib/pure-getopt-plus.inc"
pgop_context "global"
pgop_description          "This is an example application"

#          CONTEXT      SHORT  LONG      DESCRIPTION
#pgop_add   n:     name:     "Set your name"
#pgop_read
#pgop_add   v      version   "Display the version information"

pgop_add   h      help      "Set your name"
pgop_read
#pgop_setup $@ || exit 0

#for operand in pgop_operands; do
#  echo "found an operand of ${operand}"
#done
#
#pgop_argument global version && echo "version is set"
#
#name="$(pgop_argument global name)"
#if [[ ! -z ${name} ]]; then
#  echo "name is ${name}"
#fi
