import numpy as np
import astropy.io.fits as pyfits
import os
import sys
import glob

import ROOT

if len(sys.argv) != 1:
    outfilename = sys.argv[1]
else:
    outfilename = "scorefile.root"


ROOT.gROOT.Reset()
f = ROOT.TFile(outfilename)

tree = f.Get("Events")
tree_len = tree.GetEntries()


# t_ID = np.zeros(tree_len, dtype=int)
# t_ED = np.zeros(tree_len, dtype=float)
# t_SI = np.zeros(tree_len, dtype=float)
# t_SD = np.zeros(tree_len, dtype=float)
# t_SC = np.zeros(tree_len, dtype=float)
# t_XP = np.zeros(tree_len, dtype=float)
# t_YP = np.zeros(tree_len, dtype=float)
# t_ZP = np.zeros(tree_len, dtype=float)
# t_TP = np.zeros(tree_len, dtype=float)
# t_PP = np.zeros(tree_len, dtype=float)
# t_EP = np.zeros(tree_len, dtype=float)
# t_TI = np.zeros(tree_len, dtype=float)

t_ID = np.zeros(tree_len, dtype=int)
t_ED = np.zeros(tree_len, dtype=float)
t_SI = np.zeros(tree_len, dtype=int)
t_XP = np.zeros(tree_len, dtype=float)
t_YP = np.zeros(tree_len, dtype=float)
t_ZP = np.zeros(tree_len, dtype=float)
t_TP = np.zeros(tree_len, dtype=float)
t_PP = np.zeros(tree_len, dtype=float)
t_EP = np.zeros(tree_len, dtype=float)
t_TI = np.zeros(tree_len, dtype=float)

for i, entry in enumerate(tree):
      t_ID[i] = entry.EventID
      t_ED[i] = entry.En_dep
      t_SI[i] = entry.Scint_ID
      t_XP[i] = entry.X_Primary
      t_YP[i] = entry.Y_Primary
      t_ZP[i] = entry.Z_Primary
      t_TP[i] = entry.Theta_Primary
      t_PP[i] = entry.Phi_Primary
      t_EP[i] = entry.En_Primary
      t_TI[i] = entry.Event_time



sorting = np.argsort(t_ID)

# Write FITS file
# "Null" primary array
prhdu = pyfits.PrimaryHDU()
# Extension
tbhdu = pyfits.BinTableHDU.from_columns([
                                          pyfits.Column(name='EventID', 
                                                        format='1K', 
                                                        array=t_ID[sorting]), 
                                          pyfits.Column(name='En_dep', 
                                                        format='1D', 
                                                        unit='keV',
                                                        array=t_ED[sorting]), 
                                          pyfits.Column(name='Scint_ID', 
                                                        format='1K',
                                                        array=t_SI[sorting]),
                                          pyfits.Column(name='X_Primary',
                                                        format='1D',
                                                        unit='cm',
                                                        array=t_XP[sorting]),
                                          pyfits.Column(name='Y_Primary',
                                                        format='1D',
                                                        unit='cm',
                                                        array=t_YP[sorting]),
                                          pyfits.Column(name='Z_Primary',
                                                        format='1D',
                                                        unit='cm',
                                                        array=t_ZP[sorting]),
                                          pyfits.Column(name='Theta_Primary',
                                                        format='1D',
                                                        unit='deg',
                                                        array=t_TP[sorting]),
                                          pyfits.Column(name='Phi_Primary',
                                                        format='1D',
                                                        unit='deg',
                                                        array=t_PP[sorting]),
                                          pyfits.Column(name='En_Primary',
                                                        format='1D',
                                                        unit='keV',
                                                        array=t_EP[sorting]),
                                        pyfits.Column(name='Event_time', 
                                                        format='1D', 
                                                        unit='ns',
                                                        array=t_TI[sorting])
                                        ])

tbhdu.header.set('EXTNAME', 'EVENTS', 'Name of this binary table extension')

hdulist = pyfits.HDUList([prhdu, tbhdu])
hdulist.writeto(outfilename.split(".")[0] + ".fits", overwrite=True)

# os.system("gzip -f " + outfilename.split(".")[0] + ".fits")

