#pragma once

#include <string>

/// Client interface representing a bidder in the auction system.
class Client {
protected:
    inline static int clientIdCounter = 0;
    int clientId;
    std::string name;

public:
    Client() : clientId(++clientIdCounter) {}

    Client(const std::string& clientName) : clientId(++clientIdCounter), name(clientName) {}

    virtual ~Client() = default;

    /**
     * Receives notification about auction updates.
     *
     * @param auctionId The unique identifier of the auction
     * @param message The notification message (e.g., "New bid placed", "Auction closing soon")
     * @param currentPrice The current highest bid price
     */
    virtual void receiveNotification(int auctionId, const std::string& message, long long currentPrice) = 0;

    /**
     * Gets the unique identifier for this client/bidder.
     *
     * @return Client's unique ID (starts from 1)
     */
    int getClientId() const { return clientId; }

    /**
     * Gets the name of this client/bidder.
     *
     * @return Client's name
     */
    std::string getName() const { return name; }

    /**
     * Places a bid on an auction.
     *
     * @param auctionId The auction to bid on
     * @param bidAmount The amount to bid
     */
    virtual void placeBid(int auctionId, long long bidAmount) = 0;
};

/// Server interface representing the auction platform.
class Server {
protected:
    inline static int auctionIdCounter = 0;

public:
    virtual ~Server() = default;

    /**
     * Creates a new auction item.
     *
     * @param productName Name of the auction product
     * @param initPrice Base price for the product
     * @return The auto-generated auction ID (starts from 1)
     */
    virtual int createAuction(const std::string& productName, long long initPrice) = 0;

    /**
     * Registers a client to receive updates for a specific auction.
     *
     * @param auctionId The auction to monitor
     * @param client Pointer to the client/bidder to register (must not be nullptr)
     */
    virtual void registerClient(int auctionId, Client* client) = 0;

    /**
     * Unregisters a client from receiving updates for a specific auction.
     *
     * @param auctionId The auction to stop monitoring
     * @param client Pointer to the client/bidder to unregister (must not be nullptr)
     */
    virtual void unregisterClient(int auctionId, Client* client) = 0;

    /**
     * Processes a bid placed by a client.
     * This triggers notification logic.
     *
     * @param auctionId The auction receiving the bid
     * @param clientId The bidder's ID
     * @param bidAmount The bid amount
     * @return true if bid was successful, false otherwise
     */
    virtual bool processBid(int auctionId, int clientId, long long bidAmount) = 0;

    /**
     * Gets the current highest bid for an auction.
     *
     * @param auctionId The auction to query
     * @return The current highest bid price
     */
    virtual long long getCurrentPrice(int auctionId) = 0;

    /**
     * Closes an auction and notifies all registered clients.
     *
     * @param auctionId The auction to close
     */
    virtual void closeAuction(int auctionId) = 0;
};
