int reset_Period = 1250;
enum btnPressType{Hold};

//precalls functions and tasks
task main();
void restartButton_Restart();


struct btn_Info{
	tSensors     locationPort;
	btnPressType pressType;
	int 			   holdLength;
	bool 				 enabled;
};

void newRestartBtn(			btn_Info    &targetedButn,
												tSensors     locationPort,
										  	btnPressType pressType,
											  int 			   holdLength,
										  	bool 				 enabled){
	targetedButn.locationPort = locationPort;
	targetedButn.pressType    = pressType;
	targetedButn.holdLength   = holdLength;
	targetedButn.enabled      = enabled;
};

btn_Info monitoredInput;
btn_Info settedInput;


task monitorInput(){
	settedInput = monitoredInput;
	bool beforePressed = false;
	bool pressed       = false;
	int  timeToSleep   = 250;
	int  pressedFor    = -timeToSleep;
	while(true){
		pressed = SensorValue(settedInput.locationPort);
		pressedFor += (pressed)?timeToSleep:-pressedFor-timeToSleep;
		switch(settedInput.pressType){
			case Hold:
				if(pressedFor >= settedInput.holdLength){
					restartButton_Restart();
					stopTask(monitorInput);
				}
		}
		beforePressed = pressed;
		sleep(timeToSleep);

	}
}


void restartButton_Start(btn_Info type){
	if(type.enabled) return;
	type.enabled = true;
	startTask(monitorInput);
	return;
}

//stops monitoring button by ending monitor task
void restartButton_Stop(btn_Info type){
	if(!type.enabled) return;
	type.enabled = false;
	stopTask(monitorInput);
	return;
}

//restarts main program via another thread stoping and starting main thread
task restartProgramTaskWay(){
  stopAllMotors();
	stopTask(main);
	sleep(3000);		//reaches this code as this is performed on another thread
	startTask(main);

}

//function to call thread; makes sure program ends on the line of the command
void restartButton_Restart() {
	startTask(restartProgramTaskWay);
	sleep(10);	//enough time for restart to occur

}
