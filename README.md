

# OSM Visualization

License:  **GPLv2(GNU  General  Public  License,  version  2)**

Visualization of OSM Data on base of a directed Graph. Calculate the road dimensions using given width OSM data or estimated lane width and number of the lanes. Option to move the base line from middle of the road to the left side to get a better dimensions view.

Create WMS tiles for a rectangle area and a script ((C)Leaflet) to start the tile tree from the local file system.

### How to use

Get an OSM XML-File and load it, for example from 'josm' or 'merkaartor

*On  left drive country’s like UK:*

*Select the ‘left drive’ (Directory dialogue) option before the loading of the OSM file.*

Select the base layers and single option to show the map. There are predefined layer classes:

#### Net view: road, rail, water and power

This layers build a directed Graph each. The road layer is unique and build the road geometry and follows the OSM 'highway' definition.

#### Area view: landuse, field, building

The 'landuse' layer builds the base layer for the drawing and is close to the OSM landuse definition. Next is the 'field' layer witch is on top of the 'landuse' layer. 'Field' is not an OSM definition and may include OSM tags like ‘nature’, ‘sports’. etc. On top of the 'field' layer is the 'building' layer and follows the OSM tag plus 'manmade'.

#### POI view

This layer includes any node with name or a node without name but not member of a way

### WMS

Export the WMS layer if needed.

### Dialogs

#### Profile File

The Profile File is located in the base directory and is named 'tr\_default\_profile.xml'. With this file is it possible to edit the colours of classes of elements (roads, houses, etc.).

* select the menu point 'Settings-\>Profile'
* inflate tree view item
* double click the colour fields and change the colors
* test the colours or/and save the settings

#### Directory dialogue

* Select the menu point 'Settings-\>'Directories'
* Check the path of the file 'tr\_default\_profile.xml'
* Select a path for the OSM files (default '.')
* Select the shift mode
* Change the lane width in \[mm\]

#### View options

 Nodes: show the nodes of the directed Graph

* Cross points
* POI: subset of "point of interest"
* Street names
* Parking: show the roadside parking

#### Tile dialog

* Define the rectangle by dragging the area by right mouse button
* Select the menu point 'Tools-\>'Tiles'
* Select a path for the Tile file tree
* Define WMS Level (Z)

## Tested  Systems

I have complied the source code and test on the following systems:

- Windows 11

- Linux: SuSE, Fedora

#### Qt Version

5.12 - 6.11.1

#### Clone the source code

```
$ git  clone  https://github.com/hubertqtraf/QtOSMTraffic.git
```

#### Build (cmd, Linux)

```
$ cd QtOSMTraffic
$ qmake
$ make
```

if 'qmake' is not found, select the Qt Version (Qt6 or Qt5):

```
$ /usr/lib64/qt6/bin/qmake
```

or

```
$ /usr/lib64/qt5/bin/qmake
```

### Quick Run on Windows, Mac, and Linux  with  Qt  Creator

- Open the project with Qt Creator.

- Configure the Windows project with either MinGW or Visual Compiler

- Run

