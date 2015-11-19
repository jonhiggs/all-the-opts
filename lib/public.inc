source $(dirname ${BASH_SOURCE[0]})/message_renderer.inc
source $(dirname ${BASH_SOURCE[0]})/pure-getopt-plus.inc

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
      __pgop_options ${context}
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
  case $# in
    2) __pgop_option ${context} ${option} ;;
    *) pgop_error "need a context and an option" && return 1
  esac
}

# vim: ft=sh
