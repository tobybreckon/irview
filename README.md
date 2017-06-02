# Infrared (Thermal) Video Viewer - False Colour Mapping

Requires [OpenCV 3.x](http://www.opencv.org).

---

```
IrView - (c) Toby Breckon, 2008-2017+
         with OpenCV version 3.1.0 (3.1.0)
GPL - http://www.gnu.org/licenses/gpl.html

Usage :./irview [image/video file]
Camera interface: run with no file agrument for direct camera use

Keyboard commands
         a - automatic scaling (default: on)
         b - show both false colour and original (default: off)
         c - toggle false colour (default: on)
         x - exit
```
---

_"In certain instances, it can be better to display intensity images using a false-colour map.
One of the main motives behind the use of false-colour display rests on the fact that the
human visual system is only sensitive to approximately 40 shades of grey in the range from
black to white, whereas our sensitivity to colour is much finer. False colour can also serve to
accentuate or delineate certain features or structures, making them easier to identify for the
human observer."_
[[Solomon / Breckon, 2010](http://www.fundipbook.com)]

Originally developed as a visualization and test tool to support the research in:

[Multi-Modal Target Detection for Autonomous Wide Area Search and Surveillance](http://community.dur.ac.uk/toby.breckon/publications/papers/breckon13autonomous.pdf)
(T.P. Breckon, A. Gaszczak, J. Han, M.L. Eichner, S.E. Barnes), In Proc. SPIE Emerging Technologies in Security and Defence: Unmanned Sensor Systems, SPIE, Volume 8899, No. 01, pp. 1-19, 2013. [[pdf](http://community.dur.ac.uk/toby.breckon/publications/papers/breckon13autonomous.pdf)] [[doi](http://dx.doi.org/10.1117/12.2028340)]

Work carried out as part of the _UK MoD Grand Challenge 2008_ (autonomous systems for wide area search and surveillance).

Used for the false colour visualization examples in:

 [Dictionary of Computer Vision and Image Processing](http://dx.doi.org/10.1002/9781119286462) (R.B. Fisher, T.P. Breckon, K. Dawson-Howe, A. Fitzgibbon, C. Robertson, E. Trucco, C.K.I. Williams), Wiley, 2014.
 [[Google Books](http://books.google.co.uk/books?id=TaEQAgAAQBAJ&lpg=PP1&dq=isbn%3A1118706811&pg=PP1v=onepage&q&f=false)] [[doi](http://dx.doi.org/10.1002/9781119286462)]

---

Click for youtube video example (of thermal ducks):

[![Example Video](http://img.youtube.com/vi/c9nF-k0u0Qk/0.jpg)](http://www.youtube.com/watch?v=c9nF-k0u0Qk)

---

If you find any bugs report them to me (or better still submit a pull request, please) - toby.breckon@durham.ac.uk

_"may the source be with you"_ - anon.

### (still) TODO
- variable colour maps
- video file looping
- additional auto contrast techniques
