import FWCore.ParameterSet.Config as cms
import os

from tthAnalysis.HiggsToTauTau.configs.recommendedMEtFilters_cfi import recommendedMEtFilters
from tthAnalysis.HiggsToTauTau.configs.EvtYieldHistManager_2017_cfi import EvtYieldHistManager_2017

process = cms.PSet()

process.fwliteInput = cms.PSet(
    fileNames = cms.vstring(''),
    maxEvents = cms.int32(-1),
    outputEvery = cms.uint32(100000)
)

process.fwliteOutput = cms.PSet(
    fileName = cms.string('analyze_hh_1l_3tau.root')
)

process.analyze_hh_1l_3tau = cms.PSet(
    treeName = cms.string('Events'),

    process = cms.string('HH'),

    histogramDir = cms.string('1l_3tau_OS_Tight'),

    era = cms.string('2017'),

    triggers_1e = cms.vstring("HLT_BIT_HLT_Ele23_WPLoose_Gsf_v"),
    use_triggers_1e = cms.bool(True),
    triggers_1e1tau = cms.vstring(),
    use_triggers_1e1tau = cms.bool(False),
    triggers_1mu = cms.vstring("HLT_BIT_HLT_IsoMu20_v", "HLT_BIT_HLT_IsoTkMu20_v"),
    use_triggers_1mu = cms.bool(True),
    triggers_1mu1tau = cms.vstring(),
    use_triggers_1mu1tau = cms.bool(False),
    triggers_2tau = cms.vstring(),
    use_triggers_2tau = cms.bool(False),

    apply_offline_e_trigger_cuts_1e = cms.bool(True),
    apply_offline_e_trigger_cuts_1e1tau = cms.bool(True),
    apply_offline_e_trigger_cuts_1mu = cms.bool(True),
    apply_offline_e_trigger_cuts_1mu1tau = cms.bool(True),
    apply_offline_e_trigger_cuts_2tau = cms.bool(True),

    electronSelection = cms.string('Tight'),
    muonSelection = cms.string('Tight'),
    lep_mva_cut = cms.double(0.90), # CV: used for tight lepton selection only
    apply_leptonGenMatching = cms.bool(False),

    hadTauSelection = cms.string('Tight|dR03mvaTight'),
    apply_hadTauGenMatching = cms.bool(False),

    chargeSumSelection = cms.string('OS'),
    
    applyFakeRateWeights = cms.string("disabled"), # either "disabled", "4L" or "3tau"
    leptonFakeRateWeight = cms.PSet(
        inputFileName = cms.string("tthAnalysis/HiggsToTauTau/data/FR_lep_ttH_mva_2017_Tallinn_2018May22.root"),
        histogramName_e = cms.string("FR_mva090_el_data_comb"),
        histogramName_mu = cms.string("FR_mva090_mu_data_comb"),
    ),
    hadTauFakeRateWeight = cms.PSet(
        inputFileName = cms.string("tthAnalysis/HiggsToTauTau/data/FR_tau_2017_v2.root"),
        lead = cms.PSet(
            absEtaBins = cms.vdouble(-1., 1.479, 9.9),
            graphName = cms.string("jetToTauFakeRate/$hadTauSelection/$etaBin/jetToTauFakeRate_mc_hadTaus_pt"),
            applyGraph = cms.bool(True),
            fitFunctionName = cms.string("jetToTauFakeRate/$hadTauSelection/$etaBin/fitFunction_data_div_mc_hadTaus_pt"),
            applyFitFunction = cms.bool(True),
        ),
        sublead = cms.PSet(
            absEtaBins = cms.vdouble(-1., 1.479, 9.9),
            graphName = cms.string("jetToTauFakeRate/$hadTauSelection/$etaBin/jetToTauFakeRate_mc_hadTaus_pt"),
            applyGraph = cms.bool(True),
            fitFunctionName = cms.string("jetToTauFakeRate/$hadTauSelection/$etaBin/fitFunction_data_div_mc_hadTaus_pt"),
            applyFitFunction = cms.bool(True),
        ),
        third = cms.PSet(
            absEtaBins = cms.vdouble(-1., 1.479, 9.9),
            graphName = cms.string("jetToTauFakeRate/$hadTauSelection/$etaBin/jetToTauFakeRate_mc_hadTaus_pt"),
            applyGraph = cms.bool(True),
            fitFunctionName = cms.string("jetToTauFakeRate/$hadTauSelection/$etaBin/fitFunction_data_div_mc_hadTaus_pt"),
            applyFitFunction = cms.bool(True),
        )
    ),

    isMC = cms.bool(False),
    central_or_shift = cms.string('central'),
    lumiScale = cms.double(1.),
    apply_genWeight = cms.bool(True),
    apply_hlt_filter = cms.bool(False),
    apply_met_filters = cms.bool(True),
    cfgMEtFilter = recommendedMEtFilters,
    apply_hadTauFakeRateSF = cms.bool(False),

    fillGenEvtHistograms = cms.bool(False),
    cfgEvtYieldHistManager = EvtYieldHistManager_2017,

    branchName_electrons = cms.string('Electron'),
    branchName_muons = cms.string('Muon'),
    branchName_hadTaus = cms.string('Tau'),
    branchName_jets = cms.string('Jet'),
    branchName_met = cms.string('MET'),

    branchName_genLeptons = cms.string('GenLep'),
    branchName_genHadTaus = cms.string('GenVisTau'),
    branchName_genPhotons = cms.string('GenPhoton'),
    branchName_genJets = cms.string('GenJet'),
    redoGenMatching = cms.bool(True),

    selEventsFileName_input = cms.string(''),
    selEventsFileName_output = cms.string(''),

    isDEBUG = cms.bool(False),
    hasLHE = cms.bool(True),
)
