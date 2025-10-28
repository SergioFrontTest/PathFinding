UI Architecture :

UI
--> contains MainFrameWnd --> contains a IUIDraw interface which is implemented by UIDirect2D --> contains Image
--> contains Terrain --> contains TerrainWalk

To change the display frames rate, just modify the variable :
    constexpr int FramesPerSeconds = 12; // Game FPS

MainFrameWnd is the mainframe window, the internal content is drawn using the IUIDraw interface, allowing future change of display driver.
For this example, I did an implementation for Direct2D in UIDirect2D.
