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


EventID       = np.zeros(tree_len, dtype=int)     
En_dep        = np.zeros(tree_len, dtype=float)      
Scint_ID      = np.zeros(tree_len, dtype=int)     
X_Primary     = np.zeros(tree_len, dtype=float)   
Y_Primary     = np.zeros(tree_len, dtype=float)      
Z_Primary     = np.zeros(tree_len, dtype=float)      
Theta_Primary = np.zeros(tree_len, dtype=float)   
Phi_Primary   = np.zeros(tree_len, dtype=float)    
En_Primary    = np.zeros(tree_len, dtype=float)     
Event_time    = np.zeros(tree_len, dtype=float)     
X_Detected    = np.zeros(tree_len, dtype=float)     
Y_Detected    = np.zeros(tree_len, dtype=float)     
Z_Detected    = np.zeros(tree_len, dtype=float)     

for i, entry in enumerate(tree):
    EventID[i]       = entry.EventID
    En_dep[i]        = entry.En_dep 
    Scint_ID[i]      = entry.Scint_ID
    X_Primary[i]     = entry.X_Primary
    Y_Primary[i]     = entry.Y_Primary
    Z_Primary[i]     = entry.Z_Primary
    Theta_Primary[i] = entry.Theta_Primary
    Phi_Primary[i]   = entry.Phi_Primary
    En_Primary[i]    = entry.En_Primary
    Event_time[i]    = entry.Event_time
    X_Detected[i]    = entry.X_Detected
    Y_Detected[i]    = entry.Y_Detected
    Z_Detected[i]    = entry.Z_Detected



sorting = np.argsort(EventID)

# Write FITS file
# "Null" primary array
prhdu = pyfits.PrimaryHDU()
# Extension
tbhdu = pyfits.BinTableHDU.from_columns([
    pyfits.Column(name='EventID', 
                  format='1K', 
                  array=EventID[sorting]), 
    pyfits.Column(name='En_dep', 
                  format='1D', 
                  unit='keV',
                  array=En_dep[sorting]),
    pyfits.Column(name='Scint_ID', 
                  format='1K', 
                  array=Scint_ID[sorting]),
    pyfits.Column(name='X_Primary', 
                  format='1D', 
                  unit='cm',
                  array=X_Primary[sorting]),
    pyfits.Column(name='Y_Primary', 
                  format='1D', 
                  unit='cm',
                  array=Y_Primary[sorting]),
    pyfits.Column(name='Z_Primary', 
                  format='1D', 
                  unit='cm',
                  array=Z_Primary[sorting]),
    pyfits.Column(name='Theta_Primary', 
                  format='1D', 
                  unit='degree',
                  array=Theta_Primary[sorting]),
    pyfits.Column(name='Phi_Primary', 
                  format='1D', 
                  unit='degree',
                  array=Phi_Primary[sorting]),
    pyfits.Column(name='En_Primary', 
                  format='1D', 
                  unit='keV',
                  array=En_Primary[sorting]),
    pyfits.Column(name='X_Detected', 
                  format='1D', 
                  unit='cm',
                  array=X_Detected[sorting]),
    pyfits.Column(name='Y_Detected', 
                  format='1D', 
                  unit='cm',
                  array=Y_Detected[sorting]),
    pyfits.Column(name='Z_Detected', 
                  format='1D', 
                  unit='cm',
                  array=Z_Detected[sorting])
])

tbhdu.header.set('EXTNAME', 'EVENTS', 'Name of this binary table extension')

hdulist = pyfits.HDUList([prhdu, tbhdu])
file_name_out = outfilename.split(".")[0] + ".fits"
hdulist.writeto(file_name_out, overwrite=True)
print("Wrote " + file_name_out)

# os.system("gzip -f " + outfilename.split(".")[0] + ".fits")

