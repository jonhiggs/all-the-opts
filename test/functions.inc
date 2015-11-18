function oneTimeSetUp() {
  source $(dirname $0)/../lib/pure-getopt-plus.inc

  export PGOP_GLOBAL=(
    "f: filters:"
    "-  output"
    "q: query:"
    "b  version"
  )

}

function oneTimeTearDown() {
  echo "one time tear down"
}

function tearDown() {
  echo "tear down"
}

function setUp() {
  echo "set up"
}


# vim: ai ts=2 sw=2 et sts=2 ft=sh
