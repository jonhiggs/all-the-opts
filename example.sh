#!/usr/bin/env bash
source "$(dirname $0)/lib/all-the-opts.inc"
ato_context "global"
ato_description          "This is an example application"

#          CONTEXT      SHORT  LONG      DESCRIPTION
#ato_add   n:     name:     "Set your name"
#ato_read
#ato_add   v      version   "Display the version information"

ato_add   h      help      "Set your name"
ato_read
#ato_setup $@ || exit 0

#for operand in ato_operands; do
#  echo "found an operand of ${operand}"
#done
#
#ato_argument global version && echo "version is set"
#
#name="$(ato_argument global name)"
#if [[ ! -z ${name} ]]; then
#  echo "name is ${name}"
#fi
