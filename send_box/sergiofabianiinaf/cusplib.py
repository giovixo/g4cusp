import numpy as np
import pandas as pd
import astropy.io.fits as fits
import os
import sys
import glob


#For plotting
import matplotlib
import scipy
from scipy.interpolate import interp1d
import matplotlib.gridspec as gridspec
from matplotlib.colors import LogNorm
import copy
from astropy.table import Table
from astropy.io import ascii
from matplotlib import pyplot as plt
import matplotlib.patches as patches
import matplotlib.patches as mpatches



matplotlib.rcParams.update({'font.size': 18})
matplotlib.rcParams['image.cmap'] = 'viridis'
matplotlib.rcParams["lines.linewidth"] = 3.
matplotlib.rcParams["lines.markersize"] = 5.
matplotlib.rcParams["lines.markeredgewidth"] = 3.


def loadData(filename):
    '''
    '''
    hdul = fits.open(filename)
    hdul.info()
    events = hdul[1].data
    print(events.columns)
    t_ID = events['EventID']
    t_ED = events['En_dep']
    t_SI = events['Scint_ID']
    t_XP = events['X_Primary']
    t_YP = events['Y_Primary']
    t_ZP = events['Z_Primary']
    t_TP = events['Theta_Primary']
    t_PP = events['Phi_Primary']
    t_EP = events['En_Primary']
    t_Dx = events['X_Detected']
    t_Dy = events['Y_Detected']
    t_Dz = events['Z_Detected']
    #
    EventList={}
    for evtID in t_ID:
        EventList[evtID]={}
        EventList[evtID]['EventID']=evtID
        EventList[evtID]['En_dep']=[]
        EventList[evtID]['Scint_ID']=[]
        EventList[evtID]['X_Primary']=[]
        EventList[evtID]['Y_Primary']=[]    
        EventList[evtID]['Z_Primary']=[]
        EventList[evtID]['Theta_Primary']=[]
        EventList[evtID]['Phi_Primary']=[]
        EventList[evtID]['En_Primary']=[]
        EventList[evtID]['X_Detected']=[]   
        EventList[evtID]['Y_Detected']=[]
        EventList[evtID]['Z_Detected']=[]
        
    for i,evtID in enumerate(t_ID):
        EventList[evtID]['En_dep'].append(t_ED[i])
        EventList[evtID]['Scint_ID'].append(t_SI[i])
        EventList[evtID]['X_Primary'].append(t_XP[i])
        EventList[evtID]['Y_Primary'].append(t_YP[i])   
        EventList[evtID]['Z_Primary'].append(t_ZP[i])
        EventList[evtID]['Theta_Primary'].append(t_TP[i])
        EventList[evtID]['Phi_Primary'].append(t_PP[i])
        EventList[evtID]['En_Primary'].append(t_EP[i])
        EventList[evtID]['X_Detected'].append(t_Dx[i])  
        EventList[evtID]['Y_Detected'].append(t_Dy[i])
        EventList[evtID]['Z_Detected'].append(t_Dz[i])
                    
    for i,evtID in enumerate(t_ID):
        EventList[evtID]['En_dep']=np.array(EventList[evtID]['En_dep'])
        EventList[evtID]['Scint_ID']=np.array(EventList[evtID]['Scint_ID'])
        EventList[evtID]['X_Primary']=np.array(EventList[evtID]['X_Primary'])
        EventList[evtID]['Y_Primary']=np.array(EventList[evtID]['Y_Primary'])  
        EventList[evtID]['Z_Primary']=np.array(EventList[evtID]['Z_Primary'])
        EventList[evtID]['Theta_Primary']=np.array(EventList[evtID]['Theta_Primary'])
        EventList[evtID]['Phi_Primary']=np.array(EventList[evtID]['Phi_Primary'])
        EventList[evtID]['En_Primary']=np.array(EventList[evtID]['En_Primary'])
        EventList[evtID]['X_Detected']=np.array(EventList[evtID]['X_Detected'])   
        EventList[evtID]['Y_Detected']=np.array(EventList[evtID]['Y_Detected'])
        EventList[evtID]['Z_Detected']=np.array(EventList[evtID]['Z_Detected'])
        
    del hdul
    del events
    del t_ID
    del t_ED
    del t_SI
    del t_XP
    del t_YP
    del t_ZP
    del t_TP
    del t_PP
    del t_EP
    del t_Dx
    del t_Dy
    del t_Dz
    return EventList
    
    
#def coincidences(EventList,energy=100.,no_photons = 10e+6, area_beam=144, geometric_area=10., n_bins = 500,en_max=100.,en_min=0.,en_threshold_emax=200.,en_threshold_emin=0.):
    #'''
    #energy/keV
    #area_beam/cm^2
    #geometric_area/cm^2
    #'''
    #element='scat'#'abs'
    #if element=='scat':
        #n_el1=0
        #n_el2=1
    #elif element=='abs':
        #n_el1=1
        #n_el2=0


    #energy_sct=[]
    #energy_sct2=[]
    #energy_abs=[]
    #energy_sum=[]
    #count_1=0
    #count_2=0
    #count_3=0

    #max_ev=-1
    ##Scatt-Abs coinc
    #for evt in list(EventList.keys())[0:max_ev]:
                ##Scatterer-Absorber
                #if( (len(EventList[evt]['Scint_ID'])==2)and\
                #(EventList[evt]['Scint_ID'][0]<100)and\
                #(EventList[evt]['Scint_ID'][1]>100)and\
                #(EventList[evt]['En_dep'][n_el1]<en_threshold_emax)and\
                #(EventList[evt]['En_dep'][n_el1]>en_threshold_emin)
                #):
                        #count_1+=1
                        #energy_sct.append(EventList[evt]['En_dep'][0])
                        #energy_abs.append(EventList[evt]['En_dep'][1])
                        #energy_sum.append(EventList[evt]['En_dep'][0]+EventList[evt]['En_dep'][1])
                ##to take in to account the order exchange in the file
                #if( (len(EventList[evt]['Scint_ID'])==2)and\
                #(EventList[evt]['Scint_ID'][0]>100)and\
                #(EventList[evt]['Scint_ID'][1]<100)and\
                #(EventList[evt]['En_dep'][n_el2]<en_threshold_emax)and\
                #(EventList[evt]['En_dep'][n_el2]>en_threshold_emin)
                #):
                        #count_2+=1
                        #energy_sct.append(EventList[evt]['En_dep'][1])
                        #energy_abs.append(EventList[evt]['En_dep'][0])
                        #energy_sum.append(EventList[evt]['En_dep'][0]+EventList[evt]['En_dep'][1])
                ##Double Coincidence: Scatterer-Scatterer-Absorber
                #if( (len(EventList[evt]['Scint_ID'])==3)and\
                #(EventList[evt]['Scint_ID'][0]<100)and\
                #(EventList[evt]['Scint_ID'][1]<100)and\
                #(EventList[evt]['Scint_ID'][2]>100)and\
                #(EventList[evt]['En_dep'][n_el1]<en_threshold_emax)and\
                #(EventList[evt]['En_dep'][n_el1]>en_threshold_emin)and\
                #(EventList[evt]['En_dep'][n_el1+1]<en_threshold_emax)and\
                #(EventList[evt]['En_dep'][n_el1+1]>en_threshold_emin)
                #):
                        #count_3+=1
                        #energy_sct.append(EventList[evt]['En_dep'][0])
                        #energy_sct2.append(EventList[evt]['En_dep'][1])
                        #energy_abs.append(EventList[evt]['En_dep'][2])
                        #energy_sum.append(EventList[evt]['En_dep'][0]+EventList[evt]['En_dep'][1]+EventList[evt]['En_dep'][2])
                #if( (len(EventList[evt]['Scint_ID'])==3)and\
                #(EventList[evt]['Scint_ID'][0]<100)and\
                #(EventList[evt]['Scint_ID'][1]>100)and\
                #(EventList[evt]['Scint_ID'][2]<100)and\
                #(EventList[evt]['En_dep'][n_el1]<en_threshold_emax)and\
                #(EventList[evt]['En_dep'][n_el1]>en_threshold_emin)and\
                #(EventList[evt]['En_dep'][n_el1+2]<en_threshold_emax)and\
                #(EventList[evt]['En_dep'][n_el1+2]>en_threshold_emin)
                #):
                        #count_3+=1Aeff_sct_abs_err
                        #energy_sct.append(EventList[evt]['En_dep'][0])
                        #energy_sct2.append(EventList[evt]['En_dep'][2])
                        #energy_abs.append(EventList[evt]['En_dep'][1])
                        #energy_sum.append(EventList[evt]['En_dep'][0]+EventList[evt]['En_dep'][1]+EventList[evt]['En_dep'][2])
                #if( (len(EventList[evt]['Scint_ID'])==3)and\
                #(EventList[evt]['Scint_ID'][0]>100)and\
                #(EventList[evt]['Scint_ID'][1]<100)and\
                #(EventList[evt]['Scint_ID'][2]<100)and\
                #(EventList[evt]['En_dep'][n_el1+1]<en_threshold_emax)and\
                #(EventList[evt]['En_dep'][n_el1+1]>en_threshold_emin)and\
                #(EventList[evt]['En_dep'][n_el1+2]<en_threshold_emax)and\
                #(EventList[evt]['En_dep'][n_el1+2]>en_threshold_emin)
                #):
                        #count_3+=1
                        #energy_sct.append(EventList[evt]['En_dep'][1])
                        #energy_sct2.append(EventList[evt]['En_dep'][2])
                        #energy_abs.append(EventList[evt]['En_dep'][0])
                        #energy_sum.append(EventList[evt]['En_dep'][0]+EventList[evt]['En_dep'][1]+EventList[evt]['En_dep'][2])      
                ##Scatterer multiplicity 2
                #if( (len(EventList[evt]['Scint_ID'])==2)and\
                #(EventList[evt]['Scint_ID'][0]<100)and\
                #(EventList[evt]['Scint_ID'][1]<100)and\
                #(EventList[evt]['En_dep'][n_el1]<en_threshold_emax)and\
                #(EventList[evt]['En_dep'][n_el1]>en_threshold_emin)
                #):
                        #count_1+=1
                        #energy_sct.append(EventList[evt]['En_dep'][0])
                        #energy_abs.append(EventList[evt]['En_dep'][1])
                        #energy_sum.append(EventList[evt]['En_dep'][0]+EventList[evt]['En_dep'][1])
                    
    #print('count 1', count_1)
    #print('count 2', count_2)
    #print('count 3', count_3)
    #print('tot ', count_1+count_2+count_3)

    ##Effective Area
    #count_1_2=count_1+count_2

    #Aeff_sct_abs=area_beam*((count_1_2)/no_photons)
    #Aeff_sct_abs_err = (area_beam/no_photons)*(count_1_2**0.5)
    #Aeff_sctX2_abs=area_beam*((count_3)/no_photons)
    #Aeff_sctX2_abs_err=(area_beam/no_photons)*(count_3**0.5)
    #Aeff_tot=Aeff_sct_abs+Aeff_sctX2_abs
    #count_1_2_3=count_1_2+count_3
    #Aeff_tot_err=(area_beam/no_photons)*(count_1_2_3)**0.5

    #print('')
    #print('energy ', energy)
    #print('fired photons ', no_photons)
    #print('area_beam cm^2 ', area_beam)
    #print('geometric_area cm^2 ', geometric_area)
    #print('Aeff single coinc. cm^2', Aeff_sct_abs)
    #print('Aeff single coinc. err cm^2', Aeff_sct_abs_err)
    #print('Aeff double coinc. cm^2', Aeff_sctX2_abs)
    #print('Aeff double coinc. err cm^2', Aeff_sctX2_abs_err)
    #print('Aeff tot coinc. cm^2', Aeff_tot)
    #print('Aeff tot coinc. err cm^2', Aeff_tot_err)

    #plt.figure(num=str(energy), figsize=(10, 8))
    #g=plt.hist(energy_sct, bins=n_bins,range=(en_min,en_max), histtype='step', label='Scat.')
    #g=plt.hist(energy_abs, bins=n_bins,range=(en_min, en_max), histtype='step', label='Abs.')
    #g=plt.hist(energy_sum, bins=n_bins,range=(en_min, en_max), histtype='step', label='Sum.')
    #plt.xlabel('Energy (keV)')
    #plt.ylabel('Counts ')
    #plt.legend(loc='upper left')
    #plt.title('Single coincidences - 1 Scat. + 1 Abs. - '+str(energy)+' keV')
    #plt.yscale('log')
    #return Aeff_sct_abs, Aeff_sct_abs_err, Aeff_sctX2_abs, Aeff_sctX2_abs_err, Aeff_tot, Aeff_tot_err



def sct_abs_coinc(EventList,energy=100.,no_photons = 10e+6, area_beam=144, geometric_area=10.,
                  n_bins = 500,en_max=100.,en_min=0.,en_threshold_emax=200.,en_threshold_emin=0., plots_label='sct_abs'):
    '''
    energy/keV
    area_beam/cm^2
    geometric_area/cm^2
    '''
    energy_sct=[]
    energy_abs=[]
    energy_sum=[]
    count_1=0
    count_2=0
    #Scatt-Abs coinc
    for evt in list(EventList.keys()):
        #Scatterer-Absorber
        if( (len(EventList[evt]['Scint_ID'])==2)and\
        (EventList[evt]['Scint_ID'][0]<100)and\
        (EventList[evt]['Scint_ID'][1]>100)and\
        (EventList[evt]['En_dep'][0]<en_threshold_emax)and\
        (EventList[evt]['En_dep'][0]>en_threshold_emin)
        ):
                count_1+=1
                energy_sct.append(EventList[evt]['En_dep'][0])
                energy_abs.append(EventList[evt]['En_dep'][1])
                energy_sum.append(EventList[evt]['En_dep'][0]+EventList[evt]['En_dep'][1])
        #to take in to account the order exchange in the file
        if( (len(EventList[evt]['Scint_ID'])==2)and\
        (EventList[evt]['Scint_ID'][0]>100)and\
        (EventList[evt]['Scint_ID'][1]<100)and\
        (EventList[evt]['En_dep'][1]<en_threshold_emax)and\
        (EventList[evt]['En_dep'][1]>en_threshold_emin)
        ):
                count_2+=1
                energy_sct.append(EventList[evt]['En_dep'][1])
                energy_abs.append(EventList[evt]['En_dep'][0])
                energy_sum.append(EventList[evt]['En_dep'][0]+EventList[evt]['En_dep'][1])
    #print('count 1', count_1)
    #print('count 2', count_2)
    print('tot coinc scat+abs', count_1+count_2)
    #Effective Area
    count_1_2=count_1+count_2
    count_1_2_err=count_1_2**0.5
    Aeff_sct_abs=area_beam*((count_1_2)/no_photons)
    Aeff_sct_abs_err = (area_beam/no_photons)*(count_1_2**0.5)  
    print('')
    print('energy ', energy)
    print('fired photons ', no_photons)
    print('area_beam cm^2 ', area_beam)
    print('geometric_area cm^2 ', geometric_area)
    print('Aeff single coinc. cm^2', Aeff_sct_abs)
    print('Aeff single coinc. err cm^2', Aeff_sct_abs_err)
    plt.figure(num=plots_label+' '+str(energy), figsize=(10, 8))
    g=plt.hist(energy_sct, bins=n_bins,range=(en_min,en_max), histtype='step', label='Scat.')
    g=plt.hist(energy_abs, bins=n_bins,range=(en_min, en_max), histtype='step', label='Abs.')
    g=plt.hist(energy_sum, bins=n_bins,range=(en_min, en_max), histtype='step', label='Sum.')
    plt.xlabel('Energy (keV)')
    plt.ylabel('Counts ')
    plt.legend(loc='upper left')
    plt.title('Single coincidences - 1 Scat. + 1 Abs. - '+str(energy)+' keV')
    plt.yscale('log')
    return Aeff_sct_abs, Aeff_sct_abs_err, count_1_2, count_1_2_err



def sctX2_abs_coinc(EventList,energy=100.,no_photons = 10e+6, area_beam=144, geometric_area=10.,
                  n_bins = 500,en_max=100.,en_min=0.,en_threshold_emax=200.,en_threshold_emin=0., plots_label='sctX2_abs'):
    '''
    energy/keV
    area_beam/cm^2
    geometric_area/cm^2
    '''
    energy_sct1=[]
    energy_sct2=[]
    energy_abs=[]
    energy_sum=[]
    count_1=0
    count_2=0
    count_3=0
    #Scatt-Abs coinc
    for evt in list(EventList.keys()):    
        #Double Coincidence: Scatterer-Scatterer-Absorber
        if( (len(EventList[evt]['Scint_ID'])==3)and\
        (EventList[evt]['Scint_ID'][0]<100)and\
        (EventList[evt]['Scint_ID'][1]<100)and\
        (EventList[evt]['Scint_ID'][2]>100)and\
        (EventList[evt]['En_dep'][0]<en_threshold_emax)and\
        (EventList[evt]['En_dep'][0]>en_threshold_emin)and\
        (EventList[evt]['En_dep'][1]<en_threshold_emax)and\
        (EventList[evt]['En_dep'][1]>en_threshold_emin)
        ):
            count_1+=1
            energy_sct1.append(EventList[evt]['En_dep'][0])
            energy_sct2.append(EventList[evt]['En_dep'][1])
            energy_abs.append(EventList[evt]['En_dep'][2])
            energy_sum.append(EventList[evt]['En_dep'][0]+EventList[evt]['En_dep'][1]+EventList[evt]['En_dep'][2])
        if( (len(EventList[evt]['Scint_ID'])==3)and\
        (EventList[evt]['Scint_ID'][0]<100)and\
        (EventList[evt]['Scint_ID'][1]>100)and\
        (EventList[evt]['Scint_ID'][2]<100)and\
        (EventList[evt]['En_dep'][0]<en_threshold_emax)and\
        (EventList[evt]['En_dep'][0]>en_threshold_emin)and\
        (EventList[evt]['En_dep'][2]<en_threshold_emax)and\
        (EventList[evt]['En_dep'][2]>en_threshold_emin)
        ):
            count_2+=1
            energy_sct1.append(EventList[evt]['En_dep'][0])
            energy_sct2.append(EventList[evt]['En_dep'][2])
            energy_abs.append(EventList[evt]['En_dep'][1])
            energy_sum.append(EventList[evt]['En_dep'][0]+EventList[evt]['En_dep'][1]+EventList[evt]['En_dep'][2])
        if( (len(EventList[evt]['Scint_ID'])==3)and\
        (EventList[evt]['Scint_ID'][0]>100)and\
        (EventList[evt]['Scint_ID'][1]<100)and\
        (EventList[evt]['Scint_ID'][2]<100)and\
        (EventList[evt]['En_dep'][1]<en_threshold_emax)and\
        (EventList[evt]['En_dep'][1]>en_threshold_emin)and\
        (EventList[evt]['En_dep'][2]<en_threshold_emax)and\
        (EventList[evt]['En_dep'][2]>en_threshold_emin)
        ):
            count_3+=1
            energy_sct1.append(EventList[evt]['En_dep'][1])
            energy_sct2.append(EventList[evt]['En_dep'][2])
            energy_abs.append(EventList[evt]['En_dep'][0])
            energy_sum.append(EventList[evt]['En_dep'][0]+EventList[evt]['En_dep'][1]+EventList[evt]['En_dep'][2])                          
    print('count 1', count_1)
    print('count 2', count_2)
    print('count 3', count_3)
    print('tot ', count_1+count_2+count_3)
    #Effective Area
    count_1_2_3=count_1+count_2+count_3
    count_1_2_3_err=count_1_2_3**0.5
    Aeff_sctX2_abs=area_beam*((count_1_2_3)/no_photons)
    Aeff_sctX2_abs_err=(area_beam/no_photons)*(count_1_2_3**0.5)
    print('')
    print('energy ', energy)
    print('fired photons ', no_photons)
    print('area_beam cm^2 ', area_beam)
    print('geometric_area cm^2 ', geometric_area)
    print('Aeff double coinc. cm^2', Aeff_sctX2_abs)
    print('Aeff double coinc. err cm^2', Aeff_sctX2_abs_err)
    plt.figure(num=plots_label+' '+str(energy), figsize=(10, 8))
    g=plt.hist(energy_sct1, bins=n_bins,range=(en_min,en_max), histtype='step', label='Scat. 1')
    g=plt.hist(energy_sct2, bins=n_bins,range=(en_min,en_max), histtype='step', label='Scat. 2')
    g=plt.hist(energy_abs, bins=n_bins,range=(en_min, en_max), histtype='step', label='Abs.')
    g=plt.hist(energy_sum, bins=n_bins,range=(en_min, en_max), histtype='step', label='Sum.')
    plt.xlabel('Energy (keV)')
    plt.ylabel('Counts ')
    plt.legend(loc='upper left')
    plt.title('Single coincidences - 1 Scat. + 1 Scat.+ 1 Abs. - '+str(energy)+' keV')
    plt.yscale('log')
    return Aeff_sctX2_abs, Aeff_sctX2_abs_err, count_1_2_3, count_1_2_3_err


def sctX2_coinc(EventList,energy=100.,no_photons = 10e+6, area_beam=144, geometric_area=10.,
                  n_bins = 500,en_max=100.,en_min=0.,en_threshold_emax=200.,en_threshold_emin=0., plots_label='sctX2'):
    '''
    energy/keV
    area_beam/cm^2
    geometric_area/cm^2
    '''
    energy_sct1=[]
    energy_sct2=[]
    energy_sum=[]
    count_1=0
    #Scatt-Abs coinc
    for evt in list(EventList.keys()):
        #Scatterer-Absorber
        if( (len(EventList[evt]['Scint_ID'])==2)and\
        (EventList[evt]['Scint_ID'][0]<100)and\
        (EventList[evt]['Scint_ID'][1]<100)and\
        (EventList[evt]['En_dep'][0]<en_threshold_emax)and\
        (EventList[evt]['En_dep'][0]>en_threshold_emin)and\
        (EventList[evt]['En_dep'][1]<en_threshold_emax)and\
        (EventList[evt]['En_dep'][1]>en_threshold_emin)
        ):
                count_1+=1
                energy_sct1.append(EventList[evt]['En_dep'][0])
                energy_sct2.append(EventList[evt]['En_dep'][1])
                energy_sum.append(EventList[evt]['En_dep'][0]+EventList[evt]['En_dep'][1])
    count_1_err=count_1**0.5
    Aeff_sctX2=area_beam*((count_1)/no_photons)
    Aeff_sctX2_err = (area_beam/no_photons)*(count_1**0.5)
    print('Coinc 2 scatterers ', count_1)
    print('')
    print('energy ', energy)
    print('fired photons ', no_photons)
    print('area_beam cm^2 ', area_beam)
    print('geometric_area cm^2 ', geometric_area)
    print('Aeff x2 scaterers cm^2', Aeff_sctX2)
    print('Aeff x2 scaterers err cm^2', Aeff_sctX2_err)
    plt.figure(num=plots_label+' '+str(energy), figsize=(10, 8))
    g=plt.hist(energy_sct1, bins=n_bins,range=(en_min,en_max), histtype='step', label='Scat. 1')
    g=plt.hist(energy_sct2, bins=n_bins,range=(en_min,en_max), histtype='step', label='Scat. 2')
    g=plt.hist(energy_sum, bins=n_bins,range=(en_min, en_max), histtype='step', label='Sum.')
    plt.xlabel('Energy (keV)')
    plt.ylabel('Counts ')
    plt.legend(loc='upper left')
    plt.title('Single coincidences - 1 Scat. + 1 Scat. - '+str(energy)+' keV')
    plt.yscale('log')
    return Aeff_sctX2, Aeff_sctX2_err, count_1, count_1_err



def sctX3_coinc(EventList,energy=100.,no_photons = 10e+6, area_beam=144, geometric_area=10.,
                  n_bins = 500,en_max=100.,en_min=0.,en_threshold_emax=200.,en_threshold_emin=0., plots_label='sctX3'):
    '''
    energy/keV
    area_beam/cm^2
    geometric_area/cm^2
    '''
    energy_sct1=[]
    energy_sct2=[]
    energy_sct3=[]
    energy_sum=[]
    count_1=0
    #Scatt-Abs coinc
    for evt in list(EventList.keys()):
        #Scatterer-Absorber
        if( (len(EventList[evt]['Scint_ID'])==3)and\
        (EventList[evt]['Scint_ID'][0]<100)and\
        (EventList[evt]['Scint_ID'][1]<100)and\
        (EventList[evt]['Scint_ID'][2]<100)and\
        (EventList[evt]['En_dep'][0]<en_threshold_emax)and\
        (EventList[evt]['En_dep'][0]>en_threshold_emin)and\
        (EventList[evt]['En_dep'][1]<en_threshold_emax)and\
        (EventList[evt]['En_dep'][1]>en_threshold_emin)and\
        (EventList[evt]['En_dep'][2]<en_threshold_emax)and\
        (EventList[evt]['En_dep'][2]>en_threshold_emin)
        ):
                count_1+=1
                energy_sct1.append(EventList[evt]['En_dep'][0])
                energy_sct2.append(EventList[evt]['En_dep'][1])
                energy_sct3.append(EventList[evt]['En_dep'][2])
                energy_sum.append(EventList[evt]['En_dep'][0]+EventList[evt]['En_dep'][1]++EventList[evt]['En_dep'][2])
    count_1_err=count_1**0.5
    Aeff_sctX3=area_beam*((count_1)/no_photons)
    Aeff_sctX3_err = (area_beam/no_photons)*(count_1**0.5)
    print('Coinc 3x scatterers ', count_1)
    print('')
    print('energy ', energy)
    print('fired photons ', no_photons)
    print('area_beam cm^2 ', area_beam)
    print('geometric_area cm^2 ', geometric_area)
    print('Aeff x2 scaterers cm^2', Aeff_sctX3)
    print('Aeff x2 scaterers err cm^2', Aeff_sctX3_err)
    plt.figure(num=plots_label+' '+str(energy), figsize=(10, 8))
    g=plt.hist(energy_sct1, bins=n_bins,range=(en_min,en_max), histtype='step', label='Scat. 1')
    g=plt.hist(energy_sct2, bins=n_bins,range=(en_min,en_max), histtype='step', label='Scat. 2')
    g=plt.hist(energy_sct3, bins=n_bins,range=(en_min,en_max), histtype='step', label='Scat. 3')
    g=plt.hist(energy_sum, bins=n_bins,range=(en_min, en_max), histtype='step', label='Sum.')
    plt.xlabel('Energy (keV)')
    plt.ylabel('Counts ')
    plt.legend(loc='upper left')
    plt.title('Coinc 3x Scat. - '+str(energy)+' keV')
    plt.yscale('log')
    return Aeff_sctX3, Aeff_sctX3_err, count_1, count_1_err


def sct_one_interaction(EventList,energy=100.,no_photons = 10e+6, area_beam=144, geometric_area=10.,
                  n_bins = 500,en_max=100.,en_min=0.,en_threshold_emax=200.,en_threshold_emin=0., plots_label='sct_photoelectric_abs'):
    '''
    energy/keV
    area_beam/cm^2
    geometric_area/cm^2
    '''
    energy_sct=[]
    count_1=0
    #Scatt-Abs coinc
    for evt in list(EventList.keys()):
        #Scatterer-Absorber
        if( (len(EventList[evt]['Scint_ID'])==1)and\
        (EventList[evt]['Scint_ID'][0]<100)and\
        (EventList[evt]['En_dep'][0]<en_threshold_emax)and\
        (EventList[evt]['En_dep'][0]>en_threshold_emin)
        ):      
                count_1+=1
                energy_sct.append(EventList[evt]['En_dep'][0])
    print('Only 1 Scat. interaction', count_1)
    count_1_err=count_1**0.5
    #Effective Area
    Aeff_sct_photoabs=area_beam*((count_1)/no_photons)
    Aeff_sct_photoabs_err = (area_beam/no_photons)*(count_1**0.5)  
    print('')
    print('energy ', energy)
    print('fired photons ', no_photons)
    print('area_beam cm^2 ', area_beam)
    print('geometric_area cm^2 ', geometric_area)
    print('Aeff single coinc. cm^2', Aeff_sct_photoabs)
    print('Aeff single coinc. err cm^2', Aeff_sct_photoabs_err)
    plt.figure(num=plots_label+' '+str(energy), figsize=(10, 8))
    g=plt.hist(energy_sct, bins=n_bins,range=(en_min,en_max), histtype='step', label='Scat. Photoelectric abs')
    plt.xlabel('Energy (keV)')
    plt.ylabel('Counts ')
    plt.legend(loc='upper left')
    plt.title('Only 1 Scat. interaction- '+str(energy)+' keV')
    plt.yscale('log')
    return Aeff_sct_photoabs, Aeff_sct_photoabs_err, count_1, count_1**0.5


def bars_mapping(filename,log_y=False,plots_label='bars mapping'):
    plt.figure(num=plots_label, figsize=(10, 8))
    hdul = fits.open(filename)
    hdul.info()
    events = hdul[1].data
    t_ID = events['Scint_ID']
    g=plt.hist(t_ID,bins=max(t_ID), range=(0,max(t_ID)), log=log_y,histtype='step', label='mapping', align='mid')
    plt.xlabel('Scint_ID')
    plt.ylabel('Events')
    x=filename.split('/')
    print(x[-1])
    plt.title(x[-1])
           
    
    


