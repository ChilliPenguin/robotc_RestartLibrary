int reset_Period = 1250;
enum btnPressType{Hold};

//precalls functions and tasks
task main();
void restartButton_Restart();

//data structure of the button
struct restartButton_Info{
	tSensors     locationPort;
	btnPressType pressType;
	int 			   holdLength;
	bool 				 enabled;
	//This value should not be modifided by the main program!
	unsigned int monitorInformation; //aaaaabc, a-timeheld, b-pressedBefore, c-pressedNow
};

//------------------------------------------------------------------------------
//Constructor of struct
//------------------------------------------------------------------------------

void restartButton_Create(restartButton_Info &targetedButn, tSensors locationPort,btnPressType pressType,int holdLength,bool enabled){
	targetedButn.locationPort = locationPort;
	targetedButn.pressType    = pressType;
	targetedButn.holdLength   = holdLength;
	targetedButn.enabled      = enabled;
}

restartButton_Info monitoredInput;
restartButton_Info settedInput;

restartButton_Info restartButton_MonitoredButtons[1];

//------------------------------------------------------------------------------
//Array Managers
//------------------------------------------------------------------------------

restartButton_Info test; //change name!
int restartButton_MonitoredButtons_GetIndexOfElement(restartButton_Info rB_I){
	for(int i = sizeof(restartButton_MonitoredButtons);i>0;i--){
		test = restartButton_MonitoredButtons[i];
		if(rB_I == restartButton_MonitoredButtons[i]){
			return i;
		}
	}
	return -1;
}

void restartButton_MonitoredButtons_AddElement(restartButton_Info resButton_Used){
		restartButton_Info restartButton_MonitoredButtons_TemperaryArray[sizeof(restartButton_MonitoredButtons)+1];
		for(int i = 0;i<sizeof(restartButton_MonitoredButtons);i++){
			restartButton_MonitoredButtons_TemperaryArray[i] = restartButton_MonitoredButtons[i];
		}
		restartButton_MonitoredButtons_TemperaryArray[sizeof(restartButton_MonitoredButtons)] = resButton_Used;
		restartButton_MonitoredButtons = restartButton_MonitoredButtons_TemperaryArray;
}

void restartButton_MonitoredButtons_RemoveElement(restartButton_Info resButton_Used){
	restartButton_Info restartButton_MonitoredButtons_TemperaryArray[sizeof(restartButton_MonitoredButtons)-1];
	int offsetArray = 0;
	for(int i = 0;i<sizeof(restartButton_MonitoredButtons_TemperaryArray);i++){
		if(restartButton_MonitoredButtons[i] == resButton_Used) offsetArray++;
		restartButton_MonitoredButtons_TemperaryArray[i] = restartButton_MonitoredButtons[i+offsetArray];
	}
		restartButton_MonitoredButtons = restartButton_MonitoredButtons_TemperaryArray;
}

//------------------------------------------------------------------------------
//MonitorInputs
//------------------------------------------------------------------------------

restartButton_Info refferedBtn; //preset
task restartButton_monitorInput(){
	int  timeToSleep   = 250;
	while(true){
		for(int a = 0;a<sizeof(restartButton_MonitoredButtons);a++){
			refferedBtn = &restartButton_MonitoredButtons[a];
			refferedBtn.monitorInformation |= SensorValue(refferedBtn.locationPort);
			if(refferedBtn.monitorInformation & 1){
				refferedBtn.monitorInformation += timeToSleep*0b100;
			}else {
				refferedBtn.monitorInformation ^= (((refferedBtn.monitorInformation>>2))<<2);
			}

			switch(refferedBtn.pressType){
				case Hold:
					if((refferedBtn.monitorInformation>>2) >= refferedBtn.holdLength+timeToSleep){
						restartButton_Restart();
						stopTask(restartButton_monitorInput);
					}
			}
			refferedBtn.monitorInformation += (refferedBtn.monitorInformation & 1)*2;
			sleep(timeToSleep);
		}
	}
}

//------------------------------------------------------------------------------
//Button Start and Stops
//------------------------------------------------------------------------------

void restartButton_Start(restartButton_Info &type){
	if(restartButton_MonitoredButtons_GetIndexOfElement(type) == -1){
		restartButton_MonitoredButtons_AddElement(type);
		if(sizeof(restartButton_MonitoredButtons) & 1) 	startTask(restartButton_monitorInput);
	}
	type.enabled = true;
	return;
}

//stops monitoring button by ending monitor task
void restartButton_Stop(restartButton_Info &type){
	if(restartButton_MonitoredButtons_GetIndexOfElement(type) != -1){
		restartButton_MonitoredButtons_RemoveElement(type);
		if(restartButton_MonitoredButtons == 0) stopTask(restartButton_monitorInput);
	}
	type.enabled = false;
	return;
}

//------------------------------------------------------------------------------
//Reset Program Stuff
//------------------------------------------------------------------------------

//restarts main program via another thread; stopping and starting main thread
task restartProgramTaskWay(){
  stopAllMotors();
	stopTask(main);
	sleep(3000);		//reaches this code as this is performed on another thread
	startTask(main);

}

//function to call thread; makes sure program ends on the line of the command
void restartButton_Restart() {
	startTask(restartProgramTaskWay);
	sleep(50);	//enough time for restart to occur

}
