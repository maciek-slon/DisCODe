#! /usr/bin/env python

import os
import re
import sys
import argparse
import subprocess
import calendar
import time

def progbar(perc, length):
  str = "\r["
  flen = perc*length/100
  str = str + 'X'*flen + " "*(length-flen)
  str = str + "]"
  sys.stdout.write("%s %3d%%" % (str, perc))
  sys.stdout.flush()

def toposort2(data):
  """Dependencies are expressed as a dictionary whose keys are items
and whose values are a set of dependent items. Output is a list of
sets in topological order. The first set consists of items with no
dependences, each subsequent set consists of items that depend upon
items in the preceeding sets.

>>> print '\\n'.join(repr(sorted(x)) for x in toposort2({
...     2: set([11]),
...     9: set([11,8]),
...     10: set([11,3]),
...     11: set([7,5]),
...     8: set([7,3]),
...     }) )
[3, 5, 7]
[8, 11]
[2, 9, 10]

"""
  from functools import reduce

  # Ignore self dependencies.
  for k, v in data.items():
    v.discard(k)
  # Find all items that don't depend on anything.
  extra_items_in_deps = reduce(set.union, data.itervalues()) - set(data.iterkeys())
  # Add empty dependences where needed
  data.update({item:set() for item in extra_items_in_deps})
  while True:
    ordered = set(item for item, dep in data.iteritems() if not dep)
    if not ordered:
      break
    yield ordered
    data = {item: (dep - ordered)
      for item, dep in data.iteritems()
        if item not in ordered}
  assert not data, "Cyclic dependencies exist among these items:\n%s" % '\n'.join(repr(x) for x in data.iteritems())

def runtimedeps(dcl_name):
  from discode_helper import getDclDir
  dcldir = getDclDir(dcl_name)

  depset = []
  for fn in os.listdir(os.path.join(dcldir, "tasks")):
    if fn.endswith(".xml"): 
        #print fn
        with open(os.path.join(dcldir, "tasks", fn)) as f:
          for line in f:
            match = re.search('<.*Component.*type="(.*):.*"', line)
            if (match):
              depdcl = match.group(1)
              if depdcl == dcl_name:
                continue
              if not depdcl in depset:
                depset.append(depdcl)
    else:
        continue

  return depset

def getdeps(dcl_name, runtime):
  from discode_helper import getDclDir
  
  opened = []
  closed = []
  
  ret = {}
  
  dotstr = "digraph G {\n"

  if runtime:
    dotdot = ""
    dotstr = dotstr + dcl_name + "-> {"
    opened = runtimedeps(dcl_name)
    for x in opened:
      dotstr = dotstr + dotdot + x
      dotdot = ", "
    dotstr = dotstr + "} [style=dotted];\n"

  if not dcl_name in opened:
    opened.append(dcl_name)
  
  while len(opened) > 0:
    dcl = opened.pop()
    dcldir = getDclDir(dcl)
    if dcldir == "":
      print "DCL " + dcl_name + " not found! Aborting build..."
      sys.exit(-1)
        
    depset = set([])
    with open(os.path.join(dcldir, "src", "CMakeLists.txt")) as f:
      for line in f:
        match = re.search('^[\s]*DISCODE_FIND_DCL\((.*)\)', line)
        if (match):
          depdcl = match.group(1)
          depset.add(depdcl)
          if not ((depdcl in closed) or (depdcl in opened)):
            opened.append(depdcl)
           
    if len(depset) > 0:
      ret[dcl] = depset
      
    closed.append(dcl)
      
    dotstr = dotstr + dcl + " -> {"
    dotdot = ""
    for x in depset:
      dotstr = dotstr + dotdot + x
      dotdot = ", "
    dotstr = dotstr + "}\n"
        
  dotstr = dotstr + "}"
  print dotstr

  return ret
  
def buildDCL(dclname, args):
  from discode_helper import getDclDir
  
  dcldir = getDclDir(dclname)
  builddir = os.path.join(dcldir, "build")
  if not os.path.isdir(builddir):
    os.makedirs(builddir)
    subprocess.Popen("cmake ..", cwd = builddir, shell=True).wait()
    
  if args.release:
    subprocess.Popen("cmake . -DCMAKE_BUILD_TYPE=Release", cwd = builddir, shell=True).wait()

  if args.clean:
    print "Cleaning previous build..."
    subprocess.Popen('make clean', cwd = builddir, shell = True).wait()

  errs = ""
  perc = 0
  tempname='/tmp/dclmake_build_error_log_' + str(calendar.timegm(time.gmtime()))
  log_file = open(tempname, 'w')
  print "Building..."
  p = subprocess.Popen('make', cwd = builddir, shell = True, stdout=subprocess.PIPE, stderr=log_file, bufsize=-1)
  while p.poll() is None:
    l = p.stdout.readline() # This blocks until it receives a newline.
    match = re.search('^\[[\s]*(.*)\%\]', l)
    if match:
      cur = int(match.group(1))
      perc = max(cur, perc)
      progbar(perc, 50)
  log_file.close()
  print ""
 
  log_file = open(tempname, 'r')
  if p.returncode != 0:
    print "Errors during build:"
    print log_file.read()
    return False
    
  if log_file.readline() != "":
    print "Warnings:"
    print log_file.read()
      
    
  print "Installing..."
  p = subprocess.Popen('make install/fast', cwd = builddir, shell = True, stdout=subprocess.PIPE)
  while p.poll() is None:
    l = p.stdout.readline() # This blocks until it receives a newline.
    
  return True
  
if __name__ == "__main__":
  parser = argparse.ArgumentParser()
  parser.add_argument("DCL", help="name of DCL to be built")
  parser.add_argument("-c", "--clean", help="clean install", action="store_true")
  parser.add_argument("-s", "--show-deps", help="only show dependencies without building", action="store_true")
  parser.add_argument("-r", "--runtime", help="check also runtime dependencies", action="store_true")
  parser.add_argument("--release", help="build release version", action="store_true")
  
  args = parser.parse_args()
  
  deps = getdeps(args.DCL, args.runtime)
  depslist = []
  
  if args.runtime:
    print runtimedeps(args.DCL)

  if len(deps) > 0:
    sorteddeps = toposort2(deps)
    for depset in sorteddeps:
      for dep in depset:
        depslist.append(dep)
  else:
    depslist.append(args.DCL)

  if args.show_deps:
    print "DCL in dependency order:"
    for d in depslist:
      print "\t", d
    sys.exit(0)
    
  for dep in depslist:
    print ""
    print "========================================================="
    print "== Building " + dep
    print "========================================================="
    if not buildDCL(dep, args):
      print "Build failed!"
      break
