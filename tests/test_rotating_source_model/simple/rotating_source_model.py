import sys
import pandas as pd # type: ignore
import numpy as np

def main():
    """Main function called when executing the script."""
    if len(sys.argv) == 1:
        energy_input = 100 # Default value (100 keV)
        photon_rate = 10.0 # Default value (10 photons per second)
        exposure_time = 10.0 # Default value (10 seconds)
    elif  len(sys.argv) == 2 and sys.argv[1] == "help":
          print("USAGE")
          print("python rotating_source_model.py <energy_input> <photon_rate> <exposure_time>")
          sys.exit()
    elif len(sys.argv) == 4:
        energy_input = float(sys.argv[1])
        print(f"Energy: {energy_input} keV")
        photon_rate = float(sys.argv[2])
        print(f"Photon rate: {photon_rate} photons per second")
        exposure_time = float(sys.argv[3])
        print(f"Exposure time: {exposure_time} seconds")
        print()
    else:
        print("Invalid number of arguments. Please run 'python generate_source.py help' for usage.")
        sys.exit()
    
          
    print("Running main function...")
    result = generate_events(energy_input, photon_rate, exposure_time)

    # save the result to a file
    result.to_csv("rsm-events.csv", index=False, float_format='%.6f')
    print("Events saved to rsm-events.csv")

def generate_events(energy_input, photon_rate, exposure_time):
    """
    Generate events for a given photon energy, rate, and exposure time.

    Parameters:
    energy (str): The energy in keV for which to generate events.
    photon_rate (float): The rate of photons per second.
    exposure_time (float): The exposure time in seconds.

    Returns:
    DataFrame: A pandas DataFrame containing the following columns:
        - event_id: Array of event IDs.
        - time: Array of event times.
        - angle: Array of event angles.
        - pol_x: Array of polarization x-components.
        - pol_y: Array of polarization y-components.
        - pol_z: Array of polarization z-components.
        - energy: Array of event energies.
    """

    energy_value = float(energy_input)
    N = int(photon_rate * exposure_time) # Calculate the number of photons
    event_id = np.arange(N)
    
    # Simulate Poissonian process for photon generation times
    time_intervals = np.random.exponential(1.0 / photon_rate, N)
    time = np.cumsum(time_intervals) # Cumulative sum to get event times
    
    omega = 0.01 * (2. * np.pi) # angular velocity (e.g. 2. * np.pi for one rotation per second)
    angle = omega * time
    pol_x = np.zeros(N)
    pol_y = np.zeros(N)
    pol_z = np.zeros(N)
    energy = energy_input * np.ones(N)

    # Estimate the polarization components
    # This defines a rotating vector in the zx plane from which we can calculate 
    # the polarization components in the GEANT4 code by the cross 
    # product with the momentum vector of the photon.
    pol_x = np.cos(angle)
    pol_z = np.sin(angle)

    data = {
        'event_id': event_id,
        'time': time,
        'angle': angle,
        'pol_x': pol_x,
        'pol_y': pol_y,
        'pol_z': pol_z,
        'energy': energy,
    }
    
    df = pd.DataFrame(data)

    print(f"{N} photons with energy = {energy_input} keV each have been generated.")
    
    return df

if __name__ == "__main__":
    main()
