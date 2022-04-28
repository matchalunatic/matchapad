#!/usr/bin/env python

import sys
import re
regex_float_number = re.compile('([0-9]+[.][0-9]+)')
for l in sys.stdin.readlines():
  mch = regex_float_number.findall(l)
  if mch is None:
    print(l.rstrip())
    continue
  nl = l
  for m in mch:
    n = float(m) * 10**20
    nl = nl.replace(m, str(int(n)))
  print(nl.rstrip())
