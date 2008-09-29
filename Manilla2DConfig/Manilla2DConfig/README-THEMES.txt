M2DC Theme Readme

Theme management support was introduced in M2DC Version 0.6

M2DC accepts theme files that are named *.m2dct

This file is a zip file that has been renamed from *.zip to *.m2dct

All a theme file is is a zip file with the a HTCHomeSettings.xml file, and all of the HH_ files
than Manilla 2D uses to display graphics.

When a user asks for a specific theme to be applied the customizer will
1.) Open the m2dct file.
2.) Extract all the files in it to %InstalDir%\ActiveTheme
     (ignoring the directory structure of the zip file)
3.) Read specific values from the HTCHomeSettings.xml file and write them to the actual 
     HTCHomeSettings.xml file locate din \Windows.
     - Currently the customize will read the following values
	<HTCHome><ImageList> elements with ids 121-128, and 209-216
	<HTCHome><WidgetProperty><HomeWidget> elements  with indexes 0-10

CUrrently No skins are distributed with M2DC, if you would like to contribute a skin 
for me to distribute please contact me and we will see what we can work out.

This readme may get more detailed to answer FAQs.

I reserve the right to change the file format, but I highly doubt I would ever do that.
