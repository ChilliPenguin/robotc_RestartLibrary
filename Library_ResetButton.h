//******  HOW TO USE
//
// YOU MUST INTILIZE THE rbAmount, as c cant support dynamic arrays(and robotc doesen't allow relloc())

int rbAmount = -1; //SET THIS UP


int reset_Period = 1250;
enum btnPressType{Hold};

//data structure of the button
struct restartButton_Info{
	tSensors     locationPort;
	btnPressType pressType;
	int 			   holdLength;
	bool 				 enabled;
	//This value should not be modifided by the main program!
	unsigned int monitorInformation; //aaaaabc, a-timeheld, b-pressedBefore, c-pressedNow
};

//precalls functions and tasks
task main();
void restartButton_Restart();
void restartButton_Start(restartButton_Info &type);
void restartButton_Stop(restartButton_Info &type);


//------------------------------------------------------------------------------
//Constructor of struct
//------------------------------------------------------------------------------

void restartButton_Create(restartButton_Info &targetedButn, tSensors locationPort,btnPressType pressType,int holdLength){
	targetedButn.locationPort = locationPort;
	targetedButn.pressType    = pressType;
	targetedButn.holdLength   = holdLength;
	targetedButn.enabled      = false;
}

restartButton_Info monitoredInput;
restartButton_Info settedInput;

restartButton_Info restartButton_MonitoredButtons[1];

//------------------------------------------------------------------------------
//Array Managers
//------------------------------------------------------------------------------

restartButton_Info test; //change name!
int restartButton_MonitoredButtons_GetIndexOfElement(restartButton_Info rB_I){
	for(int i = 0;i< sizeof(restartButton_MonitoredButtons)/sizeof(restartButton_MonitoredButtons[0]);i++){
		test = restartButton_MonitoredButtons[i];
		if(rB_I == restartButton_MonitoredButtons[i]){
			return i;
		}
	}
	return -1;
}

int restartButton_MonitoredButtons_AddElement(restartButton_Info resButton_Used){
	for(int i = 0;i<sizeof(restartButton_MonitoredButtons)/sizeof(restartButton_MonitoredButtons[0]);i++){
		if(restartButton_MonitoredButtons[i] == 0){
			restartButton_MonitoredButtons[i] = resButton_Used;
			return 0;
		}
	}
	return 1;
}


int restartButton_MonitoredButtons_RemoveElement(restartButton_Info resButton_Used){
	int indexPos = restartButton_MonitoredButtons_GetIndexOfElement(resButton_Used;
	if(indexPos<0) return 1;
	restartButton_MonitoredButtons[indexPos] = 0;
	return 0;

}

int restartButton_MonitoredButtons_AmountOfButtonsActive(){
	int amtOfNulls = 0;
	for(int i = 0;i<rbAmount-1;i++){
		if(restartButton_MonitoredButtons[i] == 0){
			amtOfNulls++;
		}
	}
	return amtOfNulls;
}
//------------------------------------------------------------------------------
//MonitorInputs
//------------------------------------------------------------------------------

restartButton_Info refferedBtn; //preset
task restartButton_monitorInput(){
	int  timeToSleep   = 250;
	while(true){
		for(int a = 0;a<sizeof(restartButton_MonitoredButtons)/sizeof(restartButton_MonitoredButtons[0]);a++){
			//WHY
			if(SensorValue(restartButton_MonitoredButtons[a].locationPort)){
					restartButton_MonitoredButtons[a].monitorInformation |= 1;
			}else  restartButton_MonitoredButtons[a].monitorInformation &= 0xFE; //sets first bit to 0
			if(restartButton_MonitoredButtons[a].monitorInformation & 1){
				restartButton_MonitoredButtons[a].monitorInformation += timeToSleep*0b100;
			}else {
				restartButton_MonitoredButtons[a].monitorInformation ^= (((refferedBtn.monitorInformation>>2))<<2);
			}

			switch(restartButton_MonitoredButtons[a].pressType){
				case Hold:
					if((restartButton_MonitoredButtons[a].monitorInformation>>2) >= restartButton_MonitoredButtons[a].holdLength){
						restartButton_MonitoredButtons[a].monitorInformation = 0;
						restartButton_Restart();
						stopTask(restartButton_monitorInput);
					}
			}
			//restartButton_MonitoredButtons[a].monitorInformation += (restartButton_MonitoredButtons[a].monitorInformation & 1)*2;
		}
	}
}

//------------------------------------------------------------------------------
//Button Start and Stops
//------------------------------------------------------------------------------

void restartButton_Start(restartButton_Info &type){	//int to manage return issues
	if(rbAmount < 1) return;
	if(restartButton_MonitoredButtons_GetIndexOfElement(type) == -1){
		restartButton_MonitoredButtons_AddElement(type);
	}
	if(restartButton_MonitoredButtons_AmountOfButtonsActive() == rbAmount-1)
		startTask(restartButton_monitorInput);
	type.enabled = true;
	return;
}

//stops monitoring button by ending monitor task
void restartButton_Stop(restartButton_Info &type){
	if(rbAmount < 1) return;
	if(restartButton_MonitoredButtons_GetIndexOfElement(type) != -1){
		restartButton_MonitoredButtons_RemoveElement(type);
		if(restartButton_MonitoredButtons_AmountOfButtonsActive() == rbAmount) stopTask(restartButton_monitorInput);
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
