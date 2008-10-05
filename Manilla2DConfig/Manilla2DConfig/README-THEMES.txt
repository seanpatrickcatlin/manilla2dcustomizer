M2DC Theme Readme

Theme management support was introduced in M2DC Version 0.6

So far each theme manager has supported themes in a different way the notes below are relavant for 0.9

M2DC THEME MANAGEMENT NOTES
* M2DC Process
    When a user clicks "Apply Theme" M2DC will first build a list of all of the files that M2CD is using
        on the device.  This list is made up by reading the values out of the xml file and then reading the
        rest of the "HH_*" files from the windows direcotry.
        After the list is built M2DC starts to go though the zip file.  It looks at each file in the zip sep

* HTCHomeSettings.xml
    If this file exists in your theme then M2DC will read values from the main <WidgetProperty> node.
        It will only read values that have the string "POS" or "RECT" in their name attribute.  I only
        read these values because I don't know of a situation where a theme should change other values
        (maybe width/height values???).  Please let me know if you change values other than POS and RECT
        in your theme.
     
* Hiding Clocks
    A common feature of a skin is to hide the digital or analog clock fomr the home widget.  I have seen
        more than a handful of different ways to accomplish this and no one seems ot agree on a standard.
        I am putting in writing my opinion and the way that M2DC hides clocks.  When I want to hide something
        I just set the POS and RECT values to "-1,-1" and "-1,-1,-1,-1" respectively.  This hides the item
        and has no other adverse side effects on the users htchomesettings.xml file.
     
THEME FILE DETAILS
* Your theme file must either have the extension of .zip of .m2dct.
* All that you need to do is zip up your theme files with an optional HTCHomeSettings.xml file.
* Zip File Contents
    * If your zip file does not conatin an HTCHomeSettings.xml file or a file that matching the
        search string "hh_*", then M2DC will not allow the user to use your theme.
    * If you do not include an HTCHomeSettings.xml file then the users backup HTCHomeSettings.xml
        file will be used for image positions and other theme read values.
* Future
    In the future other files may be supported.
    * One example is a *.tsk file, maybe I can figure out how to programmatically apply the tsk
        theme when the suer applies the M2DC theme.
    * Another example would be a screenshot.jpg or soemthing liek that.  I t would allow for users
        to get a general dea of what the theme looks like from within the theme manager.
     
NOTES
 * Currently No skins are distributed with M2DC, if you would like to contribute a skin 
    for me to distribute please contact me and we will see what we can work out.

 * I reserve the right to change the file format, but I highly doubt I would ever do that.
