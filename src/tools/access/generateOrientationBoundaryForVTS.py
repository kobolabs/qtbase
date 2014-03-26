# -*- encoding: utf-8 -*-

# usage:
# python generate_boundary_array data_csv

import sys

#IN_FILENAME = 'VerticalTypesettingSynthesisSpec.csv'
#OUT_FILENAME = 'orientation.csv'
ORIENTATION_UPRIGHT = 'U'
ORIENTATION_SIDEWAYS = 'R'
DEFAULT_ORIENTATION = 'U'  # for lost, omitted code points
ORIENTATION_STARTS_FROM = 'R'

NUM_OF_HEAD = 1

def _reconstruct_orientation_array(normalized, boundaries):
  c = 1
  o = ORIENTATION_STARTS_FROM
  ret = [[0,o]]
  prev = 1
  for i in range(1, len(boundaries)):
    for j in range(prev, boundaries[i]):
      ret.append([c, o])
      c+=1
    if o == ORIENTATION_UPRIGHT:
      o = ORIENTATION_SIDEWAYS
    else:
      o = ORIENTATION_UPRIGHT
    prev = boundaries[i]
  return ret

def _check1(normalized, boundaries):
  reconstructed = _reconstruct_orientation_array(normalized, boundaries)
  for i in range(0, len(reconstructed)):
    #print str(reconstructed[i][0]) + ":" + str(int(normalized[i][0], 16)) + "," + str(reconstructed[i][1]) + ":" +  str(normalized[i][1])
    if reconstructed[i][0] != int(normalized[i][0], 16) or reconstructed[i][1] != normalized[i][1]:
      return False
  return True

def _check2(raw, boundaries):
  for line in raw:
    #print line[0] + " : " + lookup_orientation(int(line[0], 16), boundaries) + "," +  line[1]
    if lookup_orientation(int(line[0], 16), boundaries) != line[1]:
      return False
  return True

def _search(c, boundaries):
  start = 0
  end = len(boundaries) - 1
  if c > boundaries[end]:
    return end
  while end - start > 1:
    middle = (start + end) / 2
    if c < boundaries[middle]:
      end = middle
    else:
      start = middle
  return start

def test(raw_table, normalized_table, boundaries_table):
  print "\nchecking results ... Phase1",
  if _check1(normalized, boundaries):
    print "passed."
  else:
    print "failed."
    return False
  print "\nchecking results ... Phase2",
  if _check2(raw_table, boundaries_table):
    print "passed."
  else:
    print "failed."
    return False
  return True
 
def lookup_orientation(c, boundaries):
  sideways = 0
  upright = 1
  if ORIENTATION_STARTS_FROM == ORIENTATION_UPRIGHT:
    upright = 0 
    sideways = 1 
  #print _search(c, boundaries)
  if _search(c, boundaries) % 2 == upright:
    return ORIENTATION_UPRIGHT
  else:
    return ORIENTATION_SIDEWAYS

def generate_boundaries(table):
  o = table[0][1]
  c = 0
  ret = [0]
  for line in table:
    if line[1] != o:
      o = line[1]
      ret.append(c)
    c += 1
  ret.append(c)
  return ret

def normalize(table):  # we assume that the codepoints are sorted
  prev = table[0]      #["-1", ORIENTATION_STARTS_FROM]
  ret = []
  for line in table:
    line[0] = line[0].replace("U+", "0x", 1)  # delete code point prefix
    lack = int(prev[0], 16) + 1
    while(int(line[0], 16) - lack > 0):
      ret.append( [str(hex(lack)), DEFAULT_ORIENTATION] )
      lack += 1
    ret.append(line)
    prev = line
  return ret

def check_orientation_format(s):
  return s == ORIENTATION_UPRIGHT or s == ORIENTATION_SIDEWAYS

def read_csv(fname):
  csvf = open(fname, 'r')
  ret = []
  c = 0
  for line in csvf:
    line = line.rstrip()
    l = line.split(',')
    if not check_orientation_format(l[1]) and c >= NUM_OF_HEAD:
      print "incorrect orientation indicator " + l[0] + ", " + l[1] + ". We assume the orientation " + DEFAULT_ORIENTATION
      l[1] = DEFAULT_ORIENTATION
    ret.append( [l[0], l[1]] ) #make list of touple
    c += 1
  return ret[NUM_OF_HEAD:]

def print_array(fo, arr, separate_by):
  for i in arr:
    if i != 0:
      fo.write(separate_by)
    fo.write(hex(i))

argvs = sys.argv
argc = len(argvs)
if argc != 3:
  print 'Usage : python %s filename_of_input_csv filename_of_output_csv' % argvs[0]
  quit()
IN_FILENAME = argvs[1]
OUT_FILENAME = argvs[2]

print "reading csv ... "
rawtable = read_csv(IN_FILENAME)
print "normalizing data ..."
normalized = normalize(rawtable)
print "generating boundaries ..."

boundaries = generate_boundaries(normalized)
#print boundaries

if test(rawtable, normalized, boundaries):
  print "array length is " + str(len(boundaries))
  print "now output array"
  fo = open(OUT_FILENAME, "w")
  #fo = sys.stdout
  print_array(fo, boundaries, ",")
  fo.close()
else:
  print "aborting."

