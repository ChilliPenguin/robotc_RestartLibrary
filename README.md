# robotc_RestartLibrary


# Set up
To use, download the main script file and place within the same folder as your script.
It is required to include the library to use the library(place on the top/before the main task) 

    #include<Library_ResetButton.h>

# Usage
Initialization of a restart button(must occur in main task)

    task main()
	{
		restartButton_Info newBTN;
		restartButton_Create(newBTN, S2, Hold, 1000, true);
	}
To start monitoring the button, use

    restartButton_Start(newBTN);
And to stop monitoring, use
   
    restartButton_Stop(newBTN);
To programmatically call a reset of the main function, use

    restartButton_Restart();
   
## Warnings
##### Do not use functions in seperate tasks(at the same time), will likely cause errors
## Documentation
#### RestartButton_Create
	restartButton_Create(&targetedButn, locationPort, pressType, holdLength, enabled)
	
Used to intilize a new button

`targetButn`: Type restartButton_Info; get instance of button to set characteristics

`locationPort`: Type tSensors; Location of the sensor port of the button to use; currently only supports digital sensors

`pressType`: Type btnPressType; currently only supports Hold, type of method to indicate restart

`holdLength`: Type int; *will be replaced*, used with Hold to determine time of hold until restart occurs(should be of intervals of 250!)

`enabled`: Type Boolean; quick shortcut for Start and Stop function when intilizing button
#### restartButton_Restart
	void restartButton_Restart()
	
Used to restart the main task of a program.
