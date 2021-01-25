# robotc_RestartLibrary


# Set up
To use, download the main script file and place within the same folder as your script.
It is required to include the library to use the library(place on the top/before the main task) 

    #include<Library_ResetButton.h>

# Usage
Will be rewritten due to difference in new code
To programmatically call a reset of the main function, use

    restartButton_Restart();
   
## Warnings
##### Do not use functions in seperate tasks(at the same time), will likely cause errors
## Documentation
Will be rewritten due to difference in new code
#### restartButton_Restart
	void restartButton_Restart()
	
Used to restart the main task of a program.
