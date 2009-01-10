define reset 
  set logging overwrite on
  set logging file foo.txt
  set logging on
  info breakpoints
  set logging off
  shell awk '/^[1-9]/ { if ($1 != 1) { print "dis " $1 } }' < foo.txt > bar.txt
  source bar.txt
  set confirm off
  r
  set confirm on
end

define ek
  while $eip < 0x80800000
    stepi
  end
  set logging overwrite on
  set logging file foo.txt
  set logging on
  info breakpoints
  set logging off
  shell awk '/^[1-9]/ { if ($1 != 1) { print "ena " $1 } }' < foo.txt > bar.txt
  source bar.txt
end

define sk
  set pagination off
  while $esp < sigstack || $esp > sigstack + 32768
    stepi
  end
  set pagination on
  stepi
end
