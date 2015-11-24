source $(dirname ${BASH_SOURCE[0]})/message_renderer.inc
source $(dirname ${BASH_SOURCE[0]})/options.inc

ATO_CONTEXT=("global")
ATO_OPERANDS=()

ato_description() {
  if [[ -z $1 ]] && [[ ! -z ${ATO_DESCRIPTION} ]]; then
    echo ${ATO_DESCRIPTION}
  else
    export ATO_DESCRIPTION="$@"
  fi
}

ato_add() {
  if [[ $# -ne 3 ]] && [[ $# -ne 4 ]]; then
    __ato_usage "<context> <short_opt> <long_opt> <desription> [option_argument]"
    return 1
  fi

  context=$1
  short=$2
  long=$3
  description=$4
  optarg=${5:-"-"}

  for t in short long; do
    if ( __ato_option_read ${context} ${!t} &> /dev/null ); then
      __ato_error "${t} option is not unique"
      return 1
    fi
  done

  # base64 encode the description and optarg
  [[ "${description}" == '-' ]] || description="$(echo ${description} | base64)"
  [[ "${optarg}" == '-' ]]      || optarg="$(echo ${optarg} | base64)"

  data="${short} ${long} ${description} ${optarg}"
  __ato_option_write "${context}" "${data}"
  return $?
}

ato_context() {
  [[ -z ${ATO_CONTEXT} ]] && ATO_CONTEXT="global"
  if [[ -z $1 ]]; then
    echo "${ATO_CONTEXT}"
  else
    export ATO_CONTEXT="$1"
  fi
}

ato_operands() {
  # show the current operands Add any arguments into the list.
  case $# in
    0)
      for operand in ${ATO_OPERANDS[@]}; do
        echo ${operand}
      done
      ;;
    *)
      for operand in $@; do
        ATO_OPERANDS+=($operand)
      done
      ;;
  esac
}

ato_set() {
  # used for setting optarg
  [[ $# -ne 2 ]] && __ato_usage "<option> <option_argument>" && return 1

  context="$(ato_context)"
  option=$1

  __ato_option_position ${context} ${option} ||
    __ato_error "option '${option}' does not exist" && return 1

  case $2 in
    "true")  optarg="true" ;;
    "false") optarg="-"    ;;
    *)       optarg="$(echo $2 | base64)"
  esac

  position=$(__ato_option_position ${context} ${option})
  data=$(__ato_option_read ${context} ${position} | awk '{$4="'${optarg}'"}1')
  [[ $? -ne 0 ]] && __ato_error "option '${option}' does not exist" && return 1
  __ato_option_write "${context}" "${data}" "${position}"
  return $?
}

ato_read() {
  [[ $# -eq 0 ]] && __ato_usage "<option>" && return 1
  context="$(ato_context)"
  option=$1
  position=$(__ato_option_position ${context} ${option})
  [[ $? -ne 0 ]] && __ato_error "option '${option}' is invalid" && return 1
  data=$(__ato_option_read ${context} ${position})
  [[ $? -ne 1 ]] && return 1
  option_argument="$(__ato_option_read_optarg "${data}")"
  case ${option_argument} in
    "true") return 0                  ;;
    *)      echo "${option_argument}" ;;
  esac
}

ato_help() {
  context="$(ato_context)"
  ato_description
  IFS=$'\n'
  for data in $(__ato_option_read ${context}); do
    short="-$(__ato_option_read_short "${data}")"
    long="--$(__ato_option_read_long "${data}")"
    description="$(__ato_option_read_description "${data}")"

    if [[ ${#long} -gt 8 ]]; then
      description="\t${description}"
    else
      description="\t\t${description}"
    fi

    echo -e "   ${short} ${long} ${description}"
  done
}

ato_setup() {
  context="$(ato_context)"
  argv="$(eval $(__ato_getopt_cmd))"
  eval "set -- $argv"
  __ato_getopt_set_operands $@
  __ato_getopt_set_option_arguments ${context} $@

  if ato_argument ${context} help; then
    ato_help
    return 1
  fi
}

ato_argument() {
  context="$(ato_context)"
  option=$1
  data="$(__ato_option_read ${context} ${option})"
  value="$(__ato_option_read_optarg "${data}")"
  [[ $? -ne 0 ]] && return 1
  case "${value}" in
    "true") return 0 ;;
    "-")    return 1 ;;
    *)      [[ ! -z ${value} ]] && echo ${value}; return 0 ;;
  esac
}

ato_options() {
  context="$(ato_context)"
  var="$(__ato_option_var ${context})[@]"

  IFS=$'\n'
  for data in $(__ato_option_read ${context}); do
    if __ato_option_read_long "${data}" &> /dev/null; then
      echo "--$(__ato_option_read_long "${data}")"
      continue
    fi

    if __ato_option_read_short "${data}" &> /dev/null; then
      echo "-$(__ato_option_read_short "${data}")"
      continue
    fi
  done
}

# vim: ft=sh
