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

# vim: ft=sh
