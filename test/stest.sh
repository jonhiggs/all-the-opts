#!/usr/bin/bash
source ../lib/pure-getopt-plus.inc

export PGOP_MY_CONTEXT=(
  "m mine"
)

export PGOP_GLOBAL=(
  "- noarg"
  "- arg:"
  "- optarg::"
)

__pgop_valid $*
echo $?

#
#argv=$(getopt -o $(__pgop_short_opts) --long $(__pgop_long_opts) -- $*) || return
#eval "set -- $argv"
#declare a
#for a; do
#  case $a in
#    "--filters")
#      echo "switch is $a"
#      sflags=$2
#      echo "sflag is $sflags"
#      shift
#    ;;
#    "--version")
#      echo "bash-my-aws v1.0"
#    ;;
#  esac
#done
