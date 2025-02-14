import sys
import pandas as pd # type: ignore
import numpy as np

def main():
    """Main function called when executing the script."""
    if len(sys.argv) > 1:
        energy_input = sys.argv[1]
    else:
        energy_input = "100" # Default value (100 keV)
    
    print("Running main function...")
    result = generate_events(energy_input)

    # save the result to a file
    result.to_csv("events.csv", index=False)
    print("Events saved to events.csv")

def generate_events(energy_input):
    """
    Generate events for a given photon energy

    Parameters:
    energy (str): The energy in keV for which to generate events.

    Returns:
    DataFrame: A pandas DataFrame containing the following columns:
        - event_id: Array of event IDs.
        - time: Array of event times.
        - angle: Array of event angles.
        - energy: Array of event energies.
        - polarisation: Array of event polarisations.
    """

    energy_value = float(energy_input)
    event_id = np.arange(100)
    time = np.zeros(100)
    angle = np.zeros(100)
    energy = energy_value * np.ones(100)
    polarisation = np.zeros(100)

    data = {
        'event_id': event_id,
        'time': time,
        'angle': angle,
        'energy': energy,
        'polarisation': polarisation
    }
    
    df = pd.DataFrame(data)

    print(f"Events for {energy_input} keV photons generated.")
    
    return df

if __name__ == "__main__":
    main()
