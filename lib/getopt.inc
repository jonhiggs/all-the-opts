source $(dirname ${BASH_SOURCE[0]})/getopt.bash
source $(dirname ${BASH_SOURCE[0]})/public.inc

__pgop_getopt_short() {
  context=$1
  options=()

  IFS=$'\n'
  for data in $(pgop_read ${context}); do
    options+="$(__pgop_option_read_short ${data})"
  done
  echo ${options[@]}
}

__pgop_getopt_long() {
  context=$1
  options=()

  IFS=$'\n'
  for data in $(pgop_read ${context}); do
    options+="$(__pgop_option_read_long ${data}),"
  done
  echo ${options[@]} | sed 's/\,$//g'
}

__pgop_getopt_cmd() {
  context=$1
  short="$(__pgop_getopt_short ${context})"
  long="$(__pgop_getopt_long ${context})"
  operands=$(pgop_operands)
  echo "getopt -o ${short} --long ${long} -- \$@"
}

__pgop_getopt_set_operands() {
  local operands=false
  for a in $@; do
    [[ "$a" == "--" ]] && local operands=true && continue
    if $operands; then
      pgop_operands $a
    fi
  done
}

__pgop_getopt_set_option_arguments() {
  context=$1
  shift

  for a in $@; do
    [[ "$a" == "--" ]] && return 0

    if [[ "$a" =~ ^- ]]; then
      option="$(echo $a | sed -E 's/^[\-]{1,2}//')"
      pgop_set ${context} ${option} "true"
    else
      pgop_set ${context} ${option} "$a"
    fi
  done
}

# vim: ft=sh
