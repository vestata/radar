# radar
## Quickstart
### Step 1
You can build the library, the example applications, and the tools using cmake. Simply create a build directory inside the Radar SDK directory, run cmake, and build the software:
```
mkdir build     # create build directory
cd build        # change into the directory build
cmake ..        # run cmake and generate build
cmake --build . # build all targets
```
For more information about building the Radar SDK from source code see the page 
“Building SDK from source code” in th

### Step 2
```
cd ./build/examples/c/BGT60LTR11AIP/raw_data
make
```

### Step 3
```
cd ./build/example/bin/BGT60LTR11AIP_raw_data_ltr11
sudo ./BGT60LTR11AIP_raw_data_ltr11
```
(us `lsusb` to check if the device is correctly connected. Often times `Failed to open device: no compatible device found (IFX_ERROR_NO_DEVICE)` happens, just execute the command several times, it should work.)

## Radar SDK

The Radar SDK is used to evaluate the following XENSIV™ Radar sensors.:

- FMCW Radars
    - BGT60 UTR11AIP
    - BGT60 TR13C
    - BGT60 ATR24C
- Doppler Radars
    - BGT60 LTR11AIP

Radar SDK allows users to configure and fetch raw data from above mentioned XENSIV™ radar sensors using C/C++, Python and Matlab programming languages. In addition, it contains sophisticated propriety algorithms to solve target detection and localization use-cases, and some useful tools to e.g. data recording and flash firmware etc.

Following sub-sections provide a brief explanation of each component of the Radar SDK.

### Radar SDK Library

The Radar SDK library is a C/C++ library providing functions to configure radar sensors and fetch raw data. Downloaded package already contains prebuilt libs for Windows, Mac, Linux platforms. In addition, source code for the core features is also part of the download package, from which users can build software for any platform.

The SDK library also comes with some of the sophisticated propriety algorithms like Presence Sensing, Segmentation Seamless Tracking, and Motion-Angle. Please be aware that the source code for these algorithms is not available, and as such, they can only be used on officially supported platforms. If you wish to run these algorithms on a different platform, please contact us, and we will provide support.

The Radar SDK library is required if you intend to develop applications in C or C++. The Radar SDK library is also a dependency of the Matlab and Python wrappers.

### Example Applications in C

The example applications illustrate the usage of the Radar SDK library and offer command line tools that allow to run algorithms. The following examples are valid for respective sensor under examples/c/ folder.

- BGT60 UTR11AIP
    - fmcw/raw_data
    - cw/continuous_wave
    - presence_sensing
- BGT60 TR13C
    - fmcw/raw_data
    - cw/continuous_wave
    - presence_sensing
    - segmentation
- BGT60 ATR24C
    - fmcw/raw_data
    - cw/continuous_wave
- BGT60 LTR11AIP
    - raw_data
    - advanced_motion_sensing

### Python Wrapper

The ifxradarsdk Python wrapper allows you from Python to configure radar sensors and fetch the raw ADC data. This wrapper supports all XENSIV™ radar sensors.

The example scripts under examples/py/ folder illustrate how to use the ifxradarsdk Python wrapper. Users can learn how to compute and plot various radar data outputs, including:

Range-Doppler map (range_doppler_map.py)
Distance FFT (distance_fft.py)
Range-Angle map (range_angle_map.py) using Digital Beamforming
Basic presence sensing (presence_sensing.py)
In all cases, the focus is on illustrating the use of the ifxradarsdk Python wrapper and explaining basic radar algorithms.

### Matlab Wrapper

The Avian Matlab Wrapper allows you from Matlab to configure XENSIV™ FMCW Radars and fetch the time-domain raw ADC signal.

The example script, under examples/matlab/ folder, implements a basic presence sensing algorithm for XENSIV™ FMCW Radars.

### Documentation

The documentation of the Radar SDK contains, among others, usage information about the Radar SDK Library, the ifxradarsdk Python wrappers, the Matlab wrapper, instructions on how to build the Radar SDK from source code, the specification of the JSON configuration schema, and guidelines how to choose the right sensor configuration.

The Radar SDK is officially supported and tested on the following platforms:

- Windows 10, x86-64
- Windows 11, x86-64
- Ubuntu 22.04, x86-64
- Raspbian Buster, armhf (Raspberry Pi Model B or later)
Other platforms (especially on Linux) typically work as well. For these platforms you need to build the Radar SDK yourself. You can find instructions how to build the Radar SDK as part of the Radar SDK documentation.


## Quickstart
### C Library

### Python Wrapper
Make sure that you have Python 3.8 or above installed. The Python wheel of the SDK Python wrapper is in the directory libs/platform/ where platform corresponds to your operating system. Install the wheel using pip:

```pip install ifxradarsdk-3.5.0-py3-none-any.whl```
You can import the use Python wrapper to connect to a sensor, and fetch raw data with following code:

```python
# import the Python wrapper
from ifxradarsdk import get_version
from ifxradarsdk.fmcw import DeviceFmcw
from ifxradarsdk.fmcw.types import create_dict_from_sequence

# connect to an FMCW 60GHz radar sensor e.g. BGT60TR13C or BGT60UTR11AIP
with DeviceFmcw() as device:
    print("Radar SDK Version: " + get_version())
    print("UUID of board: " + device.get_board_uuid())
    print("Sensor: " + str(device.get_sensor_type()))

# A device instance is initialized with the default acquisition
    # sequence for its corresponding radar sensor. This sequence can be
    # simply fetched, analyzed or modified by the user.
    sequence = device.get_acquisition_sequence()

# Fetch a number of frames
    for frame_number in range(10):
        frame_contents = device.get_next_frame()

        for frame in frame_contents:
            num_rx = np.shape(frame)[0]

            # Do some processing with the obtained frame.
            # In this example we just dump it into the console
            print("Frame " + format(frame_number) + ", num_antennas={}".format(num_rx))

            for iAnt in range(num_rx):
                mat = frame[iAnt, :, :]
                print("Antenna", iAnt, "\n", mat)
```

For more information about the FMCW Python wrapper refer to the page “Python wrapper usage” in the Radar SDK documentation, and also have a look at the example applications in the directory `apps/py/examples/`. Please note that commands slightly differs on platforms other than Windows.

### Recording Tool (ifxdaq)
You can easily install ifxdaq using pip. First unzip the ifxdaq.zip and open a shell or cmd prompt in the unzipped folder. Simply create a virtual environment, install the SDK Python wrapper, install ifxdaq, and start the recorder.

For instance, on Windows you can install ifxdaq and start ifxrecorder using:

```python
# create virtual environment
python -m venv .venv

# activate virtual environment; command differs on platforms other than Windows
.\.venv\Scripts\Activate.ps1

# make sure pip, wheel, and setuptools are installed and up-to-date
python -m pip install --upgrade pip wheel setuptools

# install Python wrapper from the Radar SDK
pip install /path/to/radar_sdk/python_wheels/ifxradarsdk-XXX-win_amd64.whl

# install ifxdaq
pip install ifxdaq-4.0.0-py3-none-any.whl

# start ifxrecorder
ifxdaq recorder
```