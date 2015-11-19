# data should be a space delimited string with the following fields:
# - short_option
# - long_option
# - description (base64 encoded)
# - value (base64 encoded or '-' for null)

source $(dirname ${BASH_SOURCE[0]})/option_helpers.inc

__pgop_option_save() {
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

# vim: ft=sh
