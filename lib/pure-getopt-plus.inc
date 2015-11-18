source $(dirname ${BASH_SOURCE[0]})/getopt.bash

__pgop_context_var() {
  # return the variable name for a context
  echo "PGOP_$(echo $1 | tr '[a-z]' '[A-Z]')"
}

__pgop_options() {
  if [[ $# -eq 0 ]]; then
    contexts="global"
  else
    contexts=$@
  fi

  for context in ${contexts}; do
    local var=$(__pgop_context_var $context)
    [[ ! -z ${!var} ]] && echo ${!var}
  done
}

__pgop_options_uniq() {
  if [[ $# -lt 2 ]]; then
    echo "needs at least two arguments" 1>&2
    return 1
  fi

  context=$1
  shift
  for o in $@; do
    if [[ ${#o} -eq 1 ]]; then
      # compare against short options
      count=$(__pgop_options ${context} | awk '{ print $1 }' | grep -c ^${o}$)
      [[ ${count} -eq 0 ]] || return 1
    else
      count=$(__pgop_options ${context} | awk '{ print $2 }' | grep -c ^${o}$)
      [[ ${count} -eq 0 ]] || return 1
    fi
  done
  return 0
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

__pgop_switch_to_opt() {
  # return the getopt config for a given switch
  switch=$1
  shift
  contexts=$@

  IFS=$'\n'
  for o in $(__pgop_options); do
    shortopt=$(echo $o | awk '{ print $1 }')
    longopt=$(echo $o | awk '{ print $2 }')

    if [[ "$(__pgop_shortopt_to_switch ${shortopt})" == "${switch}" ]]; then
      echo $o
      return 0
    fi

    if [[ "$(__pgop_longopt_to_switch ${longopt})" == "${switch}" ]]; then
      echo $o
      return 0
    fi
  done
  return 1
}

__pgop_shortopt_to_switch() {
  # convert a shortopt to the switch
  echo $1 | sed 's/^/-/g' | sed -E 's/:{1,2}$//g'
}

__pgop_longopt_to_switch() {
  # convert a longopt to the switch
  echo $1 | sed 's/^/--/g' | sed -E 's/:{1,2}$//g'
}

__pgop_valid_option() {
  __pgop_switch_to_opt $@ > /dev/null
  return $?
}

__pgop_arg_type() {
  option=$(__pgop_switch_to_opt $@) || return 255           # invalid
  echo "${option}" | grep "::$" > /dev/null  && return 2    # optional
  echo "${option}" | grep ":$" > /dev/null   && return 1    # requrired
  return 0                                                  # noarg
}

__pgop_valid() {
  getopt -o $(__pgop_short_opts) --long $(__pgop_long_opts) -- $*
  return $?
}



# vim: ft=sh
