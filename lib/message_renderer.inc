pgop_error() {
  message=$(echo $@ | sed -E 's/[\ ]+/ /g')
  echo "Error: $message" 1>&2
  return 0
}

# vim: ft=sh
