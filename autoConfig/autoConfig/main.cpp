//
//  main.cpp
//  autoConfig
//
//  Created by Wei Fang on 2/24/15.
//  Copyright (c) 2015 Wei Fang. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <exception>

using namespace std;

class AutoConfig {
    string hostname;
    string dbAdpter;
    string dbUser;
    string dbPwd;
    string dbName;
    string location;
    string serverName[10];
    string serverString;
public:
    AutoConfig(){};
    ~AutoConfig();
    void addHost();
    void addDbUser();
    void addDbPwd();
    void addDbName();
    void addServerName(int flag);
    void parseCond(int flag);
    void addLocation();
    void addHeader();
    void addFooter();
    bool writeToConfig();
    void addConfig();
};

void AutoConfig::addHost() {
    cout<< "Please enter the HOST: ";
    string host;
    cin>>host;
    this->hostname = host;
}

void AutoConfig::addDbUser() {
    cout<< "Please enter the DB User: ";
    string dbUser;
    cin>>dbUser;
    this->dbUser = dbUser;
}

void AutoConfig::addDbPwd() {
    cout<< "Please enter the DB Password: ";
    string dbPwd;
    cin>>dbPwd;
    this->dbPwd = dbPwd;
}

void AutoConfig::addDbName() {
    cout<< "Please enter the DB Name: ";
    string dbName;
    cin>>dbName;
    this->dbName = dbName;
}

void AutoConfig::addServerName(int flag) {
    int i = 0;
    string server;
    do{
        cout<< "Please enter Server Name " << i + 1 << "(enter 'q' to quit) : ";
        cin>>server;
        this->serverName[i++] = server;
    } while(server != "q");
    this->serverName[i-1] = "\0";
    this->parseCond(flag);
}

void AutoConfig::parseCond(int flag) {
    string condition;
    condition = flag <= 1 ? "if( " : "else if( ";
    if (!this->serverName[1].empty()) {
        condition += "$_SERVER['SERVER_NAME'] === '" + this->serverName[0] + "'";
        for (int i=1; !this->serverName[i].empty(); i++) {
            condition += " || $_SERVER['SERVER_NAME'] ===  '" + this->serverName[i] + "'";
        }
        condition += " )";
        this->serverString = condition;
    } else {
        this->serverString = "if( $_SERVER['SERVER_NAME'] === '" + this->serverName[0] + "' )";
    }
//    cout<< this->serverString <<endl;
}

void AutoConfig::addLocation() {
    cout<< "Absolute Path with File name : ";
    string location;
    cin>>location;
    
    this->location = location;// + "/config.php";
//    cout <<this->location <<endl;
}

void AutoConfig::addHeader() {
    ifstream my_file(this->location);
    if (my_file.good())
    {
        cout<< "File already exists!!" <<endl;
        exit(0);
    } else {
        ofstream file;
        file.open(this->location,ios::app);
        file << "<?php" << endl;
        file.close();
    }
}

void AutoConfig::addFooter() {
    ofstream file;
    file.open(this->location,ios::app);
    file <<
    "$mysqli = new mysqli(DB_SERVER,DB_USER,DB_PASS,DB_NAME);\n\
	if ($mysqli->connect_errno) {\n\
	    die(\"Failed to connect to MySQL: (\" . $mysqli->connect_errno . \") \" . $mysqli->connect_error);\n\
    }" <<endl;
    file.close();
}

bool AutoConfig::writeToConfig() {
    ofstream file;
    file.open(this->location,ios::app);
    file <<
    this->serverString + "\n\
    {\n\
        define(\"ENV\", \"\");\n\
        define(\"BASE_URL\", \"\");\n\
        define(\"RETURN_URL\", \"\");\n\
        define(\"FB_APP\", \"\");\n\
        define(\"CDN_URL\",\"\");\n\
        define(\"DB_SERVER\", '" + this->hostname + "');\n\
        define(\"DB_USER\", '" + this->dbUser + "');\n\
        define(\"DB_PASS\", '" + this->dbPwd + "');\n\
        define(\"DB_NAME\", '" + this->dbName + "');\n\
    }" <<endl;
    file.close();
    return true;
}

void AutoConfig::addConfig() {
    string input;
    int flag = 1;
    do{
        this->addHost();
        this->addDbUser();
        this->addDbPwd();
        this->addDbName();
        this->addServerName(flag);
        this->writeToConfig();
        cout << "Add Another Config? (Y/n)";
        cin >> input;
        flag++;
    } while(input == "Y" || input == "y" || input == "Yes" || input == "yes");
}

int main(int argc, const char * argv[])
{
    AutoConfig * a;
    a = new AutoConfig();
    cout << "Add a New Config? (Y/n)";
    string input;
    cin >> input;
    if(input == "Y" || input == "y" || input == "Yes" || input == "yes") {
        a->addLocation();
        a->addHeader();
        a->addConfig();
        a->addFooter();
        cout << "..." << endl;
        cout << "Generated successfully! Thanks!" << endl;
    }
    return 0;
}

