#pragma once
#include "observer_subject.h"
#include "server_client.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

class AuctionSubject : public Subject {
public:
    enum AuctionState { CREATED, OPEN, CLOSED };

private:
    int auctionId;
    std::string itemName;
    long long currentPrice;
    int highestBidderId;
    std::vector<Observer*> observers;
    std::string lastMessage;
    AuctionState state;

    Observer* findObserver(int observerId) {
        for (auto it : observers)
            if (it->getObserverId() == observerId) return it;
        return nullptr;
    }

public:
    AuctionSubject(int id, const std::string& name, long long initPrice)
        : auctionId(id),
          itemName(name),
          currentPrice(initPrice),
          highestBidderId(-1),
          state(CREATED) {
        lastMessage = "New auction item: " + itemName;
    }

    virtual ~AuctionSubject() = default;

    void attach(Observer* observer) override {
        if (observer) {
            // Check if observer is already attach
            auto it = std::find(observers.begin(), observers.end(), observer);
            if (it == observers.end()) {
                observers.push_back(observer);
                std::cout << "Observer ID " << observer->getObserverId() << " attach to auction #" << auctionId << "\n";
            }
        }
    }

    void detach(Observer* observer) override {
        if (observer) {
            auto it = std::find(observers.begin(), observers.end(), observer);
            if (it != observers.end()) {
                observers.erase(it);
                std::cout << "Observer ID " << observer->getObserverId() << " detach from auction #" << auctionId
                          << "\n";
            }
        }
    }

    void notify() override {
        std::cout << "\n[auction #" << auctionId << "] notify " << observers.size() << " observers:\n";

        for (Observer* observer : observers)
            if (observer) observer->update(this);
        std::cout << '\n';
    }

    void notifyObserver(int observerId) {
        std::cout << "\n[auction #" << auctionId << "] notify #" << observerId << " observer:\n";

        auto observer = findObserver(observerId);
        if (observer) observer->update(this);
    }

    bool updateBid(int bidderId, long long bidAmount) {
        if (state != OPEN) {
            lastMessage = "Auction is not open for bidding";
            notifyObserver(bidderId);
            return false;
        }

        if (bidAmount <= currentPrice) {
            lastMessage = "Bid must be higher than current price";
            notifyObserver(bidderId);
            return false;
        }

        currentPrice = bidAmount;
        highestBidderId = bidderId;
        lastMessage = "New bid by bidder #" + std::to_string(bidderId) + " - " + std::to_string(bidAmount) + "vnd";

        notify();
        return true;
    }

    void openAuction() {
        state = OPEN;
        //     lastMessage = "Auction is now OPEN for bidding";
        //     notify();
    }

    void closeAuction() {
        state = CLOSED;
        if (highestBidderId != -1) {
            lastMessage = "Auction CLOSED. Winner: Bidder #" + std::to_string(highestBidderId) + " with "
                          + std::to_string(currentPrice) + "vnd";
        } else
            lastMessage = "Auction CLOSED with no bidder";

        notify();
    }

    int getAuctionId() const { return auctionId; }
    std::string getItemName() const { return itemName; }
    long long getCurrentPrice() const { return currentPrice; }
    int getHighestBidderId() const { return highestBidderId; }
    std::string getLastMessage() const { return lastMessage; }
    AuctionState getState() const { return state; }

    std::string getStateString() const {
        switch (state) {
            case CREATED:
                return "CREATED";
            case OPEN:
                return "OPEN";
            case CLOSED:
                return "CLOSED";
            default:
                return "UNKNOWN";
        }
    }
};



class AuctionServer : public Server {
private:
    /// manages auction using auctionID
    std::map<int, AuctionSubject> auctions;

public:
    AuctionServer() = default;
    virtual ~AuctionServer() = default;

    int createAuction(const std::string& productName, long long initPrice) override {
        int auctionId = ++auctionIdCounter;

        auctions.insert({auctionId, AuctionSubject(auctionId, productName, initPrice)});

        std::cout << "Auction #" << auctionId << " set: " << productName << " (starting price: " << initPrice
                  << "vnd)\n";

        auctions.at(auctionId).openAuction();
        return auctionId;
    }

    AuctionSubject* findAuction(int auctionId) {
        auto it = auctions.find(auctionId);
        if (it == auctions.end()) {
            std::cerr << "Error: Cannot find auction #" << auctionId << '\n';
            return nullptr;
        }
        return &it->second;
    }

    void registerClient(int auctionId, Client* client) override {
        AuctionSubject* auction = findAuction(auctionId);
        if (!auction) return;

        Observer* observer = dynamic_cast<Observer*>(client);
        if (observer) auction->attach(observer);
    }

    void unregisterClient(int auctionId, Client* client) override {
        AuctionSubject* auction = findAuction(auctionId);
        if (!auction) return;

        Observer* observer = dynamic_cast<Observer*>(client);
        if (observer) auction->detach(observer);
    }

    bool processBid(int auctionId, int clientId, long long bidAmount) override {
        AuctionSubject* auction = findAuction(auctionId);
        if (!auction) return false;

        return auction->updateBid(clientId, bidAmount);
    }

    long long getCurrentPrice(int auctionId) override {
        AuctionSubject* auction = findAuction(auctionId);
        if (!auction) return -1;

        return auction->getCurrentPrice();
    }

    void closeAuction(int auctionId) override {
        AuctionSubject* auction = findAuction(auctionId);
        if (!auction) return;

        auction->closeAuction();
    }

    void displayAuctionInfo(int auctionId) {
        AuctionSubject* auction = findAuction(auctionId);
        if (!auction) return;

        std::cout << "\nAuction #" << auction->getAuctionId();
        std::cout << "\nItem: " << auction->getItemName();
        std::cout << "\nState: " << auction->getStateString();
        std::cout << "\nCurrent Price: " << auction->getCurrentPrice() << "vnd";
        std::cout << "\nHighest Bidder: ";
        std::cout << (auction->getHighestBidderId() != -1 ? "bidder #" + std::to_string(auction->getHighestBidderId())
                                                          : "None");
        std::cout << "\nLast Message: " << auction->getLastMessage() << "\n\n";
    }
};
