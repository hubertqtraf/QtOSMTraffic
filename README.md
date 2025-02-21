
#  OSM Visualisation

License:  **GPLv2(GNU  General  Public  License,  version  2)**

Visualisation of OSM Data on base of a directed Graph. 
Calculate the road dimensions using given width OSM data or estimated lane width and number of the lanes.
Option to move the base line from middle of the road to the left side to get a better dimensions view.

###  How to use: 
Get an OSM XML-File and load it, for example from 'josm' or 'merkaartor'

#### Profile File
The Profile File is located in the base directory and is named 'tr_default_profile.xml'.
With this file is it possible to edit the colours of classes of elements (roads, houses, etc.).

* select the menu point 'Settings->Profile'
* inflate tree view item
* double click the colour fields and change the colors
* test the colours or/and save the settings

#### Directory dialogue

* select the menu point 'Settings->'Directories'
* check the path of the file 'tr_default_profile.xml'
* select a path for the OSM files (default '.')
* select the shift mode
* change the lane width in [mm]

####  View options
* Nodes: show the nodes of the directed Graph
* Cross points
* POI: subset of "point of interest"
* Street names
* Parking: show the roadside parking

##  Tested  Systems
I have complied the source code and test on the following systems:

* Windows  11
* Linux: SuSE, Fedora

####  Qt Version
5.12 - 6.8.2

####  Clone the source code
```sh
$ git  clone  https://github.com/hubertqtraf/QtOSMTraffic.git
```

####  Build (cmd, Linux)

```sh  
$ cd QtOSMTraffic
$ qmake
$ make
```
if 'qmake' is not found, select the Qt Version (Qt6 or Qt5):

```sh
$ /usr/lib64/qt6/bin/qmake
```
or
```sh
$ /usr/lib64/qt5/bin/qmake
```

###  Quick Run on Windows, Mac, and Linux  with  Qt  Creator
 - Open the project with Qt Creator.
 - Configure the Windows project with either MinGW or Visual Compiler 
 - Run

