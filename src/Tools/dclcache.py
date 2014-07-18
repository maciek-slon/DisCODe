#!/usr/bin/env python

import sys
import os
import discode_helper as dh

from ctypes import *

def refreshDCLCache(dclname, dclpath):
  mappings = {}
  p = os.path.join(dclpath, "dist", "lib")
  outfile = os.path.join(dclpath, "dist", "cache.txt")
  f = open(outfile, 'w')
  for libname in os.listdir(p):
    try:
      lib = CDLL(os.path.join(p, libname))
    except:
 #     print "Can't open", libname
      continue
      
    try:
      lib.returnCName.restype = c_char_p
      cname = lib.returnCName()
      mappings[cname] = libname
      f.write(cname + "\t" + libname + "\n")
    except:
#      print "Can't read", libname, "name"
      continue
  
#  print mappings
  f.close()

def main(argv):
  dcls = dh.listDCL()
  if len(argv)>1 and argv[1] in dcls.keys():
    refreshDCLCache(argv[1], dcls[argv[1]])
  else:
    for dcl in dcls.keys():
      refreshDCLCache(dcl, dcls[dcl])

if __name__=="__main__":
  main(sys.argv)
