# Cross-Correlation Reichardt Detector Concept Implemented with Frame-based Output

![reichardt1.png](https://bitbucket.org/repo/6a96ng/images/3603282252-reichardt1.png)

Uses frame-by-frame input, from any webcam, to produce a visualization of a cross-correlation Reichardt Detector.


# **Cross-Correlation Explained**

![diagram](https://bitbucket.org/repo/6a96ng/images/2403735046-reichardt_detector_model.jpg)

The advantages of this model are its simplicity in implementation, and low computational demand.  Consider pixel intensities A and B.  Two consecutive frames are collected from the input source, from which we determine A1, B1, A2, B2 (where 1 is the earlier of the two frames).  The time elapsed between the two frames will serve as our 'delay'.  As seen in the diagram, we take the sum of positive product of A2 and B1, and the negative product of A1 and B2.  If the sum results in a large absolute value (beyond a chosen threshold), it indicates a movement, the direction of which is determined by the sign.

In this implementation, we take positive (+) to be *forward* motion (i.e. right or down), and negative (-) to be *reverse* motion (i.e. left or up).  

FORWARD = GREEN 

REVERSE = RED

## Requirements

- [OpenCV 3.1.0][1]

[1]: http://opencv.org/

## Installation

Use the following command to add opencv to pkg-config path (requires pkgconfig to be installed):

$ export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:</path/to/opencv.pc>

## Usage

Compile with the provided makefile and run the executable with a webcam attached. 

## Credits

Cody Barnson

## License

    Cross-Correlation Reichardt Detector Concept Implemented with Frame-based Output
    Copyright (C) 2016  Cody Barnson

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.