#include "auction_server.h"
#include "bidder_observer.h"
#include "log_observer.h"

#include <iostream>

int main() {
    AuctionServer* server = new AuctionServer();

    BidderObserver* qanle = new BidderObserver("qanle", server);
    BidderObserver* nagisa = new BidderObserver("nagisa", server);
    BidderObserver* an = new BidderObserver("an", server);
    // LogObserver serverLog = LogObserver("auction_server.log");

    int auction1 = server->createAuction("Miata 95", 110000000);

    std::cout << "\nRegister clients to auction\n";
    server->registerClient(auction1, qanle);
    server->registerClient(auction1, nagisa);
    server->registerClient(auction1, an);

    // AuctionSubject* auction = server->findAuction(auction1);
    // if(auction)
    //     auction->attach(&serverLog);


    std::cout << "\nBid on auction 1\n";
    qanle->placeBid(auction1, 200000000);
    nagisa->placeBid(auction1, 1000000000);
    an->placeBid(auction1, 550000000);

    server->displayAuctionInfo(auction1);

    server->unregisterClient(auction1, nagisa);
    qanle->placeBid(auction1, 1500000000);

    server->closeAuction(auction1);
    server->displayAuctionInfo(auction1);


    std::cout << "\nAUCTION 2\n";
    int auction2 = server->createAuction("Dong Son's drum", 500000);

    // auction = server->findAuction(auction2);
    // if(auction)
    //     auction->attach(&serverLog);

    server->registerClient(auction2, nagisa);
    server->registerClient(auction2, an);

    std::cout << "\nBid on auction 2\n";
    nagisa->placeBid(auction2, 510000);
    an->placeBid(auction2, 550000);

    server->closeAuction(auction2);
    server->displayAuctionInfo(auction2);

    delete qanle;
    delete nagisa;
    delete an;
    delete server;

    return 0;
}
