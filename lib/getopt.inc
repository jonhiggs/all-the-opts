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

#__pgop_getopt_longargs() {
#}

# vim: ft=sh
