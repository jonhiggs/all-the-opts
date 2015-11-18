source $(dirname ${BASH_SOURCE[0]})/message_renderer.inc
source $(dirname ${BASH_SOURCE[0]})/pure-getopt-plus.inc

pgop_description() {
  if [[ -z $1 ]]; then
    echo ${PGOP_DESCRIPTION}
  else
    export PGOP_DESCRIPTION="$@"
  fi
}

pgop_add() {
  context=$1
  short=$2
  long=$3
  description=$4

  case $# in
    0)
      pgop_error "context must be set"
      return 1
      ;;
    1)
      __pgop_options ${context}
    ;;
    4)
      if ! __pgop_options_uniq ${context} ${short} ${long}; then
        pgop_error "non-unique option"
        return 1
      fi

      var=$(__pgop_context_var $context)
      eval "$(echo ${var})+=('${short} ${long} ${description}')"
    ;;
  esac

}

# vim: ft=sh
