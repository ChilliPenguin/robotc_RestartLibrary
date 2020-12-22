# robotc_RestartLibrary

# Set up
To use, download the main script file and place within the same folder as your script.
It is required to include the library to use the library(place on the top/before the main task) 

    #include<Library_ResetButton.h>

# Usage
Initialization of a restart button(must occur in main task)

    task main()
	{
		btn_Info newBTN;
		restartButton_Create(newBTN, S2, Hold, 1000, true);
	}
To start monitoring the button, use

    restartButton_Start(newBTN);
And to stop monitoring, use
   
    restartButton_Stop(newBTN);
To programmatically call a reset of the main function, use

    restartButton_Restart();
   
