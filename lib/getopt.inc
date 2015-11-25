source $(dirname ${BASH_SOURCE[0]})/getopt.bash
source $(dirname ${BASH_SOURCE[0]})/public.inc

__ato_getopt_short() {
  context=$1
  options=()

  IFS=$'\n'
  for data in $(__ato_option_read ${context}); do
    options+="$(__ato_option_read_short ${data})"
  done
  echo ${options[@]}
}

__ato_getopt_long() {
  context=$1
  options=()

  IFS=$'\n'
  for data in $(__ato_option_read ${context}); do
    options+="$(__ato_option_read_long ${data}),"
  done
  echo ${options[@]} | sed 's/\,$//g'
}

__ato_getopt_cmd() {
  context=$1
  short="$(__ato_getopt_short ${context})"
  long="$(__ato_getopt_long ${context})"
  operands=$(ato_operands)
  echo "getopt -o ${short} --long ${long} -- \$@"
}

__ato_getopt_set_operands() {
  local operands=false
  for a in $@; do
    [[ "$a" == "--" ]] && local operands=true && continue
    if $operands; then
      ato_operands $a
    fi
  done
}

__ato_getopt_set_option_arguments() {
  context=$1
  shift

  for a in $@; do
    [[ "$a" == "--" ]] && return 0

    if [[ "$a" =~ ^- ]]; then
      option="$(echo $a | sed -E 's/^[\-]{1,2}//')"
      ato_set ${option} "true"
    else
      ato_set ${option} "$a"
    fi
  done
}

# vim: ft=sh
