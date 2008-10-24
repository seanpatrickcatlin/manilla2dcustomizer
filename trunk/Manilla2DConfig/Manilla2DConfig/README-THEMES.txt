M2DC Theme Readme

Theme management support was introduced in M2DC Version 0.6

So far each theme manager has supported themes in a different way the notes below are relevant for 0.14

RECOMMENDED SKINNING PROCESS FOR END USERS
1) If you have not already made a backup of your Manilla 2D Theme files it is highly recommended that you do that
    - Click on the "Restore" tab in M2DC, and click on the button "Backup M2D Theme"
    - If it were up to me I would require this step... but creating a backup takes up ALOT of space on low resource
        devices, so I do not require it.
2) If you have a backup of your original theme, apply that before applying any new custom themes
    - This ensures that you have a "clean" copy of Manilla2D, and greatly reduces the number of problems that
        you may run in to when dealing with skins for Manilla 2D.
3) Make M2DC aware of your theme
    - You have 2 options here
    Option 1: Copy your theme to the M2DC theme directory (usually "\Program Files\M2DC\Themes")
        Every time M2DC starts up it will check this directory for new themes, so this is a quicker
        way to add themes to M2DC.
    Option 2: Add the theme file via the theme manager
        - Start M2DC
        - go to the "Themes" tab
        - click on the "Add" button
        - find the zip or m2dct file that you want to add
        - click on the "OK" button in the lower left hand corner, or upper right hand corner
4) Select the theme that you want to apply in M2DC
    - Start M2DC
    - go to the "Themes" tab
    - click on the name of the theme that you want to apply in the big list of themes
    - click on the "Apply" button
5) Sit back and watch as M2DC skins your M2D installation
    - The process can take awhile, but should be done in 5-10 minutes

M2DC THEME MANAGEMENT NOTES
* M2DC Process
    When a user clicks "Apply Theme" M2DC will do the follow steps
        1) Build a list of all of the files that Manilla 2D is actually using on the device.
            This list is made up by reading the values out of the xml file and then reading the rest of
            the "HH_*" files from the windows directory.
        2) Extract the HTCHomeSettings.xml file from the theme (if the file exists)
        3) Build a list of all of the files that the new theme xml refers to.
        4) Read each file name in the zip file, lookup where the file should be saved to and extract that file
        5) Update the new theme xml file so that all of the images referenced in the file point to the proper place.
     
THEME FILE DETAILS
* Your theme file must either have the extension of .zip of .m2dct.
* All that you need to do is zip up your theme files with an optional (but HIGHLY recommended) HTCHomeSettings.xml file.
* Zip File Contents
    * If your zip file does not contain an HTCHomeSettings.xml file or a file that matching the
        search string "hh_*", then M2DC will not allow the user to use your theme.
    * If your zip file contains an image "preview.jpeg" or "preview.jpg", then this images
        will be used on the theme manager tab to show users a small screenshot of what the theme
        looks like.  M2DC is not very good at image scaling, so for best results ot your users
        the preview file should be 75x100 pixels
* Future
    In the future other files may be supported.
    * One example is a *.tsk file, maybe I can figure out how to programmatically apply the tsk
        theme when the suer applies the M2DC theme.
    * The other example is a welcomehead image
     
NOTES
 * Currently No skins are distributed with M2DC, if you would like to contribute a skin 
    for me to distribute please contact me (Mpenguin14@gmail.com )and we will see what we can work out.

 * I reserve the right to change the file format, but I highly doubt I would ever do that.
