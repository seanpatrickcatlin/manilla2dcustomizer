#!/usr/bin/python## Manilla 2D Customizer# One step build and release script# from zipfile import ZipFile, ZIP_DEFLATEDimport osimport shutilimport sysimport timedef readFileVersion(fileName):    try:        f = file(fileName)    except IOError:        return ""    st = f.read()    ar = st.split()    major = ar.index("PRODUCT_VERSION_MAJOR")    minor = ar.index("PRODUCT_VERSION_MINOR")    qfe   = ar.index("PRODUCT_VERSION_QFE")    version=ar[major+1] + "." + ar[minor+1] + "." + ar[qfe+1]    return version    def get_last_svn_rev():    lastRev=-1    cmd = "\"" + SVN_COMMAND + " info " + SVN_REPOSITORY + "\""    for line in os.popen(cmd, 'r').readlines():        pos = line.find("Revision: ")        if pos != -1:            lastRev = int(line.split()[1])    return lastRev    def incMinor(versionString):    verList=versionString.split(".")    verList[1]=str(int(verList[1])+1)    return verList[0]+"."+verList[1]+"."+verList[2]def setVerSvnRev(versionString, revision):    verList=versionString.split(".")    verList[2]=str(revision)    return verList[0]+"."+verList[1]+"."+verList[2]    def setFileVersion(fileName, version):    try:        f = open(fileName, "r")    except IOError:        return    fileArray = f.readlines()    f.close()        ext = fileName[fileName.rfind('.')+1:]        if(ext == "h"):        verArray = version.split('.')                count = 0                for line in fileArray:            if(line.find("#define PRODUCT_VERSION_MAJOR ") != -1):                fileArray[count] = line[0: line.rfind(' ')+1] + verArray[0] + "\n"            elif(line.find("#define PRODUCT_VERSION_MINOR ") != -1):                fileArray[count] = line[0: line.rfind(' ')+1] + verArray[1] + "\n"            elif(line.find("#define PRODUCT_VERSION_QFE ") != -1):                fileArray[count] = line[0: line.rfind(' ')+1] + verArray[2] + "\n"                        count = count+1                elif(ext == "xml"):        count = 0        for line in fileArray:            if(line.find("<version>") != -1):                fileArray[count] = "  <version>" + version + "</version>\n"                        count += 1            try:        f = open(fileName, "w")    except IOError:        return    f.writelines(fileArray)    f.close()            return    def setNewCabUrlInApptodateXML(apptodateXmlFileName, newURL):    try:        f = open(apptodateXmlFileName, "r")    except IOError:        return    fileArray = f.readlines()    f.close()        count = 0    for line in fileArray:        if(line.find("<caburl>") != -1):            fileArray[count] = "  <caburl>" + newURL + "</caburl>\n"                    count += 1            try:        f = open(apptodateXmlFileName, "w")    except IOError:        return    f.writelines(fileArray)    f.close()        return # this zipfile sample adapted from http://www.penzilla.net/tutorials/python/scripting/def zipRecursively(path, archive):    paths = os.listdir(path)    for p in paths:        p = os.path.join(path, p) # Make the path relative        if os.path.isdir(p): # Recursive case            zipRecursively(p, archive)        else:            archive.write(p) # Write the file to the zipfile    returndef createZipFile(path, archname):    # Create a ZipFile Object primed to write    archive = ZipFile(archname, "w", ZIP_DEFLATED) # "a" to append, "r" to read    # Recurse or not, depending on what path is    if os.path.isdir(path):        zipRecursively(path, archive)    else:        archive.write(path)    archive.close()    return ############################# ## the actual implementation code##############################ORIGINAL_VERSION_H_FILE = "..\\Manilla2DConfig\\Manilla2DConfig\\version.h"VERSION_FILES_TO_UPDATE = []VERSION_FILES_TO_UPDATE.append("..\\Manilla2DConfig\\Manilla2DConfig\\version.h")VERSION_FILES_TO_UPDATE.append("..\\Manilla2DConfig\\Manilla2DConfig\\M2DC.xml")VERSION_FILES_TO_UPDATE.append("..\\Manilla2DConfig\\Manilla2DConfigSetup\\version.h")SVN_COMMAND = "\"C:\\\\Program Files\\\\CollabNet Subversion\\\\svn.exe\""SVN_REPOSITORY = "https://manilla2dcustomizer.googlecode.com/svn"DEVENV_COMMAND = "\"C:\\\\Program Files\\\\Microsoft Visual Studio 9.0\\\\Common7\\\\IDE\\\\devenv\""## (Step 1) # Update svn repositoryprint "\nUpdate SVN Repository"cmd = "\"" + SVN_COMMAND + " update ..\\\""os.system(cmd)## (Step 2)# Get the original file verisonprint "\nREAD ORIGINAL VERSION STRING"verStr = readFileVersion(ORIGINAL_VERSION_H_FILE)print "Original File: " + ORIGINAL_VERSION_H_FILEprint "File Version: " + verStr## (Step 3)# Get the latest revision number of the svn repositoryprint "\nGET LATEST SVN REV NUMBER"last_svn_rev = get_last_svn_rev()print "Current SVN Revision: " + str(last_svn_rev)## (Step 4)# Increment the minor build number, set the svn revision numberprint "\nINCREMENR BUILD NUMBER AND REV NUMBER"new_svn_rev = last_svn_rev+1print "New SVN Revision: " + str(new_svn_rev)verStr = setVerSvnRev(incMinor(verStr), new_svn_rev)print "New Version String: " + verStr## (Step 5)# update the apptodate xml file with the location to the new cab fileprint "\nUPDATE APPTODATE XML FILE"newBaseFileName = "M2DC_v" + verStrnewCabFileName = newBaseFileName + ".CAB"newCabUrlStr = "http://manilla2dcustomizer.googlecode.com/files/" + newCabFileNameprint "New File Name: " + newCabFileNamesetNewCabUrlInApptodateXML("..\\Manilla2DConfig\\Manilla2DConfig\\M2DC.xml", newCabUrlStr)## (Step 6)# Write the new filesprint "\nWRITE NEW VERSION FILES"for fileToUpdate in VERSION_FILES_TO_UPDATE:    setFileVersion(fileToUpdate, verStr)    count=1    ## (Step 7)# Output the version string and open notepad for editing changelog.txtprint "\nDISPLAY DATE AND VERSION, OPEN NOTEPAD FOR CHANGLEOG"print time.ctime()print "NewVersion: " + verStros.system("notepad.exe changelog.txt")# (Step 8)# commit the modified files (M2DC.xml, version.h, changelog.txt)# print "\nCOMMIT CHANGES TO SVN"cmd = "\"" + SVN_COMMAND + " commit -m \"updated build number to " + verStr + "\" ..\\\""os.system(cmd)# (Step 9)# build the solution to get the cab fileprint "\nBUILDING CAB FILE FROM SOLUTION"cmd = "\"" + DEVENV_COMMAND + " ..\\Manilla2DConfig\\Manilla2DConfig.sln /rebuild release /project Manilla2DConfigCab\""for line in os.popen(cmd, 'r').readlines():    print line    # (Step 10)# copy the cab file to the CWD# rename the cab file "M2DC_v" + verStr + ".CAB"print "\nCOPY CAB FILE TO CWD"originalCabFile = "..\\Manilla2DConfig\\Manilla2DConfigCab\\Release\\M2DC.CAB"shutil.copy(originalCabFile, newCabFileName)# (Step 11)# do an svn export on the sorce tree, export it to a folder local to here named Manilla2DConfig_v + verStr + "-src"print "\nSVN EXPORT"newSrcDir = newBaseFileName + "-src"cmd = "\"" + SVN_COMMAND + " export " +  SVN_REPOSITORY + "/trunk " + newSrcDir + "\""os.system(cmd)# (Step 12)# zip up the source file directory, delete the directoryprint "\nZIP SVN EXPORT FOLDER"newSrcZip = newSrcDir + ".zip"createZipFile(newSrcDir, newSrcZip)# (Step 13)# upload the resulting cab and source zip file to google codeprint "\nUPLOAD CAB FILE AND SOURCE ZIP"# pause here so that the user can read the outputtempInput=raw_input('Press enter to continue')############# END OF SCRIPT #############