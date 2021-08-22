#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include <vector>

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void connectToMySQL();
	void resetTable();
	void sendPacketToLedController(vector<vector<int>> sendData);
	vector<vector<int>> readTable();
	vector<vector<int>> checkDifference(vector<vector<int>> previousTableData, vector<vector<int>> currentTableData);
	string getTime();

	int timeThen = 0;
	
	vector<vector<int>> currentTableData;
	vector<vector<int>> previousTableData;
	vector<vector<int>> sendData;

	ofxOscSender snd;
	ofImage img;
};
