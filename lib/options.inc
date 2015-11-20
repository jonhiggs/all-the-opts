# data should be a space delimited string with the following fields:
# - short_option
# - long_option
# - description (base64 encoded)
# - value (base64 encoded or '-' for null)

source $(dirname ${BASH_SOURCE[0]})/option_helpers.inc

__pgop_option_read() {
  context=$1
  position=$2
  found=1
  case $# in
    1) # grab all the options
      var="$(__pgop_option_var ${context})[@]"
      IFS=$'\n'
      for line in "${!var}"; do
        echo $line
        found=0
      done
      ;;
    2) # cherry-pick specific option
      var="$(__pgop_option_var ${context})[${position}]"
      [[ -z ${!var} ]] && return 1
      echo ${!var}
      found=0
    ;;
  esac
  return $found
}

__pgop_option_write() {
  context=$1
  data=$2
  var="$(__pgop_option_var ${context})"

  if [[ -z ${3} ]]; then
    position="$(__pgop_option_var_size ${context})"
  fi

  save_point="$(__pgop_option_var ${context})[${position}]"
  eval "$(echo ${save_point})='${data}'"
  return 0
}

__pgop_option_read_short() {
  echo $1 | awk '{ print $1 }'
}

__pgop_option_read_long() {
  echo $1 | awk '{ print $2 }'
}

__pgop_option_read_description() {
  echo $1 | awk '{ print $3 }' | base64 -D
}

__pgop_option_read_optarg() {
  data="$(echo $1 | awk '{ print $4 }')"
  case "${data}" in
    '-')    return 1                              ;;
    "true") return 0                              ;;
    *)      echo ${data} | base64 -D && return 0  ;;
  esac
}

# vim: ft=sh
