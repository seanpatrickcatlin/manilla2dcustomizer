## I need a one step build and release script# #  svn checkout https://manilla2dcustomizer.googlecode.com/svn/trunk/ manilla2dcustomizer --username catlin.sean## #### (Step 1) # Update svn repository# svn update# (Step 2)# Get the latest revision number of the svn repositro# svn info;# parse the outpu for revision number# (Step 3)# Set the new Version number in M2DC.xml and version.h# right now the scheme is this "A.B.C"# A is the major build number, this will only be changed by hand when a significant verison comes out (verison 1.0 wiull be the official non beta release, and it iwll include theming support)# B is the Minor build number, it will incremented by 1 every time this script is ran# C is the svn revision number... I need to take the integer from setp 2 and increment it by 1# (Step 4)# display the ful lversion number, launch notepad for updating the changelog.txt file# # (Step 5)# commit the modified files M2DC.xml, verison.h, changelog.txt# # (Step 6)# build the solution to get the cab file# # (Step 8)# rename the cab file M2DC_vA.B.C.CAB# (Step 8)# 