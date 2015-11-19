source $(dirname ${BASH_SOURCE[0]})/getopt.bash

__pgop_options() {
  if [[ $# -eq 0 ]]; then
    contexts="global"
  else
    contexts=$@
  fi

  for context in ${contexts}; do
    local var="$(__pgop_context_var $context)[@]"
    for x in "${!var}"; do
      echo $x
    done
  done
}

__pgop_option() {
  # cherry-pick single options
  [[ $# -lt 2 ]] && pgop_error "needs at least two arguments" && return 1

  context=$1
  shift
  for o in $@; do
    if [[ ${#o} -eq 1 ]]; then
      __pgop_options ${context} | grep -E "${o}[\:]{0,2}\ [^\ ]+\ "
    else
      __pgop_options ${context} | grep -E "[^\ ]{1,3}\ ${o}[:]{0,2}\ "
    fi
  done
  return 0
}

__pgop_option_position() {
  context=$1
  option=$2
  str=$(__pgop_option ${context} ${option})
  n=0
  IFS=$'\n'
  for option in $(__pgop_options ${context}); do
    if [[ "${option}" == "${str}" ]]; then
      echo $n
      return 0
    fi
    n=$(( $n + 1 ))
  done
}

__pgop_option_replace() {
  [[ $# -ne 4 ]] && pgop_error "you must provide four arguments" && return 1
  context=$1
  option=$2
  if ! __pgop_option_exists ${context} ${option}; then
    pgop_error "the option '${option}' does not exist"
    return 1
  fi

  description="$(echo $3 | base64)"
  value="$(echo $4 | base64)"
  position="$(__pgop_option_position ${context} ${option})"
  line="$(
    pgop_read ${context} ${option}   |
      awk '{$3="'${description}'"}1' |
      awk '{$4="'${value}'"}1'
  )"

  local var="$(__pgop_context_var ${context})[${position}]"
  eval "$(echo ${var})='${line}'"
}

__pgop_option_description() {
  context=$1
  name=$2
  __pgop_option ${context} ${name} | awk '{ print $4 }' | base64 -D
}

__pgop_option_value() {
  context=$1
  option=$2
  encoded_value="$(__pgop_option ${context} ${option} | awk '{ print $3 }')"
  if [[ "${encoded_value}" == "-" ]]; then
    return 1
  else
    echo "${encoded_value}" | base64 -D
    return $?
  fi
}


__pgop_option_exists() {
  [[ ! -z "$(__pgop_option $@)" ]]
  return $?
}

__pgop_options_uniq() {
  [[ $# -lt 2 ]] && pgop_error "needs at least two arguments" && return 1

  context=$1
  shift
  for o in $@; do
    [[ ! -z "$(__pgop_option ${context} ${o})" ]] && return 1
  done
  return 0
}

__pgop_option_type() {
  option=$(__pgop_switch_to_opt $@) || return 255           # invalid
  echo "${option}" | grep "::$" > /dev/null  && return 2    # optional
  echo "${option}" | grep ":$" > /dev/null   && return 1    # requrired
  return 0                                                  # noarg
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

__pgop_valid() {
  getopt -o $(__pgop_short_opts) --long $(__pgop_long_opts) -- $*
  return $?
}

# vim: ft=sh
