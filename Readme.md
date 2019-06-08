![SDR Angel banner](doc/img/sdrangel_banner.png)

**SDRangel** is an Open Source Qt5 / OpenGL 3.0+ SDR and signal analyzer frontend to various hardware.

**Check the discussion group** [here](https://groups.io/g/sdrangel)

&#9888; SDRangel is intended for the power user. We expect you to already have some experience with SDR applications and digital signal processing in general. SDRangel might be a bit overwhelming for you however you are encouraged to use the discussion group above to look for help. You can also find more information in the [Wiki](https://github.com/f4exb/sdrangel/wiki).

<h1>Hardware requirements</h1>

SDRangel is a near real time application that is demanding on CPU power and clock speeds for low latency. Recent (2015 or later) Core i7 class CPU is recommended preferably with 4 HT CPU cores (8 logical CPUs or more) with nominal clock over 2 GHz and at least 8 GB RAM. Modern Intel processors will include a GPU suitable for proper OpenGL support. On the other hand SDRangel is not as demanding as recent computer games for graphics and CPU integrated graphics are perfectly fine. USB-3 ports are also preferable for high speed, low latency USB communication. It may run on beefy SBCs and was tested successfully on a Udoo Ultra.

The server variant does not need a graphical display and therefore OpenGL support. It can run on smaller devices check the Wiki for hardware requirements and a list of successful cases.

<h1>Source code</h1>

<h2>Repository branches</h2>

- cmake: build system refactory with many patches to build the code on every platform
- master: the production branch
- dev: the development branch
- legacy: the modified code from the parent application [hexameron rtl-sdrangelove](https://github.com/hexameron/rtl-sdrangelove) before a major redesign of the code was carried out and sync was lost.

<h2>Untested plugins</h2>

These plugins come from the parent code base and have been maintained so that they compile but they are not being actively tested:

channelrx:

  - demodlora

<h1>Specific features</h1>

<h2>Multiple device support</h2>

Since version 2 SDRangel can integrate more than one hardware device running concurrently.

<h2>Transmission support</h2>

Since version 3 transmission or signal generation is supported for BladeRF, HackRF (since version 3.1), LimeSDR (since version 3.4) and PlutoSDR (since version 3.7.8) using a sample sink plugin. These plugins are:

  - [BladeRF1 output plugin](https://github.com/f4exb/sdrangel/tree/dev/plugins/samplesink/bladerf1output)
  - [BladeRF2 output plugin](https://github.com/f4exb/sdrangel/tree/dev/plugins/samplesink/bladerf2output)
  - [HackRF output plugin](https://github.com/f4exb/sdrangel/tree/dev/plugins/samplesink/hackrfoutput)
  - [LimeSDR output plugin](https://github.com/f4exb/sdrangel/tree/dev/plugins/samplesink/limesdroutput)
  - [PlutoSDR output plugin](https://github.com/f4exb/sdrangel/tree/dev/plugins/samplesink/plutosdroutput)
  - [XTRX output plugin](https://github.com/f4exb/sdrangel/tree/dev/plugins/samplesink/xtrxoutput) Experimental.
  - [File output or file sink plugin](https://github.com/f4exb/sdrangel/tree/dev/plugins/samplesink/filesink)
  - [Remote device via Network](https://github.com/f4exb/sdrangel/tree/dev/plugins/samplesink/remoteoutput) Linux only

<h2>REST API</h2>

Since version 4 a REST API is available to interact with the SDRangel application. More details are provided in the server instance documentation in the `sdrsrv` folder.

<h2>Server instance</h2>

Since version 4 the `sdrangelsrv` binary launches a server mode SDRangel instance that runs wihout the GUI. More information is provided in the Readme file of the `sdrsrv` folder.

<h2>Detached RF head server</h2>

Since version 4.1 the previously separated project SDRdaemon has been modified and included in SDRangel. The `sdrangelsrv` headless variant can be used for this purpose using the Remote source or sink channels.

<h1>Notes on pulseaudio setup</h1>

The audio devices with Qt are supported through pulseaudio and unless you are using a single sound chip (or card) with a single output port or you are an expert with pulseaudio config files you may get into trouble when trying to route the audio to a different output port. These notes are a follow-up of issue #31 with my own experiments with HDMI audio output on the Udoo x86 board. So using this example of HDMI output you can do the following:

  - Install pavucontrol. It is included in most distributions for example:
    - Ubuntu/Debian: `sudo apt-get install pavucontrol`
    - openSUSE: `sudo zypper in pavucontrol`
  - Check the audio config with alsamixer. On the Udoo x86 the HDMI output depends on the S/PDIF control and it occasionally gets muted when the HDMI monitor is turned off or goes to sleep. So in any case make sure nothing is muted there.
  - Open pavucontrol and open the last tab (rightmost) called 'Configuration'
  - Select HDMI from the profiles list in the 'Configuration' tab
  - Then in the 'Output devices' tab the HDMI / display port is selected as it is normally the only one. Just double check this
  - In SDRangel's Preferences/Audio menu make sure something like `alsa_output.pci-0000_00_1b.0.hdmi-stereo` is selected. The default might also work after the pulseaudio configuration you just made.

In case you cannot see anything related to HDMI or your desired audio device in pavucontrol just restart pulseaudio with `pulseaudio -k` (`-k` kills the previous instance before restarting) and do the above steps again.

# Build

## Generic

### Requirements

- cpu with SSSE3 (x86 architecture) or Neon (arm architecture)
- compiler that supports full c++11 specification (gcc, clang or msvc)
- cmake >= 3.1
- Qt >= 5.5 (this version is only to define a lower bound)
- Boost
- pkg-config
- fftw3f
- libusb
- opencv
- other libraries can be installed based on personal usage
  or can be built internally with `-DENABLE_EXTERNAL_LIBRARIES=ON`
  see CMake Options paragraph.

### Build

- done like every other cmake project
   
  ```
  mkdir build && cd build
  cmake .. [OPTIONS]
  make
  ```
     
- if you would like to install the project

  ```
  make install
  ```

### Usage
- run the binary `./sdrangel` for the gui or `./sdrangelsrv` for the server.
- all libs and plugins are on `libs/` and `${prefix}/lib/sdrangel` when install.

## Debian and Derivatives

### Requirements

for a full packages list you should check the following files:
  
  - .travis.yml for Ubuntu 16.04
  - .appveyor.yml for Ubuntu 18.04
  - debian/control for a basic one

then you need to install devices driver and optionally external libraries;
essentially there are two ways:

  - preferred mode: use the distribution one like

  ```
  sudo apt-get install airspy
  ```
  
  and if not provided use `cmake/ci/build_*.sh` scripts that build and
  install the desired software; remember that the build scripts
  use `${HOME}/external` as base directory.

  - use `-DENABLE_EXTERNAL_LIBRARIES=ON` as cmake option to build
    internally all dependencies and drivers. This can be used with debuild;
    take in consideration that this doesn't include udev rules yet.

### Build

  - cmake options can be declared with (on zsh/bash like shell)

    ```
    export CMAKE_CUSTOM_OPTIONS="-DFORCE_SSE41=ON"
    ```

  - then you can run the debian build package

    ```
    debuild --preserve-envvar CMAKE_CUSTOM_OPTIONS -i -us -uc -b
    ```

at the end of process you will find the deb package on ../

### Usage

when you have the .deb you can install the package on the system with

```
sudo dpkg -i sdrangel_*.deb
```

and you find a startup link on your menu. remove the package is easy

```
sudo apt-get remove sdrangel
```

## Windows

### Requirements

- Win7, Win10, Win server 2012 or 2016 64bit
- MSVC 2015/2017
- Qt installed on default directory or use `-DQT_PATH=C:\path_qt\` and `-DQT_MISSING=OFF` as parameters for cmake
- cmake (add to the path `SET PATH=%PATH%;d:\CMake\bin` if not present)
- nsis

### Build

- clone the repository with git (in this case)

  ```
  git clone -b cmake https://github.com/ra1nb0w/sdrangel.git
  ```

- enter on the repository with

  ```
  cd sdrangel
  ```

- get submodule that contains basic windows libraries already built

  ```
  git submodule update --init --recursive
  ```

- now prepare the environment (I use cmd.exe as terminal)

  ```
  set "CMAKE_GENERATOR=Visual Studio 15 2017 Win64"
  set "CMAKE_CUSTOM_OPTIONS=-DFORCE_SSE41=ON"
  ```
  
  use the following if you are using MSVC 2015

  ```
  set "CMAKE_GENERATOR=Visual Studio 14 2015 Win64"
  ```

  and if you want native cpu flags omits `CMAKE_CUSTOM_OPTIONS`;
  for more option, read the paragraph at the end of this e-mail.

- start the build and packaging phases

  ```
  cmake\ci\build_sdrangel.bat
  ```

- at the end of the build, and only if success, you obtain a .zip
  and an installer with sdrangel and sdrangelsrv, all plugins, all
  dependencies and all devices except perseus-sdr and soapysdrplay.
  Both contains vcdistr and the installer automatically install it
  if not found on the system.
  An installer example can be found at this url [1]; built on Win7
  with MSVC 2017.

### Usage

- the code can be run from bin/ folder, the folder on the zip or
  installed on the system with the installer; the last one install
  also an icon on your start menu.
  on your start menu.

### Notes

- this build is on par with linux features
- build tested only in 64bit mode
- basic library like, boost, opencv, libusb, are already built and
  can be found at external/windows
- all dependencies, like cm256cc are build from sources
- all devices are built from source and cmake install only the
  required library so can be cases where the device doesn't work. I
  only have rtlsdr, limesdr and sdrplay (not included yet) therefore
  test your hardware and post the error shown on console if something
  doesn't work.

## macOS

### Requirements

all dependencies and device drivers are available on macports [2] and
can be seen on .travis.yml; see line with `sudo port -N -k install`.
when all my changes will be on master I will push SDRangel on macports
so will be easy to install it; you will only need to run the following
command

```
sudo port install sdrangel
```

and you find an .app on Applications.<br>
For the moment you can use the following instructions

- first of all install macports following
  [macports install](https://www.macports.org/install.php); you also need XCode 10.

- create a temporary folder, like

  ```
  mkdir -p ~/tmp/SDRangel && cd ~/tmp/SDRangel
  ```

- download port file on that directory

  ```
  curl -O https://raw.githubusercontent.com/ra1nb0w/macports-ports/sdrangel/science/SDRangel/Portfile
  ```

- add your variants to the end of default_variants of Portfile like

  ```
  default_variants +soapysdr +rtlsdr +funcube +server +gui +native +limesuite
  ```

  remember to choose your device, see variant lines.

- then build and install with

  ```
  sudo port install
  ```

- at the end you have the app at /Applications/MacPorts/SDRangel.app

### Build

it is quite the same as generic

```
mkdir build && cd build
cmake ..
make
```

if you like to get .dmg with the .app and all dependencies included use the
following command

```
mkdir build && cd build
cmake .. -DBUNDLE=ON
make package
```

remember that this activate/include only libraries present on the
system; if you want to build everything without installing it with
macports (except required libraries) use the cmake option
`-DENABLE_EXTERNAL_LIBRARIES=ON`.

### Usage

- if built without bundle run it as generic `./sdrangel` or `./sdrangelsrv`
- if built with bundle, install SDRangel.app like all other software

### Notes

- when build the bundle for distribution takes into account the
  variable CMAKE_OSX_DEPLOYMENT_TARGET or use the minimum macOS
  version.
- if you build with macOS 10.14 and macports qt5-qtbase can be high
  cpu usage of sdrangel; this is an upstream bug that will be fixed.


## CMake options

there are many options for cmake and you can find it in the head of
CMakeLists.txt; them most important  are:

- `ENABLE_EXTERNAL_LIBRARIES` build and install useful libraries and
  devices driver; the list can be seen on external/CMakeLists.txt;<br>
  *default: off*
- `BUNDLE` generate the package at the end of the build; always enabled
  on windows and optionally on macOS. On Linux every distribution has
  its tools to create the package.<br>
  *default: off*
- `ENABLE_*` enable or disable device support;
  default: all are enabled and the build will be done if the library
  is discovered.<br>
  *default: on*
- `BUILD_SERVER` or `BUILD_GUI` to choose which binary build.<br>
  *default: both are enabled*
- `FORCE_SSSE3` or `FORCE_SSE41` to disable cpu flags auto-discovery; very
  useful when create a package that should be distributed.<br>
  *default: off* and auto-discovery is used
- `DEBUG_OUTPUT` show more log during execution; impact cpu usage<br>
  *default: off*
- `CMAKE_INSTALL_PREFIX` where to install the binaries and the libraries<br>
  *default: system dependent*
- `CMAKE_BUILD_TYPE` choose the type of the build; see
  https://cmake.org/cmake/help/latest/variable/CMAKE_BUILD_TYPE.html<br>
  *default: Release*

You can also use cmake-gui o ncurses frontend; from terminal you can
use

```
cmake -LAH
```

  to see all variables.

## Continuous integrator

I implemented the code and configuration to support two continuous
integrator [3] that verify that build and packing work on Ubuntu
16.04, Ubuntu 18.05, macOS (4 versions), Windows MSVC 2015 and MSVC
2017. In total they run 13 VMs.  This permit to check easily that your
changes doesn't break on some platforms. Testing is always boring and
error-prone so declare it facilitate the life. Further, you can use
the result (package) to auto deploy on github every time you tag the
code. Not done yet but very easy to implement.

Both use scripts present on the `cmake/ci/` folder.

### travis-ci

defined by .travis.yml and test the code on Ubuntu 16.04 and macOS
10.11, 10.12, 10.13 and 10.14..

### appveyor

introduced because support Ubuntu 18.04 and has Windows with Qt
already installed.
defined by .appveyor.yml and test the code con Windows Server 2016
with MSVC 2017, Windows Server 2012 with MSVC 2015 and Ubuntu 18.04.

## Final notes

- surely I forgot something to say so please don't esitate to ask.
- I did so many changes on the code to support all platforms that I
  don't remember therefore the best way to understand it is to grasp
  them from commit messages.
- Sorry for the uncorrelated/inconsistent commit messages; I promise
  to myself that at the end of the work, and so now, I would have
  reorganized them but they are already upstream so it is dangerous
  to touch it. Again, I am sorry.
- the new build system based on cmake should require a long
  explanation, also because it is a quite complex project with many
  libraries, plugins and all external dependencies, but for the moment
  I have not other time to dedicate so ping me with your questions.

<h1>Supported hardware</h1>

&#9758; Details on how to compile support libraries and integrate them in the final build of SDRangel are detailed in the [Wiki page for compilation in Linux](https://github.com/f4exb/sdrangel/wiki/Compile-from-source-in-Linux) mentioned earlier.

The notes below are left for complementary information

<h2>Airspy</h2>

[Airspy R2](https://airspy.com/airspy-r2/) and [Airspy Mini](https://airspy.com/airspy-mini/) are supported through the libairspy library that should be installed in your system for proper build of the software and operation support.

Please note that if you are using a recent version of libairspy (>= 1.0.6) the dynamic retrieval of sample rates is supported. To benefit from it you should modify the `plugins/samplesource/airspy/CMakeLists.txt` and change line `add_definitions(${QT_DEFINITIONS})` by `add_definitions("${QT_DEFINITIONS} -DLIBAIRSPY_DYN_RATES")`. In fact both lines are present with the last one commented out.

Be also aware that the lower rates (2.5 MS/s or 5 MS/s with modified firmware) are affected by a noise artifact so 10 MS/s is preferable for weak signal work or instrumentation. A decimation by 64 was implemented to facilitate narrow band work at 10 MS/s input rate.

<h2>Airspy HF+</h2>

[Airspy HF+](https://airspy.com/airspy-hf-plus/) is supported through [the airspyhf library](https://github.com/airspy/airspyhf).

It is recommended to add `-DRX_SAMPLE_24BIT=ON` on the cmake command line to activate the Rx 24 bit DSP build and take advantage of improved dynamic range when using decimation.

&#9758; From version 3.12.0 the Linux binaries are built with the 24 bit Rx option.

<h2>BladeRF classic (v.1)</h2>

[BladeRF1](https://www.nuand.com/bladerf-1) is supported through the libbladeRF library. Note that libbladeRF v2 is used since version 4.2.0 (git tag 2018.08).

&#9758; Please note that if you use your own library the FPGA image `hostedx40.rbf` or `hostedx115.rbf` shoud be placed in e.g. `/opt/install/libbladeRF/share/Nuand/bladeRF` unless you have flashed the FPGA image inside the BladeRF.

The plugins used to support BladeRF classic are specific to this version of the BladeRF:
  - [BladeRF1 input](https://github.com/f4exb/sdrangel/tree/dev/plugins/samplesource/bladerf1input)
  - [BladeRF1 output](https://github.com/f4exb/sdrangel/tree/dev/plugins/samplesink/bladerf1output)

<h2>BladeRF micro (v.2)</h2>

From version 4.2.0.

[BladeRF 2 micro](https://www.nuand.com/bladerf-2-0-micro/) is also supported using libbladeRF library.

The plugins used to support BladeRF 2 micro are specific to this version of the BladeRF:
  - [BladeRF2 input](https://github.com/f4exb/sdrangel/tree/dev/plugins/samplesource/bladerf2input)
  - [BladeRF2 output](https://github.com/f4exb/sdrangel/tree/dev/plugins/samplesink/bladerf2output)

<h2>FunCube Dongle</h2>

Linux only.

Both Pro and Pro+ are supported with the plugins in fcdpro and fcdproplus respectively. For the Pro+ the band filter selection is not effective as it is handled by the firmware using the center frequency.

The control interface is based on qthid and has been built in the software in the fcdhid library. You don't need anything else than libusb support. Library fcdlib is used to store the constants for each dongle type.

The Pro+ has trouble starting. The sound card interface is not recognized when you just plug it in and start SDRAngel. The workaround is to start qthid then a recording program like Audacity and start recording in Audacity. Then just quit Audacity without saving and quit qthid. After this operation the Pro+ should be recognized by SDRAngel until you unplug it.

<h2>HackRF</h2>

[HackRF](https://greatscottgadgets.com/hackrf/) is supported through the libhackrf library. Please note that you will need a recent version (2015.07.2 at least) of libhackrf that supports the sequential listing of devices and the antenna power control (bias tee). To be safe anyway you may choose to build and install the Github version: `https://github.com/mossmann/hackrf.git`. Note also that the firmware must be updated to match the library version as per instructions found in the HackRF wiki.

HackRF is better used with a sampling rate of 4.8 MS/s and above. The 2.4 and 3.2 MS/s rates are considered experimental and are way out of specs of the ADC. You may or may not achieve acceptable results depending on the unit. A too low sampling rate will typically create ghost signals (images) and/or raise the noise floor.

<h2>LimeSDR</h2>

[LimeSDR](https://myriadrf.org/projects/limesdr/) and its smaller clone LimeSDR Mini are supported using LimeSuite library (see next).

<p>&#9888; Version 18.10.0 of LimeSuite is used in the builds and corresponding gateware loaded in the LimeSDR should be is used. If you compile from source version 18.10.0 of LimeSuite must be used.</p>

<p>&#9888; LimeSDR-Mini seems to have problems with Zadig driver therefore it is supported in Linux only. A workaround is to use it through SoapySDR</p>

<h2>Perseus</h2>

Linux only.

[The Perseus](http://microtelecom.it/perseus/) is supported with [my fork of libperseus-sdr library](https://github.com/f4exb/libperseus-sdr.git) on the `fixes` branch which is the default. There are a few fixes from the original mainly to make it work in a multi-device context.

Please note that the Perseus input plugin will be built only if the 24 bit Rx DSP chain is activated in the compilation with the `-DRX_SAMPLE_24BIT=ON` option on the cmake command line.

&#9758; From version 3.12.0 the Linux binaries are built with the 24 bit Rx option and Perseus input plugin.

<h2>PlutoSDR</h2>

[PlutoSDR](https://wiki.analog.com/university/tools/pluto) is supported with the libiio interface. Be aware that version 0.16 or higher is needed. Also the device should be flashed with firmware 0.29 or higher.

<h2>RTL-SDR</h2>

RTL-SDR based dongles are supported through the librtlsdr library.

<h2>SDRplay RSP1</h2>

Linux only.

[SDRplay RSP1](https://www.sdrplay.com/rsp1/) is supported through the [libmirisdr-4](https://github.com/f4exb/libmirisdr-4) library found in this very same Github space. There is no package distribution for this library and you will have to clone it, build and install it in your system. However Debian packages of SDRangel contain a pre-compiled version of this library.

&#9888; The RSP1 has been discontinued in favor of RSP1A. Unfortunately due to their closed source nature RSP1A nor RSP2 can be supported natively in SDRangel. As a workaround you may use it through SoapySDR.

<h2>XTRX</h2>

Experimental and Linux only. Compile from source.

[XTRX](https://xtrx.io) is supported through the set of [xtrx libraries](https://github.com/xtrx-sdr/images).

Before starting SDRangel you have to add the library directory to the `LD_LIBRARY_PATH` variable for example with `export LD_LIBRARY_PATH=/opt/install/xtrx-images/lib:$LD_LIBRARY_PATH`.

&#9888; Reception may stall sometimes particularly with sample rates lower than 5 MS/s and also after changes. You may need to stop and restart the device (stop/start button) to recover.

&#9888; Right after (re)start you may need to move the main frequency dial back and forth if you notice that you are not on the right frequency.

&#9888; Simultaneous Rx and Tx is not supported. Dual Tx is not working either.

<h1>Plugins for special devices</h1>

These plugins do not use any hardware device connected to your system.

<h2>File input</h2>

The [File source plugin](https://github.com/f4exb/sdrangel/tree/dev/plugins/samplesource/filesource) allows the playback of a recorded IQ file. Such a file is obtained using the recording feature. Click on the record button on the left of the main frequency dial to toggle recording. The file has a fixed name `test_<n>.sdriq` created in the current directory where `<n>` is the device tab index.

Note that this plugin does not require any of the hardware support libraries nor the libusb library. It is always available in the list of devices as `FileSource[0]` even if no physical device is connected.

The `.sdriq` format produced are the 2x2 bytes I/Q samples with a header containing the center frequency of the baseband, the sample rate and the timestamp of the recording start. Note that this header length is a multiple of the sample size so the file can be read with a simple 2x2 bytes I/Q reader such as a GNU Radio file source block. It will just produce a short glitch at the beginning corresponding to the header data.

<h2>File output</h2>

The [File sink plugin](https://github.com/f4exb/sdrangel/tree/dev/plugins/samplesink/filesink) allows the recording of the I/Q baseband signal produced by a transmission chain to a file in the `.sdriq` format thus readable by the file source plugin described just above.

Note that this plugin does not require any of the hardware support libraries nor the libusb library. It is always available in the list of devices as `FileSink[0]` even if no physical device is connected.

<h2>Test source</h2>

The [Test source plugin](https://github.com/f4exb/sdrangel/tree/master/plugins/samplesource/testsource) is an internal continuous wave generator that can be used to carry out test of software internals.

<h2>Remote input</h2>

Linux only.

The [Remote input plugin](https://github.com/f4exb/sdrangel/tree/dev/plugins/samplesource/remoteinput) is the client side of an instance of SDRangel running the Remote Sink channel plugin. On the "Data" line you must specify the local address and UDP port to which the remote server connects and samples will flow into the SDRangel application (default is `127.0.0.1`port `9090`). It uses the meta data to retrieve the sample flow characteristics such as sample rate and receiving center frequency. The remote is entirely controlled by the REST API. On the "API" line you can specify the address and port at which the remote REST API listens. However it is used just to display basic information about the remote.

The data blocks transmitted via UDP are protected against loss with a Cauchy MDS block erasure codec. This makes the transmission more robust in particular with WiFi links.

There is an automated skew rate compensation in place. During rate readjustment streaming can be suspended or signal glitches can occur for about one second.

This plugin will be built only if the [CM256cc library](https://github.com/f4exb/cm256cc) is installed in your system.

Note that this plugin does not require any of the hardware support libraries nor the libusb library. It is always available in the list of devices as `RemoteInput` even if no physical device is connected.

<h2>Remote output</h2>

Linux only.

The [Remote output plugin](https://github.com/f4exb/sdrangel/tree/dev/plugins/samplesink/remoteoutput) is the client side of and instance of SDRangel running the Remote Source channel plugin. On the "Data" line you must specify the distant address and UDP port to which the plugin connects and samples from the SDRangel application will flow into the transmitter server (default is `127.0.0.1`port `9090`). The remote is entirely controlled by the REST API. On the "API" line you can specify the address and port at which the remote REST API listens. The API is pinged regularly to retrieve the status of the data blocks queue and allow rate control to stabilize the queue length. Therefore it is important to connect to the API properly (The status line must return "API OK" and the API label should be lit in green).

The data blocks sent via UDP are protected against loss with a Cauchy MDS block erasure codec. This makes the transmission more robust in particular with WiFi links.

This plugin will be built only if the [CM256cc library](https://github.com/f4exb/cm256cc) IS installed in your system.

Note that this plugin does not require any of the hardware support libraries nor the libusb library. It is always available in the list of devices as `RemoteOutput` even if no physical device is connected.

<h1>Channel plugins with special conditions</h1>

<h2>DSD (Digital Speech Decoder)</h2>

This is the `demoddsd` plugin. At present it can be used to decode the following digital speech formats:

  - DMR/MOTOTRBO
  - dPMR
  - D-Star
  - Yaesu System Fusion (YSF)
  - NXDN

It is based on the [DSDcc](https://github.com/f4exb/dsdcc) C++ library which is a rewrite of the original [DSD](https://github.com/szechyjs/dsd) program. So you will need to have DSDcc installed in your system. Please follow instructions in [DSDcc readme](https://github.com/f4exb/dsdcc/blob/master/Readme.md) to build and install DSDcc.

If you have one or more serial devices interfacing the AMBE3000 chip in packet mode you can use them to decode AMBE voice frames. For that purpose you will need to compile with [SerialDV](https://github.com/f4exb/serialDV) support. Please refer to this project Readme.md to compile and install SerialDV. Note that your user must be a member of group `dialout` (Ubuntu/Debian) or `uucp` (Arch) to be able to use the dongle.

Although such serial devices work with a serial interface at 400 kb in practice maybe for other reasons they are capable of handling only one conversation at a time. The software will allocate the device dynamically to a conversation with an inactivity timeout of 1 second so that conversations do not get interrupted constantly making the audio output too choppy. In practice you will have to have as many devices connected to your system as the number of conversations you would like to be handled in parallel.

Alternatively you can use [mbelib](https://github.com/szechyjs/mbelib) but mbelib comes with some copyright issues.

While DSDcc is intended to be patent-free, `mbelib` that it uses describes functions that may be covered by one or more U.S. patents owned by DVSI Inc. The source code itself should not be infringing as it merely describes possible methods of implementation. Compiling or using `mbelib` may infringe on patents rights in your jurisdiction and/or require licensing. It is unknown if DVSI will sell licenses for software that uses `mbelib`.

Possible copyright issues apart the audio quality with the DVSI AMBE chip is much better.

If you are not comfortable with this just do not install DSDcc and/or mbelib and the plugin will not be compiled and added to SDRangel. For packaged distributions just remove from the installation directory:

  - For Linux distributions: `plugins/channel/libdemoddsd.so`
  - For Windows distribution: `dsdcc.dll`, `mbelib.dll`, `plugins\channel\demoddsd.dll`

<h1>Features</h1>

<h2>Changes from SDRangelove</h2>

See the v1.0.1 first official release [release notes](https://github.com/f4exb/sdrangel/releases/tag/v1.0.1)

<h2>To Do</h2>

Since version 3.3.2 the "todos" are in the form of tickets opened in the Issues section with the label "feature". When a specific release is targeted it will appear as a milestone. Thus anyone can open a "feature" issue to request a new feature.

Other ideas:

  - Enhance presets management (Edit, Move, Import/Export from/to human readable format like JSON).
  - Allow arbitrary sample rate for channelizers and demodulators (not multiple of 48 kHz). Prerequisite for polyphase channelizer
  - Implement polyphase channelizer
  - Level calibration
  - Even more demods. Contributors welcome!

<h1>Developer's notes</h1>

Please check the developer's specific [readme](./ReadmeDeveloper.md)
