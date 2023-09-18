import FWCore.ParameterSet.Config as cms
from PhysicsTools.NanoAOD.common_cff import Var
from PhysicsTools.NanoAOD.common_cff import *

def add_dSAmuons(process):
  process.customizeddSAmuonsTask = cms.Task()
  process.schedule.associate(process.customizeddSAmuonsTask)

  dSAmuonsInput = cms.InputTag("displacedStandAloneMuons")

  process.customdSAmuonsConstituentsTable = cms.EDProducer("SimpleTrackFlatTableProducer",
    src = dSAmuonsInput,
    cut = cms.string(""), #we should not filter after pruning
    name = cms.string("displacedStandAloneMuons"),
    doc = cms.string("displacedStandAloneMuons information"),
    singleton = cms.bool(False), # the number of entries is variable
    extension = cms.bool(False), # this is the extension table for the AK8 constituents
    variables = cms.PSet(
      pt = Var("pt()", float),
      ptErr = Var("ptError()", float),
      eta = Var("eta()", float),
      etaErr = Var("etaError()", float),
      charge = Var("charge()", int),
      phi = Var("phi()", float),
      phiErr = Var("phiError()", float),
      vx = Var("vx()", float),
      vy = Var("vy()", float),
      vz = Var("vz()", float),
      chi2 = Var("chi2()", float),
      ndof = Var("ndof()", float),
    )
  )
  process.customizeddSATask.add(process.customdSAmuonsConstituentsTable)
  return process
  

def ttalps_customizeMC(process):
  add_dSAmuons(process)
  return process
