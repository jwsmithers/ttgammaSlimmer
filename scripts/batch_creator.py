channel="ejets"
path="/eos/atlas/user/j/jwsmith/reprocessedNtuples/v009_flattened/CR1/"+channel
nn_file="model4_100_singlelepton_ELD.json"


print """
executable            = /afs/cern.ch/work/j/jwsmith/github/ttgammaSlimmer/build_batch/"""+channel+"""/ttgammaSlimmer
transfer_input_files  = """ + nn_file +  """
getenv                = True
JobBatchName	      = """ + channel + """
+JobFlavour           = "testmatch"
"""

from os import listdir
from os.path import isfile, join
ntuples = [f for f in listdir(path) if isfile(join(path, f))]

for i in ntuples:
  if "ttgamma" in i:
    print "output = output/$(ClusterId).$(ProcId)."+i.rstrip(".root")+".out"
    print "error = error/$(ClusterId).$(ProcId)."+i.rstrip(".root")+".err"
    print "log = log/$(ClusterId).$(ProcId)."+i.rstrip(".root")+".log"
    print "arguments = "+i
    print "queue"
    print "\n"
