#  OSM Visualisation

License:  **GPLv2(GNU  General  Public  License,  version  2)**

Visualisation of OSM Data on base of a directed Graph. 
Calculate the road dimensions using given width OSM data or estimated lane width and number of the lanes.
Option to move the base line from middle of the road to the left side to get a better dimensions view.

###  How to use: 
Get an OSM XML-File and load it, for example from 'josm' or 'merkaartor'

###  View options
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
5.12 - 6.6

####  Clone the source code
```sh
$ git  clone  https://github.com/hubertqtraf/QtOSMTraffic.git
```

####  Build

```sh  
$ cd QtOSMTraffic
$ qmake
$ make
```

###  Quick Run on Windows, Mac, and Linux  with  Qt  Creator
 - Open the project with Qt Creator.
 - Configure the Windows project with either MinGW or Visual Compiler 
 - Run

