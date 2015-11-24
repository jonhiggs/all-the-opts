__ato_option_var() {
  # return the variable name for a context
  context=$1
  echo "ATO_$(echo $1 | tr '[a-z]' '[A-Z]')"
}

__ato_option_var_size() {
  # return the variable name for a context
  context=$1
  var="ATO_$(echo $1 | tr '[a-z]' '[A-Z]')"
  eval "echo \"\${#${var}[@]}\""
}

__ato_option_position() {
  context=$1
  option=$2
  n=0
  var="$(__ato_option_var ${context})[@]"
  IFS=$'\n'
  for data in ${!var}; do
    short="$(__ato_option_read_short "${data}" | tr -d ':')"
    long="$(__ato_option_read_long "${data}" | tr -d ':')"
    if [[ "${short}" == "${option}" ]] || [[ "${long}" == "${option}" ]]; then
      echo $n
      return 0
    fi
    n=$(( $n + 1 ))
  done
  return 1
}


# vim: ft=sh
