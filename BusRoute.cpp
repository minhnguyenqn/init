#include "BusRoute.h"

BusRoute::BusRoute(string routeId)
    : routeId(routeId), outboundCount(0), built(false) {}

string BusRoute::getId() const {
    return routeId;
}

bool BusRoute::isBuilt() const {
    return built;
}

int BusRoute::getStopCount(Direction direction) {
    // TODO Q3
    if (!built) {
        return 0;
    }

    if (direction == OUTBOUND) {
        return outboundCount;
    }
    return stops.size() - outboundCount + 2;
}


int BusRoute::physicalIndex(int index, Direction direction) {
    // TODO Q3
    if (index < 0 || index >= getStopCount(direction)) {
        throw out_of_range("Index is out of range");
    }
    if (direction == OUTBOUND) {
        return index;
    }
    return (outboundCount - 1 + index) % stops.size();
}

BusStop& BusRoute::getStop(int index, Direction direction) {
    // TODO Q3
    return stops.get(physicalIndex(index, direction));

}

void BusRoute::build(SLinkedList<BusStop>& outbound, SLinkedList<BusStop>& inbound) {
    // TODO Q3
    built = false;
    outboundCount = 0;
    stops.clear();
    // Thêm toàn bộ các trạm lượt đi.
    for (auto it = outbound.begin(); it != outbound.end(); ++it) {
        stops.add(*it);
    }
    int index = 0;
    int inboundCount = inbound.size();
    for (auto it = inbound.begin(); it != inbound.end(); ++it) {
        if (index > 0 && index < inboundCount - 1) {
            stops.add(*it);
        }
        ++index;
    }
    outboundCount = outbound.size();
    built = true;
}

int BusRoute::getHopCount(string fromStopId, string toStopId, Direction direction) {
    // TODO Q3
    int fromIndex = -1;
    int toIndex = -1;
    int stopCount = getStopCount(direction);
    for (int i = 0; i < stopCount; ++i) {
        string id = getStop(i, direction).getId();
        if (id == fromStopId) {
            fromIndex = i;
        }
        if (id == toStopId) {
            toIndex = i;
        }
    }
    if (fromIndex == -1 || toIndex == -1 || fromIndex > toIndex) {
        return -1;
    }
    return toIndex - fromIndex;
}
