# data should be a space delimited string with the following fields:
# - short_option
# - long_option
# - description (base64 encoded)
# - value (base64 encoded or '-' for null)

source $(dirname ${BASH_SOURCE[0]})/message_renderer.inc
source $(dirname ${BASH_SOURCE[0]})/option_helpers.inc

__ato_option_read() {
  if [[ ! -z $2 ]] && [[ ! $2 =~ [0-9]+ ]]; then
    __ato_usage "<context> [position]"
    return 1
  fi

  context=$1
  position=$2
  found=1
  case $# in
    1) # grab all the options
      var="$(__ato_option_var ${context})[@]"
      IFS=$'\n'
      for line in "${!var}"; do
        echo $line
        found=0
      done
      ;;
    2) # cherry-pick specific option
      var="$(__ato_option_var ${context})[${position}]"
      [[ -z ${!var} ]] && return 1
      echo ${!var}
      found=0
    ;;
  esac
  return $found
}

__ato_option_write() {
  if [[ $# -ne 2 ]] && [[ $# -ne 3 ]]; then
    __ato_usage "<context> <data> [position]"
    return 1
  fi

  context=$1
  data=$2
  position="${3:-$(__ato_option_var_size ${context})}"
  var="$(__ato_option_var ${context})"

  # save to the correct array element
  save_point="$(__ato_option_var ${context})[${position}]"
  eval "$(echo ${save_point})='${data}'"
  return 0
}

__ato_option_read_short() {
  echo $1 | awk '{ print $1 }'
}

__ato_option_read_long() {
  echo $1 | awk '{ print $2 }'
}

__ato_option_read_description() {
  echo $1 | awk '{ print $3 }' | base64 -D
}

__ato_option_read_optarg() {
  data="$(echo $1 | awk '{ print $4 }')"
  case "${data}" in
    '-')    return 1                              ;;
    "true") return 0                              ;;
    *)      echo ${data} | base64 -D && return 0  ;;
  esac
}

# vim: ft=sh
