#pragma once
#include "auction_server.h"
#include "observer_subject.h"
#include "server_client.h"

#include <iostream>


class BidderObserver : public Client, public Observer {
private:
    Server* server;

public:
    BidderObserver(const std::string& name, Server* srv) : Client(name), server(srv) {
        std::cout << "Bidder created: " << name << " (ID: " << clientId << ")\n";
    }

    virtual ~BidderObserver() = default;

    void update(Subject* subject) override {
        AuctionSubject* auction = dynamic_cast<AuctionSubject*>(subject);

        if (auction) {
            int auctionId = auction->getAuctionId();
            std::string message = auction->getLastMessage();
            long long currentPrice = auction->getCurrentPrice();

            receiveNotification(auctionId, message, currentPrice);
        }
    }

    int getObserverId() const override { return clientId; }

    void receiveNotification(int auctionId, const std::string& message, long long currentPrice) override {
        std::cout << "[" << name << " (ID:" << clientId << ")]\n";
        std::cout << "Auction #" << auctionId << ": " << message << " | Current price:  " << currentPrice << "vnd\n";
    }

    void placeBid(int auctionId, long long bidAmount) override {
        std::cout << "[" << name << " (ID:" << clientId << ")]\n";
        std::cout << "Bidder place bid of " << bidAmount << "vnd on auction #" << auctionId << "\n";

        if (server) server->processBid(auctionId, clientId, bidAmount);
    }
};
