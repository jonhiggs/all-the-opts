source $(dirname ${BASH_SOURCE[0]})/getopt.bash

__pgop_options() {
  # takes contexts as an arguments
  # merges global options with contexts and returns a new list
  for opt in "${PGOP_GLOBAL[@]}"; do
    echo $opt
  done

  for context in $@; do
    var="PGOP_$(echo $context | tr '[a-z]' '[A-Z]')"
    [[ ! -z ${!var} ]] && echo ${!var}
  done
}

__pgop_short_opts() {
  # merges the options to construct the pure-getops '-o' arguments
  for opt in $(__pgop_options $@ | awk '{print $1}'); do
    [[ "${opt}" == '-' ]] && continue
    echo -n ${opt}
  done
}

__pgop_long_opts() {
  # merges the options to construct the pure-getops '--long' arguments
  opts=""
  for opt in $(__pgop_options $@ | awk '{print $2}'); do
    [[ "${opt}" == '-' ]] && continue
    opts+="${opt},"
  done
  echo -n ${opts} | sed 's/,$//g'
}

__pgop_arg_type() {
  # 0 is noarg
  # 1 is optarg
  # 2 is arg
}

__pgop_valid_option() {
  # return true if the switch has been defined either globally, or in the
  # current context.
  return 0
}

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
