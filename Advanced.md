# D-Air Race Lap Times - Advanced Usage
This documentation is for those who are comfortable with C++.

## Prerequisites
This project was developed using Visual Studio Code, using the Microsoft C/C++ and C/C++ extension pack extensions, CMake, CMake Tools and C++ TestMate extensions, Google Test and matPlotLibcpp. MatPlotLib was chosen because there is a good C++ interface and because I had some experience with it from working on some Python projects.

The downside to matPlotLibcpp is that it requires Python to be installed in order to run, and something appears to require that the system that runs the program have the exact same version (major and minor - i.e. 3.10) as the machine where the program was compiled. That is, if I compile with Python 3.10.2, it will probably run with 3.10.1, but definitely won't run on Python 3.9. Any advice on removing this restriction would be greatly appreciated!

## Available Functionality
This program can:
- Calculate and display lap times - text and graph.
- Display the contents of any of the D-Air downloaded files.
- Graph the contents of any one of these files against latitude/longitude points, as a color.
- Graph the lap time trigger points on top of the latitude and longitude points.
This functionality can only be accessed by commenting out and uncommenting lines in the main method of the daineseMain source file and then recompiling.

The standard Dainese-supplied software downloads the raw data from the D-Air suit, saves it, uses it to calculate and save a number of additional data channels and can display charts containing most of this data. However, I have not been able to get it to display lap times. 

Since I wanted to get some practice with C++, I decided to see if I could figure out how the downloaded data is stored and whether I could use it to calculate my own lap times. The short answer is yes, and they indicate how much room I have for improvement in my track riding. (The motorcycle lap record for the Shannonville long track is 1:43, my best time is 2:20!)

The data is stored in binary files with a separate file for each channel. A channel could be the output from a specific sensor in the suit or it could be calculated from one or more of those channels. The raw data is stored in the files whose extensions start with an A while the calculated data is in files that start with C. There are several other file types in the download directory, but they appear to be mostly logs or descriptions of the contents of the other files.

The standard software that Dainese supplies contains quite a few programs. One of them, WFormel (which can be launched by double-clicking on a "*.DDD" file), shows the various channels, with multiple tabs showing contents, units of measure, adjustment factors, limits, sampling frequency and data size. I found the "Channel-Type" to be a pretty accurate reflection of the C++ data type to use when reading the data in binary mode. 

### Displaying File Contents
The displayFileContents function will read in the contents of a given file, convert them to a slightly more easily understood format and output the results - the first few values, along with the number of values read and the maximum and minimum values in the file. The vectorMaxElementsToDisplay setting in config.txt defines how many elements to output. 

To select the file to view, uncomment the line in daineseMain.cpp/main for the desired file and recompile.

### Graphing a Channel Over Track Layout
The graphFileAgainstLatLong function displays the selected channel as a color range over top of the track layout. It can display things like where your maximum velocity or acceleration (positive and negative) occur.

Uncomment the appropriate line in main, or, if the channel you want isn't listed, copy an existing line and change it to the DaineseDataTypes type that you want and copy the template data types from the displayFileContents line for the same type above.

### Speed vs Track Layout at Mosport
![Velocity](/images/Velocity.jpg)

### Calculating Lap Times
The lapTimes function will calculate laptimes from the given data and config, printing them to the command prompt and also graphing them.

It works by using the target trackEntrance, startFinish and trackExit1/trackExit2 points to trigger when you enter the track, cross startFinish or exit the track. It starts the first lap when you cross the trackEntrance, starts another lap every time you cross startFinish and ends the last lap when you cross either trackExit. Thus, the first and last laps of every session are your out lap and in lap times, which aren't comparable to a regular lap, but can be useful. 

This approach is susceptible to problems, mainly that if you pass by one of the trigger points sufficiently far from the precise target GPS coordinates, for example if you are on one edge of the track while the target is on the other, the point might not trigger, depending on how big the latLongWindow config value is. However, the opposite problem is triggering a point inappropriately because the latLongWindow is too big - i.e. triggering trackEntrance when just going by on the track (since the entrance is generally right beside the track.) 

A careful balance in picking points is required. For example, the earlier you pick the point in the track entrance lane the further you likely are from the actual track, but it could also be wider, or it may be before the point where you are allowed to enter the track - so you may be sitting there for some time before a session, and thus the out lap time may include that time. Similarly, track exits can get wider the further you get from the track.

It is possible that some tracks may have alternate exits that could be used in the event of something like a red flag. I have provided support for two exit points in the config, but a track may have more. If a track only has one, you can leave the GPS coordinates for the second as 0.

## Adding a New Track
Technically, all you have to do is copy one of the existing track sections in config.txt, comment out the original section and put the right numbers in your new section. The difficulty lies in selecting GPS coordinates for the trigger points that will reliably be hit when you want them to be but not be accidentally hit when you don't want them to be. 

The first step is to pick your longitude filter values. This is important because if you have data from two different places in the download, say from two tracks, or because you turned on the suit at home, the latitude/longitude plots will end up being two points - the difference between the two groups will be so much bigger than the differences within each group that you will see no detail within a group. You can use Google maps to get the approximate GPS coordinates for the track you are working on - the longFilterLow and longFilterHigh don't have to be very precise, just narrow enough to filter out everything that isn't at the track. 

Then, use the graphTrackPOIs function to display the track layout. Pick your trackEntrance, startFinish and trackExit points, using the fact that matPlotLib will display the exact coordinates of the mouse pointer in the bottom right corner of the window. Update the config file with these initial values then rerun the program. You should see some faint colored lines across the otherwise black lap traces - these are the points that the program has identified as being at the various trigger points.

You may have to use matPlotLib's zoom button to be able to see the individual points and whether you are missing laps that you shouldn't, or picking up all the points that you should. Adjust the target points as necessary, or the window size, if it is possible to do so without getting false positives. 

Once you are happy with the results, switch to running the lapTimes function and confirm that you aren't missing any times - usually indicated by laps that are exceptionally long. 

Note that picking a startFinish point that exactly matches the official start/finish line of the track isn't necessary, if you aren't trying to compare laptimes from this program to officially recorded times. (If you have official times, why are you using this program?) Rather, pick a point somewhere on the startFinish straight that is far enough from the track exit and entrance that you can expand the window without getting false positives for any of the trigger points.

The displayTimeLatLong function will output the time and latitude/longitude points for every measurement in the given files, if you need more help figuring out why a certain point wasn't triggered on a given lap - but it is a lot of data to go through. (Use the start time of the problematic lap to estimate where to start your search.)
