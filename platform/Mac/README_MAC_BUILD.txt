Building BzWorkbench (BZWB) on Mac OS X 

Xcode project format is set to "xcode 2.4-compatible", at present I'm using Xcode 3.1.4

BzWorkbench requires the following dependencies: FLTK, OSG, CURL

The xcode project included is setup to embed FLTK and OSG in the BzWorkbench application. 
As these frameworks are not present on a standard Mac OS X Installation.
 
CURL is standard on MacOS X, so in this case the xcode project is set to use the systems libcurl.
So there is no need to build CURL, unless you want to.


Building Fast Light Tool Kit ( fulltick / fltk ) 

I used the supplied xcode project to build fltk 1.3.x r7216,
(you should be able to use the latest revision)

copy the FLTK Frameworks into bzworkbeach/platform/mac/libs/fltk

Since FLTK does not include their headers within the Frameworks 
like they should. You'll need to make them available to the xcode project.

copy the FLTK FL directory and header files it contains into bzworkbeach/platform/mac/libs/fltk/include

copy the FLTK config.h header file into bzworkbeach/platform/mac/libs/fltk/include


Building Open Scene Graph (OSG)

I used the supplied xcode project to build OSG 2.8.2, 

copy the OSG Frameworks into bzworkbeach/platform/mac/libs/osg

copy OSG Plugins into bzworkbeach/platform/mac/libs/osg/plugins/


Once you have the FLTK and OSG in place open the xcode project and try to build


 


