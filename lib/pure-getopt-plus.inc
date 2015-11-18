source $(dirname ${BASH_SOURCE[0]})/getopt.bash

__pgop_short_opts() {
  # merges the options to construct the pure-getops '-o' arguments
  local opts=""
  for opt in "${PGOP_GLOBAL[@]}"; do
    opt=$(echo ${opt} | awk '{print $1}')
    [[ "${opt}" == "-" ]] || opts+=$opt
  done
  echo ${opts}
}

__pgop_long_opts() {
  # merges the options to construct the pure-getops '--long' arguments
  local opts=""
  for opt in "${GLOBAL_OPTIONS[@]}"; do
    opt=$(echo ${opt} | awk '{print $2}')
    [[ "${opt}" == "-" ]] || opts+="${opt},"
  done
  echo ${opts} | sed 's/,$//g'
}

#__pgop_arg_type() {
#  # 0 is noarg
#  # 1 is optarg
#  # 2 is arg
#}
#
#__pgop_switch_valid() {
#  # return true if the switch has been defined either globally, or in the
#  # current context.
#}

__pgop_switch_value() {
  # takes a switch and echo 'true', 'false' or the value of the switch.
  # return 0 if switch exists
  # return 1 if switch does not exist

  argv=$(getopt -o $(__pgop_short_opts) --long $(__pgop_long_opts) -- $*) || return
  eval "set -- $argv"
  declare a
  for a; do
    case $a in
      "--filters")
        echo "switch is $a"
        sflags=$2
        echo "sflag is $sflags"
        shift
      ;;
      "--version")
        echo "bash-my-aws v1.0"
      ;;
    esac
  done
}

# vim: ft=sh
