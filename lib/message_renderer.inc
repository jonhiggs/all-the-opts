__ato_error() {
  message=$(echo $@ | sed -E 's/[\ ]+/ /g')
  echo "ERROR: $message" 1>&2
  return 0
}

__ato_usage() {
  cmd="${FUNCNAME[1]}"
  echo "USAGE: ${cmd} $@" > /dev/stderr
  return 0
}

# vim: ft=sh
