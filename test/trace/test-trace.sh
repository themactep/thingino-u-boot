# Copyright (c) 2013 The Chromium OS Authors.
#
# SPDX-License-Identifier:	GPL-2.0+

# Simple test script for tracing with sandbox

OUTPUT_DIR=sandbox
TRACE_OPT="FTRACE=1"

fail() {
	echo "Test failed: $1"
	if [ -n ${tmp} ]; then
		rm ${tmp}
	fi
	exit 1
}

build_uboot() {
	echo "Build sandbox"
	OPTS="O=${OUTPUT_DIR} ${TRACE_OPT}"
	NUM_CPUS=$(grep -c processor /proc/cpuinfo)
	make ${OPTS} sandbox_config
	make ${OPTS} -s -j${NUM_CPUS}
}

run_trace() {
	echo "Run trace"
	./${OUTPUT_DIR}/u-boot <<END
	trace stats
	hash sha256 0 10000
	trace pause
	trace stats
	hash sha256 0 10000
	trace stats
	trace resume
	hash sha256 0 10000
	trace pause
	trace stats
	reset
END
}

check_results() {
	echo "Check results"

	# Expect sha256 to run 3 times, so we see the string 6 times
	if [ $(grep -c sha256 ${tmp}) -ne 6 ]; then
		fail "sha256 error"
	fi

	# 4 sets of results (output of 'trace stats')
	if [ $(grep -c "traced function calls" ${tmp}) -ne 4 ]; then
		fail "trace output error"
	fi

	# Check trace counts. We expect to see an increase in the number of
	# traced function calls between each 'trace stats' command, except
	# between calls 2 and 3, where tracing is paused.
	# This code gets the sign of the difference between each number and
	# its predecessor.
	counts="$(tr -d , <${tmp} | awk '/traced function calls/ { diff = $1 - upto; upto = $1; printf "%d ", diff < 0 ? -1 : (diff > 0 ? 1 : 0)}')"

	if [ "${counts}" != "1 1 0 1 " ]; then
		fail "trace collection error: ${counts}"
	fi
}

echo "Simple trace test / sanity check using sandbox"
echo
tmp="$(tempfile)"
build_uboot
run_trace >${tmp}
check_results ${tmp}
rm ${tmp}
echo "Test passed"
