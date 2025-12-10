#pragma once

class Subject;

/**
 * Observer interface using the "pull" model. 
 * Observers are notified via update() and pull data from the Subject.
 */
class Observer {
public:
    virtual ~Observer() = default;

    /**
     * Called when the subject's state changes.
     *
     * @param subject Pointer to the subject that triggered the update
     */
    virtual void update(Subject* subject) = 0;

    /**
     * Gets the unique identifier for this observer.
     *
     * @return Observer's unique ID
     */
    virtual int getObserverId() const = 0;
};

class Subject {
public:
    virtual ~Subject() = default;

    /**
     * Attaches an observer to this subject.
     *
     * @param observer Pointer to the observer to attach
     */
    virtual void attach(Observer* observer) = 0;

    /**
     * Detaches an observer from this subject.
     *
     * @param observer Pointer to the observer to detach
     */
    virtual void detach(Observer* observer) = 0;

    /// Notifies all attached observers of a state change.
    virtual void notify() = 0;
};
