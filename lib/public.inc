source $(dirname ${BASH_SOURCE[0]})/message_renderer.inc
source $(dirname ${BASH_SOURCE[0]})/options.inc
#source $(dirname ${BASH_SOURCE[0]})/pure-getopt-plus.inc

PGOP_CONTEXT=("global")

pgop_description() {
  if [[ -z $1 ]]; then
    echo ${PGOP_DESCRIPTION}
  else
    export PGOP_DESCRIPTION="$@"
  fi
}

pgop_add() {
  # Add an option to a context. If no option is provided, then the available
  # options for the context is shown.

  context=$1
  short=$2
  long=$3
  description="$(echo $4 | base64)"
  value=${5:-"-"}

  case $# in
    1)
      pgop_read ${context}
    ;;
    4|5)
      if ! __pgop_options_uniq ${context} ${short} ${long}; then
        pgop_error "non-unique option"
        return 1
      fi

      var=$(__pgop_context_var $context)
      eval "$(echo ${var})+=('${short} ${long} ${description} ${value}')"
    ;;
    *)
      # TODO: change this to standard USAGE
      pgop_error "you must provide context, short option, long option,
        description and optionally a value too."
      return 1
      ;;
  esac
}

pgop_context() {
  # show the current context. Add any arguments into the context.
  for context in $@; do
    PGOP_CONTEXT+=($context)
  done
  for context in ${PGOP_CONTEXT[@]}; do
    echo ${context}
  done
}

pgop_set() {
  # used for setting and showing the value of an option
  context=$1
  option=$2
  value=$3

  case $# in
    2) # fetching
        __pgop_option_value ${context} ${option}
      ;;
    3) # setting
        __pgop_option_replace                                 \
          "${context}"                                        \
          "${option}"                                         \
          "$(__pgop_option_description ${context} ${option})" \
          "${value}"
        return $?
      ;;
    *) pgop_error "need a context and an option" && return 1
  esac
}

pgop_read() {
  context=$1
  shift
  options=$@
  found=1
  case $# in
    0) # when no option is asked for
      __pgop_option_read ${context}
      ;;
    *) # when options are asked for
      for option in ${options}; do
        __pgop_option_read                                \
          ${context}                                      \
          $(__pgop_option_position ${context} ${option})
        [[ $? -eq 0 ]] && found=0
      done
      ;;
  esac
  return ${found}
}

# vim: ft=sh
