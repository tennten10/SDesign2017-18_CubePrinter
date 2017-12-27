# SDesign2017-18_CubePrinter

#Code functions we will need to create:
1. Turn on/standby/off
2. LED control - on during layer printing, off during standby and off, off during print bed adjustments
3. Print bed motor control - interfacing with stepper/servo motor drivers and controlling position. Must keep coordinated with layer of the print.
4. LCD masking - Create inverse image of the object we are creating, and store X number of layers it will take to build. Maybe create a 2 dimensional array system to keep track of commands. 
5. Locked position (and mechanism?) for during launch and when the print is finished. 

(optional)6. If necessary due to pinout or memory, separate arduinos can be used for different systems and the clocks can be synced.



One method for creating our images for the screen could be as follows...
  - https://formlabs.com/blog/open-source-dlp-slicer/
  - convert .png to .c or .raw using http://www.rinkydinkelectronics.com/t_imageconverter565.php to make it compatible with UTFT library
  - upload to SD card and index through it in the program
  
  - alternatively, use slic3r to create svg image, convert to png and follow same procedure.

I tested methods and one simple way to do this is to use the formlabs link: - https://formlabs.com/blog/open-source-dlp-slicer/
It takes an STL file as input and slices it into layers depending on the inport parameters. Make sure that the layer height in the slicer matches with the layer height in our Config_Parameters file. It could potentially be adjusted depending on the print. 
It will download a zip file with hundreds of .png images with the file names such as out0000.png ... out0349.png. Unzip that folder to any location. 
If you downloaded the UTFT adruino library, find the Tools folder within that. There is a command line program capable of converting batches of images. It has documentation in the pdf. 
Copy the ImgConv.exe program into the unzipped folder. 
Open CMD.exe from your windows desktop. Navigate to that folder. Copying the extension from an open windows explorer window makes this easy. the syntax to navigate to that location is something like: > cd C:\Users\Mike\Downloads\foldername
Enter: ImgConv.exe to make sure it responds with help directions. We will be converting to .raw files. I haven't gotten the output directory to work, but we could push the converted files to another directory theoretically. 
Enter: ImgConv out????.png /r 
  - this will convert all the png files with names beginning with out and 4 wildcard characters to .raw format and store in the same folder. *.png theoretically works, but hasn't when I've tried. /o C:/... to output to another folder. It will take a while to convert all the files but is much better than doing them individually.  
  After it finishes converting all the files, open the folder, sort by file type, and copy into a separate folder. Put this folder onto the SD card to be used in the printer. 
After this, change the text names in the 2nd arduino code to match up with the names on the SD card. Update code. 
