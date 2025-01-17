#include "subscription.h"

/**
 * @brief Subscription::operator == Compares subscription equality based on client id only.
 * @param rhs Right-hand side.
 * @return true or false
 *
 * QoS is not used in the comparision. This means you upgrade your QoS by subscribing again. The
 * specs don't specify what to do there.
 */
bool Subscription::operator==(const Subscription &rhs) const
{
    if (session.expired() && rhs.session.expired())
        return true;
    if (session.expired() || rhs.session.expired())
        return false;

    const std::shared_ptr<Session> lhs_ses = session.lock();
    const std::shared_ptr<Session> rhs_ses = rhs.session.lock();

    return lhs_ses && rhs_ses && lhs_ses->getClientId() == rhs_ses->getClientId();
}

void Subscription::reset()
{
    session.reset();
    qos = 0;
}

