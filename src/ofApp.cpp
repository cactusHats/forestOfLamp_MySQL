#include "ofApp.h"
#include "define.h"

#include <stdlib.h>
#include <iostream>
//#include "stdafx.h"

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>

#include <time.h>

using namespace std;
using namespace sql;

Driver* driver;
Connection* con;
Statement* stmt;
PreparedStatement* pstmt;
ResultSet *result;

//--------------------------------------------------------------
void ofApp::setup() {

	img.load("database.png");

	//osc通信
	snd.setup(IP_HOST, PORT_TO_LCON);

	connectToMySQL(); //MySQLへ接続
	//resetTable(); //DBのリセット

	currentTableData = readTable(); //テーブルデータの参照
	previousTableData = currentTableData; //データの更新

	cout << "---------------------" << endl;
	cout << "--- Process start ---"	<< endl;
	cout << "---------------------" << endl;
	cout << endl;
}

//--------------------------------------------------------------
void ofApp::update() {

	if ((clock() - timeThen) > READ_INTERVAL * 1000) {
		currentTableData = readTable(); //テーブルデータの参照
		sendData = checkDifference(previousTableData, currentTableData); //前回読み取りデータとの差異の確認
		sendPacketToLedController(sendData); //データの送信
		
		previousTableData = currentTableData; //データの更新

		timeThen = clock();
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	img.draw(22, 22);
}

//--------------------------------------------------------------
void ofApp::connectToMySQL() {
	try {
		driver = get_driver_instance();
		con = driver->connect("tcp://40.76.87.126:3306", "pm", "masashi23_");
		cout << "---------------------" << endl;
		cout << "Connection succeeded!" << endl;
		cout << "---------------------" << endl;
		cout << endl;
	}
	catch (sql::SQLException e) {
		cout << "Could not connect to server. Error message: " << e.what() << endl;
		system("pause");
	}

	con->setSchema("pm");
}

//--------------------------------------------------------------
void ofApp::resetTable() {

	con->setSchema("pm");

	cout << "Table resetting..." << endl;
	cout << endl;

	for (int i = 1; i <= 84; i++) {
		pstmt = con->prepareStatement("UPDATE lampColorData SET count = ? WHERE id = ?");
		pstmt->setInt(1, 0); //count
		pstmt->setInt(2, i); //id
		pstmt->executeQuery();
	}
	
	cout << "----------------------" << endl;
	cout << "Table reset succeeded!" << endl;
	cout << "----------------------" << endl;

}

//--------------------------------------------------------------
vector<vector<int>> ofApp::readTable() {
	pstmt = con->prepareStatement("SELECT * FROM lampColorData;");
	result = pstmt->executeQuery();

	vector<int> tmp;
	vector<vector<int>> tableData;
	
	tableData.clear();

	while (result->next()) {
		tmp.clear();
		tmp.push_back(result->getInt(1)); //id
		tmp.push_back(result->getInt(2)); //red
		tmp.push_back(result->getInt(3)); //green
		tmp.push_back(result->getInt(4)); //blue
		tmp.push_back(result->getInt(5)); //counter
		tableData.push_back(tmp);
		//printf("Reading from table = (%d, %d, %d, %d, %d)\n", tmp[0], tmp[1], tmp[2], tmp[3], tmp[4]);
	};

	/*
	for (int i = 0; i < tableData.size(); i++) {
		printf("Reading from table = (%d, %d, %d, %d, %d)\n", tableData[i].at(0), tableData[i].at(1), tableData[i].at(2), tableData[i].at(3), tableData[i].at(4));
	}
	cout << "----------" << endl;
	*/

	return tableData;
}

//--------------------------------------------------------------
vector<vector<int>> ofApp::checkDifference(vector<vector<int>> previousTableData, vector<vector<int>> currentTableData) {
	vector<vector<int>> sendData;
	sendData.clear();

	for (int i = 0; i < currentTableData.size(); i++) {
		if (previousTableData[i].at(4) != currentTableData[i].at(4)) { //countを比較
			sendData.push_back(currentTableData[i]);
		}
	}

	/*
	for (int i = 0; i < sendData.size(); i++) {
		printf("difference = %d, %d, %d, %d, %d\n", sendData[i].at(0), sendData[i].at(1), sendData[i].at(2), sendData[i].at(3), sendData[i].at(4));
	}
	*/

	return sendData;
}

//--------------------------------------------------------------
void ofApp::sendPacketToLedController(vector<vector<int>> sendData) {

	for (int i = 0; i < sendData.size(); i++) {
		ofxOscMessage m;
		m.setAddress("/lamp/rgbVal");

		sendData[i].at(0) -= 1; //id:1-84を0-83に変更
		m.addIntArg(sendData[i].at(0)); //id 
		m.addIntArg(sendData[i].at(1)); //red
		m.addIntArg(sendData[i].at(2)); //green
		m.addIntArg(sendData[i].at(3)); //blue

		snd.sendMessage(m);

		printf("send data = ID=%d, R=%d, G=%d, B=%d, ", sendData[i].at(0), sendData[i].at(1), sendData[i].at(2), sendData[i].at(3));
		cout << getTime() << endl;
	}
}

//--------------------------------------------------------------
string ofApp::getTime() {

	string s, m, h;

	if (ofGetSeconds() < 10)
		s = "0" + ofToString(ofGetSeconds(), 0);
	else
		s = ofToString(ofGetSeconds(), 0);
	if (ofGetMinutes() < 10)
		m = "0" + ofToString(ofGetMinutes(), 0);
	else
		m = ofToString(ofGetMinutes(), 0);

	if (ofGetHours() < 10)
		h = "0" + ofToString(ofGetHours(), 0);
	else
		h = ofToString(ofGetHours(), 0);

	return h + ":" + m + ":" + s;
}