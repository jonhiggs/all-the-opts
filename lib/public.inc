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
    __ato_usage "<short_opt> <long_opt> <desription> [option_argument]"
    return 1
  fi

  context="$(ato_context)"
  short=$1
  long=$2
  description=$3
  optarg=${4:-"-"}

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

  if ! __ato_option_exists ${context} ${option}; then
    __ato_error "option '${option}' does not exist"
    return 1
  fi

  case $2 in
    "true")  optarg="true" ;;
    "false") optarg="-"    ;;
    *)       optarg="$(echo $2 | base64)"
  esac


  position=$(__ato_option_position ${context} ${option})
  data=$(__ato_option_read ${context} ${position})
  new_data=$(echo ${data} | awk -v r="${optarg}" '{$4=r}1')
  __ato_option_write "${context}" "${new_data}" "${position}"
  return $?
}

ato_read() {
  [[ $# -eq 0 ]] && __ato_usage "<option>" && return 1
  context="$(ato_context)"
  option=$1
  position=$(__ato_option_position ${context} ${option})
  if [[ $? -ne 0 ]]; then
    __ato_error "option '${option}' is invalid"
    return 1
  fi
  data=$(__ato_option_read ${context} ${position} || return 1)
  option_argument="$(__ato_option_read_optarg "${data}")"

  case ${option_argument} in
    "true")  return 0                              ;;
    "false") return 1                              ;;
    *)       echo "${option_argument}" | base64 -D ;;
  esac
  return 0
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
  argv="$(eval $(__ato_getopt_cmd ${context}))"
  eval "set -- $argv"
  __ato_getopt_set_operands $@
  __ato_getopt_set_option_arguments ${context} $@
}

ato_argument() {
  [[ $# -ne 1 ]] && __ato_usage "<option>"
  context="$(ato_context)"
  position=$(__ato_option_position ${context} $1)
  data="$(__ato_option_read ${context} ${position})"
  value="$(__ato_option_read_optarg "${data}")"
  case "${value}" in
    "true")  return 0                   ;;
    "false") return 1                   ;;
    *)       echo ${value} | base64 -D  ;;
  esac
}

ato_options() {
  context="$(ato_context)"
  var="$(__ato_option_var ${context})[@]"

  # TODO: refactor this into __ato_options_something
  IFS=$'\n'
  for data in $(__ato_option_read ${context}); do
    optarg="$(__ato_option_read_optarg "${data}")"
    [[ "${optarg}" == "false" ]] && continue
    __ato_option_read_long "${data}" || __ato_option_read_short "${data}"
  done
}

# vim: ft=sh
