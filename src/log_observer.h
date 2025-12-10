#pragma once
#include "auction_server.h"
#include "observer_subject.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

class LogObserver : public Observer {
private:
    inline static int logObserverIdCounter = 0;
    int observerId;
    std::string logFileName;
    std::ofstream logFile;

    void writeLog(const std::string& message) {
        if (logFile.is_open()) logFile << message << std::endl;
    }

public:
    LogObserver(const std::string& filename) : observerId(++logObserverIdCounter), logFileName(filename) {
        logFile.open(logFileName, std::ios::app);
        if (logFile.is_open()) {
            std::cout << "LogObserver #" << observerId << " created (logging to: " << logFileName << ")\n";
            writeLog("Log Observer #" + std::to_string(observerId) + " initialized");
        } else
            std::cerr << "Error: Cannot open log file:  " << logFileName << "\n";
    }

    virtual ~LogObserver() {
        if (logFile.is_open()) {
            writeLog("Log Observer #" + std::to_string(observerId) + " shutting down");
            logFile.close();
        }
        std::cout << "LogObserver #" << observerId << " destroyed\n";
    }

    void update(Subject* subject) override {
        AuctionSubject* auction = dynamic_cast<AuctionSubject*>(subject);

        if (auction) {
            int auctionId = auction->getAuctionId();
            std::string itemName = auction->getItemName();
            std::string state = auction->getStateString();
            long long currentPrice = auction->getCurrentPrice();
            int highestBidderId = auction->getHighestBidderId();
            std::string message = auction->getLastMessage();

            std::ostringstream logEntry;
            logEntry << "AUCTION #" << auctionId << " [" << itemName << "] "
                     << "| State: " << state << " | Price: " << currentPrice << " VND "
                     << " | Highest Bidder: ";

            if (highestBidderId != -1)
                logEntry << "#" << highestBidderId;
            else
                logEntry << "None";

            logEntry << " | Event: " << message;


            writeLog(logEntry.str());

            // std::cout << "[LogObserver #" << observerId << "] Logged event for Auction #" << auctionId << "\n";
        }
    }

    int getObserverId() const override { return observerId; }

    std::string getLogFileName() const { return logFileName; }
};
