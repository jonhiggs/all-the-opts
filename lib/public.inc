source $(dirname ${BASH_SOURCE[0]})/message_renderer.inc
source $(dirname ${BASH_SOURCE[0]})/options.inc

PGOP_CONTEXT=("global")
PGOP_OPERANDS=()

pgop_description() {
  if [[ -z $1 ]]; then
    echo ${PGOP_DESCRIPTION}
  else
    export PGOP_DESCRIPTION="$@"
  fi
}

pgop_add() {
  if [[ $# -ne 4 ]] && [[ $# -ne 5 ]]; then
    # FIXME: make it a proper usage
    pgop_error "you must provide context, short option, long option,
      description and optionally an optarg too"
    return 1
  fi

  context=$1
  short=$2
  long=$3
  description=$4
  optarg=${5:-"-"}

  for t in short long; do
    if ( pgop_read ${context} ${!t} &> /dev/null ); then
      pgop_error "${t} option is not unique"
      return 1
    fi
  done

  # base64 encode the description and optarg
  [[ "${description}" == '-' ]] || description="$(echo ${description} | base64)"
  [[ "${optarg}" == '-' ]]      || optarg="$(echo ${optarg} | base64)"

  data="${short} ${long} ${description} ${optarg}"
  __pgop_option_write "${context}" "${data}"
  return $?
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

pgop_operands() {
  # show the current operands Add any arguments into the list.
  case $# in
    0)
      for operand in ${PGOP_OPERANDS[@]}; do
        echo ${operand}
      done
      ;;
    *)
      for operand in $@; do
        PGOP_OPERANDS+=($operand)
      done
      ;;
  esac
}

pgop_set() {
  # used for setting optarg
  [[ $# -ne 3 ]] && pgop_error "need a context, option and argument" && return 1

  context=$1
  option=$2

  case $3 in
    "true")  optarg="true" ;;
    "false") optarg="-"    ;;
    *)       optarg="$(echo $3 | base64)"
  esac

  data=$(pgop_read $context ${option} | awk '{$4="'${optarg}'"}1')
  position=$(__pgop_option_position ${context} ${option})
  [[ $? -ne 0 ]] && pgop_error "option '${option}' does not exist" && return 1
  __pgop_option_write "${context}" "${data}" "${position}"
  return $?
}

pgop_read() {
  context=${1:-"global"}
  shift
  options=$@
  found=1
  case $# in
    0) # when no option is asked for
      __pgop_option_read ${context}
      ;;
    *) # when options are asked for
      for option in ${options}; do
        position="$(__pgop_option_position ${context} ${option})"
        if [[ $? -eq 0 ]]; then
          __pgop_option_read ${context} ${position}
          found=0
        fi
      done
      ;;
  esac
  return ${found}
}

pgop_help() {
  context=$1
  pgop_description
  IFS=$'\n'
  for data in $(pgop_read ${context}); do
    short="-$(__pgop_option_read_short "${data}")"
    long="--$(__pgop_option_read_long "${data}")"
    description="$(__pgop_option_read_description "${data}")"

    if [[ ${#long} -gt 8 ]]; then
      description="\t${description}"
    else
      description="\t\t${description}"
    fi

    echo -e "   ${short} ${long} ${description}"
  done
}

pgop_setup() {
  context=$1
  shift
  argv="$(eval $(__pgop_getopt_cmd))"
  eval "set -- $argv"
  __pgop_getopt_set_operands $@
  __pgop_getopt_set_option_arguments ${context} $@
}

pgop_argument() {
  context=$1
  option=$2
  data="$(pgop_read ${context} ${option})"
  value="$(__pgop_option_read_optarg "${data}")"
  [[ $? -ne 0 ]] && return 1
  case "${value}" in
    "true") return 0 ;;
    "-")    return 1 ;;
    *)      [[ ! -z ${value} ]] && echo ${value}; return 0 ;;
  esac
}

# vim: ft=sh
