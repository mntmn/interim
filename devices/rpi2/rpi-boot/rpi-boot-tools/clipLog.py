#!/usr/bin/env python

# Invoke as ./clipLog.py < input > output

import sys

while 1:
	b = sys.stdin.read(1)
	if b == '\xff':
		break
	sys.stdout.write(b)

