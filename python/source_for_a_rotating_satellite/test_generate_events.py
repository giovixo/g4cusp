import pytest # type: ignore
import pandas as pd # type: ignore
from rotating_source_model import generate_events

def test_generate_events_valid_input():
    energy_input = 100.0
    photon_rate = 10.0
    exposure_time = 10.0
    df = generate_events(energy_input, photon_rate, exposure_time)
    
    assert isinstance(df, pd.DataFrame)
    assert len(df) == int(photon_rate * exposure_time)
    assert set(df.columns) == {'event_id', 'time', 'angle', 'pol_x', 'pol_y', 'pol_z', 'energy'}
    assert all(df['energy'] == energy_input)

# Test if the function works with a zero exposure time
def test_generate_events_zero_exposure_time():
    energy_input = 100.0
    photon_rate = 10.0
    exposure_time = 0.0
    df = generate_events(energy_input, photon_rate, exposure_time)
    
    assert isinstance(df, pd.DataFrame)
    assert len(df) == 0
    assert set(df.columns) == {'event_id', 'time', 'angle', 'pol_x', 'pol_y', 'pol_z', 'energy'}