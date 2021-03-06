#include "hhAnalysis/multilepton/interface/Data_to_MC_CorrectionInterface_hh_1l_3tau_trigger.h"

#include "TauAnalysisTools/TauTriggerSFs/interface/TauTriggerSFs2017.h" // TauTriggerSFs2017

#include "tthAnalysis/HiggsToTauTau/interface/leptonTypes.h" // kElectron, kMuon
#include "tthAnalysis/HiggsToTauTau/interface/analysisAuxFunctions.h" // kEra_2017
#include "tthAnalysis/HiggsToTauTau/interface/cmsException.h" // cmsException()
#include "tthAnalysis/HiggsToTauTau/interface/data_to_MC_corrections_auxFunctions.h" // aux::
#include "tthAnalysis/HiggsToTauTau/interface/sysUncertOptions.h" // TriggerSFsys, getTriggerSF_option()

#include <TString.h> // Form()
#include <TMath.h> // std::fabs()

#include <boost/algorithm/string/predicate.hpp> // boost::ends_with()

#include <cassert> // assert()

Data_to_MC_CorrectionInterface_hh_1l_3tau_trigger::Data_to_MC_CorrectionInterface_hh_1l_3tau_trigger(const edm::ParameterSet & cfg)
  : Data_to_MC_CorrectionInterface_1l_2tau_trigger(cfg)
  , hadTau3_genPdgId_(0)
  , hadTau3_pt_(0.)
  , hadTau3_eta_(0.)
  , hadTau3_phi_(0.)
  , hadTau3_decayMode_(0)
  , effTrigger_2tau_tauLeg_(era_str_, hadTauSelection_, TauTriggerType::DiTau)
{}

Data_to_MC_CorrectionInterface_hh_1l_3tau_trigger::~Data_to_MC_CorrectionInterface_hh_1l_3tau_trigger()
{}

void
Data_to_MC_CorrectionInterface_hh_1l_3tau_trigger::setTriggerBits(bool isTriggered_1e,
                                                                  bool isTriggered_1e1tau,
                                                                  bool isTriggered_1m,
                                                                  bool isTriggered_1m1tau,
                                                                  bool isTriggered_2tau)
{
  isTriggered_1e_     = isTriggered_1e;
  isTriggered_1e1tau_ = isTriggered_1e1tau;
  isTriggered_1m_     = isTriggered_1m;
  isTriggered_1m1tau_ = isTriggered_1m1tau;
  isTriggered_2tau_   = isTriggered_2tau;
}

void
Data_to_MC_CorrectionInterface_hh_1l_3tau_trigger::setLeptons(int lepton_type,
                                                              double lepton_pt,
                                                              double lepton_eta)
{
  lepton_type_ = lepton_type;
  lepton_pt_   = lepton_pt;
  lepton_eta_  = lepton_eta;
}

void
Data_to_MC_CorrectionInterface_hh_1l_3tau_trigger::setHadTaus(int hadTau1_genPdgId, double hadTau1_pt, double hadTau1_eta, int hadTau1_decayMode,
                                                              int hadTau2_genPdgId, double hadTau2_pt, double hadTau2_eta, int hadTau2_decayMode)
{
  throw cmsException(this, __func__, __LINE__) << "Invalid call";
}

void
Data_to_MC_CorrectionInterface_hh_1l_3tau_trigger::setHadTaus(double hadTau1_pt, double hadTau1_eta, double hadTau1_phi, int hadTau1_decayMode,
                                                              double hadTau2_pt, double hadTau2_eta, double hadTau2_phi, int hadTau2_decayMode,
                                                              double hadTau3_pt, double hadTau3_eta, double hadTau3_phi, int hadTau3_decayMode)
{
  hadTau1_pt_        = hadTau1_pt;
  hadTau1_eta_       = hadTau1_eta;
  hadTau1_phi_       = hadTau1_phi;
  hadTau1_decayMode_ = hadTau1_decayMode;

  hadTau2_pt_        = hadTau2_pt;
  hadTau2_eta_       = hadTau2_eta;
  hadTau2_phi_       = hadTau2_phi;
  hadTau2_decayMode_ = hadTau2_decayMode;

  hadTau3_pt_        = hadTau3_pt;
  hadTau3_eta_       = hadTau3_eta;
  hadTau3_phi_       = hadTau3_phi;
  hadTau3_decayMode_ = hadTau3_decayMode;
}

double
Data_to_MC_CorrectionInterface_hh_1l_3tau_trigger::getSF_triggerEff(TriggerSFsys central_or_shift) const
{
  if(isDEBUG_)
  {
    std::cout << get_human_line(this, __func__, __LINE__) << '\n';
  }
  assert(check_triggerSFsys_opt(central_or_shift));
  const TriggerSFsys central_or_shift_1l1tau =
    central_or_shift == TriggerSFsys::shift_0l2tauUp || central_or_shift == TriggerSFsys::shift_0l2tauDown ?
    TriggerSFsys::central :
    central_or_shift
  ;
  const TriggerSFsys central_or_shift_0l2tau =
    central_or_shift == TriggerSFsys::shift_1l1tauUp || central_or_shift == TriggerSFsys::shift_1l1tauDown ?
    TriggerSFsys::central :
    central_or_shift
  ;

  double eff_1l_data             = 0.;
  double eff_1l_mc               = 0.;
  double eff_1l1tau_lepLeg_data  = 0.;
  double eff_1l1tau_lepLeg_mc    = 0.;
  double eff_1l1tau_tauLeg1_data = 0.;
  double eff_1l1tau_tauLeg1_mc   = 0.;
  double eff_1l1tau_tauLeg2_data = 0.;
  double eff_1l1tau_tauLeg2_mc   = 0.;
  double eff_1l1tau_tauLeg3_data = 0.;
  double eff_1l1tau_tauLeg3_mc   = 0.;

  bool isTriggered_1l     = false;
  bool isTriggered_1l1tau = false;

  if(lepton_type_ == kElectron)
  {
    if(isDEBUG_)
    {
      std::cout << "electron: pT = " << lepton_pt_ << ", eta = " << lepton_eta_ << '\n';
    }
    eff_1l_data            = get_from_lut(effTrigger_1e_data_,            lepton_pt_, lepton_eta_, isDEBUG_);
    eff_1l_mc              = get_from_lut(effTrigger_1e_mc_,              lepton_pt_, lepton_eta_, isDEBUG_);
    eff_1l1tau_lepLeg_data = get_from_lut(effTrigger_1e1tau_lepLeg_data_, lepton_pt_, lepton_eta_, isDEBUG_);
    eff_1l1tau_lepLeg_mc   = get_from_lut(effTrigger_1e1tau_lepLeg_mc_,   lepton_pt_, lepton_eta_, isDEBUG_);

    eff_1l1tau_tauLeg1_data = 0.;
    eff_1l1tau_tauLeg1_mc   = 0.;
    if(std::fabs(hadTau1_eta_) <= 2.1 && aux::hasDecayMode(allowedDecayModes_, hadTau1_decayMode_))
    {
      eff_1l1tau_tauLeg1_data = effTrigger_1e1tau_tauLeg_.getTauTriggerEvalData(central_or_shift_1l1tau, hadTau1_pt_, hadTau1_eta_, hadTau1_phi_, hadTau1_decayMode_);
      eff_1l1tau_tauLeg1_mc   = effTrigger_1e1tau_tauLeg_.getTauTriggerEvalMC  (central_or_shift_1l1tau, hadTau1_pt_, hadTau1_eta_, hadTau1_phi_, hadTau1_decayMode_);
    }

    eff_1l1tau_tauLeg2_data = 0.;
    eff_1l1tau_tauLeg2_mc   = 0.;
    if(std::fabs(hadTau2_eta_) <= 2.1 && aux::hasDecayMode(allowedDecayModes_, hadTau2_decayMode_))
    {
      eff_1l1tau_tauLeg2_data = effTrigger_1e1tau_tauLeg_.getTauTriggerEvalData(central_or_shift_1l1tau, hadTau2_pt_, hadTau2_eta_, hadTau2_phi_, hadTau2_decayMode_);
      eff_1l1tau_tauLeg2_mc   = effTrigger_1e1tau_tauLeg_.getTauTriggerEvalMC  (central_or_shift_1l1tau, hadTau2_pt_, hadTau2_eta_, hadTau2_phi_, hadTau2_decayMode_);
    }

    eff_1l1tau_tauLeg3_data   = 0.;
    eff_1l1tau_tauLeg3_mc     = 0.;
    if(std::fabs(hadTau3_eta_) <= 2.1 && aux::hasDecayMode(allowedDecayModes_, hadTau3_decayMode_))
    {
      eff_1l1tau_tauLeg3_data = effTrigger_1e1tau_tauLeg_.getTauTriggerEvalData(central_or_shift_1l1tau, hadTau3_pt_, hadTau3_eta_, hadTau3_phi_, hadTau3_decayMode_);
      eff_1l1tau_tauLeg3_mc   = effTrigger_1e1tau_tauLeg_.getTauTriggerEvalMC  (central_or_shift_1l1tau, hadTau3_pt_, hadTau3_eta_, hadTau3_phi_, hadTau3_decayMode_);
    }

    isTriggered_1l     = isTriggered_1e_;
    isTriggered_1l1tau = isTriggered_1e1tau_;
  }
  else if(lepton_type_ == kMuon)
  {
    if(isDEBUG_)
    {
      std::cout << "muon: pT = " << lepton_pt_ << ", eta = " << lepton_eta_ << '\n';
    }
    eff_1l_data            = get_from_lut(effTrigger_1m_data_,            lepton_pt_, lepton_eta_, isDEBUG_);
    eff_1l_mc              = get_from_lut(effTrigger_1m_mc_,              lepton_pt_, lepton_eta_, isDEBUG_);
    eff_1l1tau_lepLeg_data = get_from_lut(effTrigger_1m1tau_lepLeg_data_, lepton_pt_, lepton_eta_, isDEBUG_);
    eff_1l1tau_lepLeg_mc   = get_from_lut(effTrigger_1m1tau_lepLeg_mc_,   lepton_pt_, lepton_eta_, isDEBUG_);

    eff_1l1tau_tauLeg1_data = 0.;
    eff_1l1tau_tauLeg1_mc   = 0.;
    if(std::fabs(hadTau1_eta_) <= 2.1 && aux::hasDecayMode(allowedDecayModes_, hadTau1_decayMode_))
    {
      eff_1l1tau_tauLeg1_data = effTrigger_1m1tau_tauLeg_.getTauTriggerEvalData(central_or_shift_1l1tau, hadTau1_pt_, hadTau1_eta_, hadTau1_phi_, hadTau1_decayMode_);
      eff_1l1tau_tauLeg1_mc   = effTrigger_1m1tau_tauLeg_.getTauTriggerEvalData(central_or_shift_1l1tau, hadTau1_pt_, hadTau1_eta_, hadTau1_phi_, hadTau1_decayMode_);
    }

    eff_1l1tau_tauLeg2_data   = 0.;
    eff_1l1tau_tauLeg2_mc     = 0.;
    if(std::fabs(hadTau2_eta_) <= 2.1 && aux::hasDecayMode(allowedDecayModes_, hadTau2_decayMode_))
    {
      eff_1l1tau_tauLeg2_data = effTrigger_1m1tau_tauLeg_.getTauTriggerEvalData(central_or_shift_1l1tau, hadTau2_pt_, hadTau2_eta_, hadTau2_phi_, hadTau2_decayMode_);
      eff_1l1tau_tauLeg2_mc   = effTrigger_1m1tau_tauLeg_.getTauTriggerEvalData(central_or_shift_1l1tau, hadTau2_pt_, hadTau2_eta_, hadTau2_phi_, hadTau2_decayMode_);
    }

    eff_1l1tau_tauLeg3_data   = 0.;
    eff_1l1tau_tauLeg3_mc     = 0.;
    if(std::fabs(hadTau3_eta_) <= 2.1 && aux::hasDecayMode(allowedDecayModes_, hadTau3_decayMode_))
    {
      eff_1l1tau_tauLeg3_data = effTrigger_1m1tau_tauLeg_.getTauTriggerEvalData(central_or_shift_1l1tau, hadTau3_pt_, hadTau3_eta_, hadTau3_phi_, hadTau3_decayMode_);
      eff_1l1tau_tauLeg3_mc   = effTrigger_1m1tau_tauLeg_.getTauTriggerEvalData(central_or_shift_1l1tau, hadTau3_pt_, hadTau3_eta_, hadTau3_phi_, hadTau3_decayMode_);
    }

    isTriggered_1l     = isTriggered_1m_;
    isTriggered_1l1tau = isTriggered_1m1tau_;
  }
  else
  {
    assert(0);
  }

  if(isDEBUG_)
  {
    std::cout << "isTriggered_1l = " << isTriggered_1l << '\n';
    std::cout << "isTriggered_1l1tau = " << isTriggered_1l1tau << '\n';
    
    std::cout << "eff_1l_data = " << eff_1l_data << '\n';
    std::cout << "eff_1l_mc = " << eff_1l_mc << '\n';
    std::cout << "eff_1l1tau_lepLeg_data = " << eff_1l1tau_lepLeg_data << '\n';
    std::cout << "eff_1l1tau_lepLeg_mc = " << eff_1l1tau_lepLeg_mc << '\n';
    
    std::cout << "eff_1l1tau_tauLeg1_data = " << eff_1l1tau_tauLeg1_data << '\n';
    std::cout << "eff_1l1tau_tauLeg1_mc = " << eff_1l1tau_tauLeg1_mc << '\n';
    std::cout << "eff_1l1tau_tauLeg2_data = " << eff_1l1tau_tauLeg2_data << '\n';
    std::cout << "eff_1l1tau_tauLeg2_mc = " << eff_1l1tau_tauLeg2_mc << '\n';
    std::cout << "eff_1l1tau_tauLeg3_data = " << eff_1l1tau_tauLeg3_data << '\n';
    std::cout << "eff_1l1tau_tauLeg3_mc = " << eff_1l1tau_tauLeg3_mc << '\n';
  }

  const double eff_2tau_tauLeg1_data = effTrigger_2tau_tauLeg_.getTauTriggerEvalData(central_or_shift_0l2tau, hadTau1_pt_, hadTau1_eta_, hadTau1_phi_, hadTau1_decayMode_);
  const double eff_2tau_tauLeg1_mc   = effTrigger_2tau_tauLeg_.getTauTriggerEvalMC  (central_or_shift_0l2tau, hadTau1_pt_, hadTau1_eta_, hadTau1_phi_, hadTau1_decayMode_);

  const double eff_2tau_tauLeg2_data = effTrigger_2tau_tauLeg_.getTauTriggerEvalData(central_or_shift_0l2tau, hadTau2_pt_, hadTau2_eta_, hadTau2_phi_, hadTau2_decayMode_);
  const double eff_2tau_tauLeg2_mc   = effTrigger_2tau_tauLeg_.getTauTriggerEvalMC  (central_or_shift_0l2tau, hadTau2_pt_, hadTau2_eta_, hadTau2_phi_, hadTau2_decayMode_);

  const double eff_2tau_tauLeg3_data = effTrigger_2tau_tauLeg_.getTauTriggerEvalData(central_or_shift_0l2tau, hadTau3_pt_, hadTau3_eta_, hadTau3_phi_, hadTau3_decayMode_);
  const double eff_2tau_tauLeg3_mc   = effTrigger_2tau_tauLeg_.getTauTriggerEvalMC  (central_or_shift_0l2tau, hadTau3_pt_, hadTau3_eta_, hadTau3_phi_, hadTau3_decayMode_);

  if(isDEBUG_)
  {
    std::cout << "eff_2tau_tauLeg1_data = " << eff_2tau_tauLeg1_data << '\n';
    std::cout << "eff_2tau_tauLeg1_mc = " << eff_2tau_tauLeg1_mc << '\n';
    std::cout << "eff_2tau_tauLeg2_data = " << eff_2tau_tauLeg2_data << '\n';
    std::cout << "eff_2tau_tauLeg2_mc = " << eff_2tau_tauLeg2_mc << '\n';
    std::cout << "eff_2tau_tauLeg3_data = " << eff_2tau_tauLeg3_data << '\n';
    std::cout << "eff_2tau_tauLeg3_mc = " << eff_2tau_tauLeg3_mc << '\n';
  }

  double prob_data = 0.;
  double prob_mc   = 0.;

  for(int lepton_status = k1lAnd1l1tau; lepton_status <= kNot1lAndNot1l1tau; ++lepton_status)
  {
    int nTrig_1l = 0;
    if(lepton_status == k1lAnd1l1tau || lepton_status == k1lAndNot1l1tau)
    {
      ++nTrig_1l;
    }

    int nTrig_1l1tau_lepLeg = 0;
    if(lepton_status == k1lAnd1l1tau || lepton_status == kNot1lAnd1l1tau)
    {
      ++nTrig_1l1tau_lepLeg;
    }

    const double prob_lepton_data = getProb_lepton(lepton_status, eff_1l_data, eff_1l1tau_lepLeg_data);
    const double prob_lepton_mc   = getProb_lepton(lepton_status, eff_1l_mc,   eff_1l1tau_lepLeg_mc);

    for(int tau1_status = k1l1tauAnd2tau; tau1_status <= kNot1l1tauAndNot2tau; ++tau1_status)
    {
      const double prob_tau1_data = getProb_tau(tau1_status, eff_1l1tau_tauLeg1_data, eff_2tau_tauLeg1_data);
      const double prob_tau1_mc   = getProb_tau(tau1_status, eff_1l1tau_tauLeg1_mc,   eff_2tau_tauLeg1_mc);

      for(int tau2_status = k1l1tauAnd2tau; tau2_status <= kNot1l1tauAndNot2tau; ++tau2_status)
      {
        const double prob_tau2_data = getProb_tau(tau2_status, eff_1l1tau_tauLeg2_data, eff_2tau_tauLeg2_data);
        const double prob_tau2_mc   = getProb_tau(tau2_status, eff_1l1tau_tauLeg2_mc,   eff_2tau_tauLeg2_mc);

        for(int tau3_status = k1l1tauAnd2tau; tau3_status <= kNot1l1tauAndNot2tau; ++tau3_status)
        {
          const double prob_tau3_data = getProb_tau(tau3_status, eff_1l1tau_tauLeg3_data, eff_2tau_tauLeg3_data);
          const double prob_tau3_mc   = getProb_tau(tau3_status, eff_1l1tau_tauLeg3_mc,   eff_2tau_tauLeg3_mc);

	  int nTrig_1l1tau_tauLeg = 0;
	  if(tau1_status == k1l1tauAnd2tau || tau1_status == k1l1tauAndNot2tau)
	  {
	    ++nTrig_1l1tau_tauLeg;
	  }
	  if(tau2_status == k1l1tauAnd2tau || tau2_status == k1l1tauAndNot2tau)
          {
            ++nTrig_1l1tau_tauLeg;
          }
	  if(tau3_status == k1l1tauAnd2tau || tau3_status == k1l1tauAndNot2tau)
          {
            ++nTrig_1l1tau_tauLeg;
          }

	  int nTrig_2tau_tauLeg = 0;
	  if(tau1_status == k1l1tauAnd2tau || tau1_status == kNot1l1tauAnd2tau)
          {
            ++nTrig_2tau_tauLeg;
          }
	  if(tau2_status == k1l1tauAnd2tau || tau2_status == kNot1l1tauAnd2tau)
          {
            ++nTrig_2tau_tauLeg;
          }
          if(tau3_status == k1l1tauAnd2tau || tau3_status == kNot1l1tauAnd2tau)
          {
            ++nTrig_2tau_tauLeg;
          }

          if(isDEBUG_)
          {
            std::cout
              << "nTrig_1l = " << nTrig_1l << ","
                 " nTrig_1l1tau_lepLeg = " << nTrig_1l1tau_lepLeg << ","
                 " nTrig_1l1tau_tauLeg = " << nTrig_1l1tau_tauLeg << ","
                 " nTrig_2tau_tauLeg   = " << nTrig_2tau_tauLeg   << '\n'
            ;
          }

          const bool isTrig_1l_toy     = nTrig_1l >= 1;
          const bool isTrig_1l1tau_toy = nTrig_1l1tau_lepLeg >= 1 && nTrig_1l1tau_tauLeg >= 1;
          const bool isTrig_2tau_toy   = nTrig_2tau_tauLeg >= 2;

          if(isTriggered_1l == isTrig_1l_toy         &&
             isTriggered_1l1tau == isTrig_1l1tau_toy &&
             isTriggered_2tau_ == isTrig_2tau_toy     )
          {
            if(isDEBUG_)
            {
              std::cout
                << "isTrig_1l_toy      = " << isTrig_1l_toy     << ","
                   " isTrig_1l1tau_toy = " << isTrig_1l1tau_toy << ","
                   " isTrig_2tau_toy   = " << isTrig_2tau_toy   << ":"
                   " prob_data = " << (prob_lepton_data * prob_tau1_data * prob_tau2_data * prob_tau3_data) << ","
                   " prob_mc   = " << (prob_lepton_mc * prob_tau1_mc * prob_tau2_mc * prob_tau3_mc)         << '\n'
              ;
            }
            prob_data += prob_lepton_data * prob_tau1_data * prob_tau2_data * prob_tau3_data;
            prob_mc   += prob_lepton_mc   * prob_tau1_mc   * prob_tau2_mc   * prob_tau3_mc;
          }
        }
      }
    }
  }

  if(isDEBUG_)
  {
    std::cout << "hadTau (lead):    pT = " << hadTau1_pt_ << ", eta = " << hadTau1_eta_ << "\n"
                 "hadTau (sublead): pT = " << hadTau2_pt_ << ", eta = " << hadTau2_eta_ << "\n"
                 "hadTau (third):   pT = " << hadTau3_pt_ << ", eta = " << hadTau3_eta_ << "\n"
                 "eff (L):      data = " << eff_1l_data             << ", MC = " << eff_1l_mc             << "\n"
                 "eff (X_lep):  data = " << eff_1l1tau_lepLeg_data  << ", MC = " << eff_1l1tau_lepLeg_mc  << "\n"
                 "eff (X_tau1): data = " << eff_1l1tau_tauLeg1_data << ", MC = " << eff_1l1tau_tauLeg1_mc << "\n"
                 "eff (X_tau2): data = " << eff_1l1tau_tauLeg2_data << ", MC = " << eff_1l1tau_tauLeg2_mc << "\n"
                 "eff (X_tau3): data = " << eff_1l1tau_tauLeg3_data << ", MC = " << eff_1l1tau_tauLeg3_mc << "\n"
                 "eff (T_tau1): data = " << eff_2tau_tauLeg1_data   << ", MC = " << eff_2tau_tauLeg1_mc   << "\n"
                 "eff (T_tau2): data = " << eff_2tau_tauLeg2_data   << ", MC = " << eff_2tau_tauLeg2_mc   << "\n"
                 "eff (T_tau3): data = " << eff_2tau_tauLeg3_data   << ", MC = " << eff_2tau_tauLeg3_mc   << '\n'
    ;
  }

  double sf = aux::compSF(prob_data, prob_mc);
  if(isDEBUG_)
  {
    int idxCase = 0;
    if(isTriggered_1l    ) idxCase += 1;
    if(isTriggered_1l1tau) idxCase += 2;
    if(isTriggered_2tau_ ) idxCase += 4;

    const std::string text_1l     = isTriggered_1l     ? "single lepton trigger fires"    : "single lepton trigger doesn't fire";
    const std::string text_1l1tau = isTriggered_1l1tau ? "lepton+tau cross trigger fires" : "lepton+tau cross trigger doesn't fire";
    const std::string text_2tau   = isTriggered_2tau_  ? "ditau trigger fires"            : "ditau trigger doesn't fire";
    std::cout << "case " << idxCase << ": "
              << text_1l     << " and "
              << text_1l1tau << " and "
              << text_2tau   << "\n"
                 " eff: "
                 "data = " << prob_data << ", "
                 "MC = "   << prob_mc   << " --> "
                 "SF = "   << sf        << '\n'
    ;
  }

  return sf;
}

double
Data_to_MC_CorrectionInterface_hh_1l_3tau_trigger::getProb_lepton(int lepton_status,
                                                                  double eff_1l,
                                                                  double eff_1l1tau_lepLeg) const
{
  double prob = 0.;
  switch(lepton_status)
  {
    case k1lAnd1l1tau:       prob = std::min(eff_1l, eff_1l1tau_lepLeg);         break;
    case k1lAndNot1l1tau:    prob = std::max(1.e-2, eff_1l - eff_1l1tau_lepLeg); break;
    case kNot1lAnd1l1tau:    prob = std::max(1.e-2, eff_1l1tau_lepLeg - eff_1l); break;
    case kNot1lAndNot1l1tau: prob = 1. - std::max(eff_1l, eff_1l1tau_lepLeg);    break;
    default:                 assert(0);
  }
  if(isDEBUG_)
  {
    std::cout << get_human_line(this, __func__, __LINE__) << ": prob = " << prob << '\n';
  }
  return prob;
}
 
double
Data_to_MC_CorrectionInterface_hh_1l_3tau_trigger::getProb_tau(int tau_status,
                                                               double eff_1l1tau_tauLeg,
                                                               double eff_2tau_tauLeg) const
{
  double prob = 0.;
  switch(tau_status)
  {
    case k1l1tauAnd2tau:       prob = std::min(eff_1l1tau_tauLeg, eff_2tau_tauLeg);         break;
    case k1l1tauAndNot2tau:    prob = std::max(1.e-2, eff_1l1tau_tauLeg - eff_2tau_tauLeg); break;
    case kNot1l1tauAnd2tau:    prob = std::max(1.e-2, eff_2tau_tauLeg - eff_1l1tau_tauLeg); break;
    case kNot1l1tauAndNot2tau: prob = 1. - std::max(eff_1l1tau_tauLeg, eff_2tau_tauLeg);    break;
    default:                   assert(0);
  }
  if(isDEBUG_)
  {
    std::cout << get_human_line(this, __func__, __LINE__) << ": prob = " << prob << '\n';
  }
  return prob;
}

bool
Data_to_MC_CorrectionInterface_hh_1l_3tau_trigger::check_triggerSFsys_opt(TriggerSFsys central_or_shift) const
{
  return
    central_or_shift == TriggerSFsys::central          ||
    central_or_shift == TriggerSFsys::shiftUp          ||
    central_or_shift == TriggerSFsys::shiftDown        ||
    central_or_shift == TriggerSFsys::shift_0l2tauUp   ||
    central_or_shift == TriggerSFsys::shift_0l2tauDown ||
    central_or_shift == TriggerSFsys::shift_1l1tauUp   ||
    central_or_shift == TriggerSFsys::shift_1l1tauDown
  ;
}
