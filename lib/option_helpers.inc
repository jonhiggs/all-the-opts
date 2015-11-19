__pgop_option_var() {
  # return the variable name for a context
  context=$1
  echo "PGOP_$(echo $1 | tr '[a-z]' '[A-Z]')"
}

__pgop_option_var_size() {
  # return the variable name for a context
  context=$1
  var="PGOP_$(echo $1 | tr '[a-z]' '[A-Z]')"
  eval "echo \"\${#${var}[@]}\""
}

__pgop_option_position() {
  context=$1
  option=$2
  n=0
  var="$(__pgop_option_var ${context})[@]"
  IFS=$'\n'
  for data in ${!var}; do
    short="$(__pgop_option_read_short "${data}")"
    long="$(__pgop_option_read_long "${data}")"
    if [[ "${short}" == "${option}" ]] || [[ "${long}" == "${option}" ]]; then
      echo $n
      return 0
    fi
    n=$(( $n + 1 ))
  done
  return 1
}


# vim: ft=sh
