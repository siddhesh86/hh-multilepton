from tthAnalysis.HiggsToTauTau.samples.tthAnalyzeSamples_2016_preselected_base import samples_2016 as samples_2016_bkg
from hhAnalysis.multilepton.samples.hhAnalyzeSamples_2016_hh import samples_2016 as samples_2016_hh

from hhAnalysis.multilepton.samples.reclassifySamples import reclassifySamples
samples_2016 = reclassifySamples(samples_2016_hh, samples_2016_bkg)