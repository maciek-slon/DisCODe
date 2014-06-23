#! /usr/bin/env python

import os
import re
import sys
import argparse
import subprocess

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

def getdeps(dcl_name):
  from discode_helper import getDclDir
  
  opened = []
  closed = []
  
  ret = {}
  
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
      
        
  return ret
  
def buildDCL(dclname, clean):
  from discode_helper import getDclDir
  
  dcldir = getDclDir(dclname)
  builddir = os.path.join(dcldir, "build")
  if not os.path.isdir(builddir):
    os.makedirs(builddir)
    subprocess.Popen("cmake ..", cwd = builddir, shell=True).wait()
    
  if clean:
    print "Cleaning previous build..."
    subprocess.Popen('make clean', cwd = builddir, shell = True).wait()
    
  perc = 0
  p = subprocess.Popen('make', cwd = builddir, shell = True, stdout=subprocess.PIPE)
  while p.poll() is None:
    l = p.stdout.readline() # This blocks until it receives a newline.
    match = re.search('^\[[\s]*(.*)\%\]', l)
    if match:
      cur = int(match.group(1))
      perc = max(cur, perc)
      progbar(perc, 50)
  print ""
      
    
  print "Installing..."
  p = subprocess.Popen('make install/fast', cwd = builddir, shell = True, stdout=subprocess.PIPE)
  while p.poll() is None:
    l = p.stdout.readline() # This blocks until it receives a newline.
  
if __name__ == "__main__":
  parser = argparse.ArgumentParser()
  parser.add_argument("DCL", help="name of DCL to be built")
  parser.add_argument("-c", "--clean", help="clean install", action="store_true")
  
  args = parser.parse_args()
  
  deps = getdeps(args.DCL)
  depslist = []
  
  if len(deps) > 0:
    sorteddeps = toposort2(deps)
    for depset in sorteddeps:
      for dep in depset:
        depslist.append(dep)
  else:
    depslist.append(args.DCL)
    
  for dep in depslist:
    print ""
    print "========================================================="
    print "== Building " + dep
    print "========================================================="
    buildDCL(dep, args.clean)
