#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include <vector>

#define CONNECT_STAR 1 //LEDÇê⁄ë± Yes=1

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	void connectToMySQL();
	void resetTable();
	vector<vector<int>> readTable();
	vector<vector<int>> checkDifference(vector<vector<int>> previousTableData, vector<vector<int>> currentTableData);
	void sendPacketToLedController(vector<vector<int>> sendData);

	int timeThen = 0;
	
	vector<vector<int>> currentTableData;
	vector<vector<int>> previousTableData;
	vector<vector<int>> sendData;

	string getTime();

	//osc
	ofxOscSender snd;

	ofImage img;
};
