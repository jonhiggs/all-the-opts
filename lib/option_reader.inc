source $(dirname ${BASH_SOURCE[0]})/option_helpers.inc

__pgop_option_read() {
  context=$1
  position=$2
  case $# in
    1) # grab all the options
      var="$(__pgop_option_var ${context})[@]"
      IFS=$'\n'
      for line in "${!var}"; do
        echo $line
      done
      ;;
    2) # cherry-pick specific option
      var="$(__pgop_option_var ${context})[${position}]"
      [[ -z ${!var} ]] && return 1
      echo ${!var}
    ;;
  esac
}

# vim: ft=sh
