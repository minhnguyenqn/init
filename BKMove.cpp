#include "BKMove.h"

void BKMove::addRoute(BusRoute* route) {
    if (route == nullptr) throw invalid_argument("route must not be null");
    routes.push_back(route);
}

int BKMove::getRouteCount() const {
    return static_cast<int>(routes.size());
}

BusRoute* BKMove::getRoute(int index) {
    if (index < 0 || index >= static_cast<int>(routes.size())) {
        throw out_of_range("BKMove route index is out of range");
    }
    return routes[index];
}

vector<RouteResult> BKMove::findDirectRoutes(string fromStopId, string toStopId) {
    // TODO Q5.1
    vector<RouteResult> results;

    for (BusRoute* route : routes) {
        for (int i = 0; i < 2; ++i) {
            Direction direction = static_cast<Direction>(i);
            int hops = route->getHopCount(
                fromStopId, toStopId, direction
            );
            if (hops >= 0) {
                results.push_back(
                    RouteResult(route->getId(), direction, hops)
                );
            }
        }
}
    int (*compare)(RouteResult&, RouteResult&) = [](RouteResult& a, RouteResult& b) -> int {
            if (a.hopCount != b.hopCount) {
                return a.hopCount < b.hopCount ? -1 : 1;
            }

            if (a.routeId != b.routeId) {
                return a.routeId < b.routeId ? -1 : 1;
            }

            if (a.direction != b.direction) {
                return a.direction < b.direction ? -1 : 1;
            }

            return 0;
        };

    QuickSort<RouteResult> sorter;
    sorter.sort(results.data(),static_cast<int>(results.size()),compare);

    return results;
}

vector<JourneyResult> BKMove::findJourneys(string fromStopId, string toStopId) {
    // TODO Q5.2
    vector<JourneyResult> results;

    vector<RouteResult> directRoutes =
        findDirectRoutes(fromStopId, toStopId);

    for (const RouteResult& result : directRoutes) {
        results.push_back(
            JourneyResult(result.routeId,result.direction,result.hopCount)
        );
    }
    for (BusRoute* firstRoute : routes) {
        for (BusRoute* secondRoute : routes) {
            if (firstRoute->getId() == secondRoute->getId()) {
                continue;
            }

            for (int d1 = 0; d1 < 2; ++d1) {
                Direction firstDirection =
                    static_cast<Direction>(d1);

                int firstStopCount =
                    firstRoute->getStopCount(firstDirection);

                int originIndex = -1;

                for (int i = 0; i < firstStopCount; ++i) {
                    if (firstRoute->getStop(i, firstDirection).getId()
                        == fromStopId) {
                        originIndex = i;
                        break;
                    }
                }
                if (originIndex == -1) {
                    continue;
                }
                for (int d2 = 0; d2 < 2; ++d2) {
                    Direction secondDirection =
                        static_cast<Direction>(d2);

                    int secondStopCount =
                        secondRoute->getStopCount(secondDirection);

                    int destinationIndex = -1;

                    for (int j = 0; j < secondStopCount; ++j) {
                        if (secondRoute->getStop(j, secondDirection).getId()
                            == toStopId) {
                            destinationIndex = j;
                            break;
                        }
                    }

                    if (destinationIndex == -1) {
                        continue;
                    }

                    for (int i = originIndex + 1;
                         i < firstStopCount; ++i) {
                        string transferStopId = firstRoute->getStop(i, firstDirection).getId();

                        for (int j = 0; j < destinationIndex; ++j) {
                            if (secondRoute->getStop(j, secondDirection).getId()
                                == transferStopId) {
                                int totalHops =(i - originIndex) +(destinationIndex - j);
                                results.push_back(
                                    JourneyResult(firstRoute->getId(),firstDirection,transferStopId,secondRoute->getId(),secondDirection,totalHops)
                                );
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    int (*compare)(JourneyResult&, JourneyResult&) =
        [](JourneyResult& a, JourneyResult& b) -> int {
            if (a.transfers != b.transfers) {
                return a.transfers < b.transfers ? -1 : 1;
            }

            if (a.totalHops != b.totalHops) {
                return a.totalHops < b.totalHops ? -1 : 1;
            }

            if (a.firstRouteId != b.firstRouteId) {
                return a.firstRouteId < b.firstRouteId ? -1 : 1;
            }

            if (a.firstDirection != b.firstDirection) {
                return a.firstDirection < b.firstDirection ? -1 : 1;
            }

            if (a.transferStopId != b.transferStopId) {
                return a.transferStopId < b.transferStopId ? -1 : 1;
            }

            if (a.secondRouteId != b.secondRouteId) {
                return a.secondRouteId < b.secondRouteId ? -1 : 1;
            }

            if (a.secondDirection != b.secondDirection) {
                return a.secondDirection < b.secondDirection ? -1 : 1;
            }

            return 0;
        };

    QuickSort<JourneyResult> sorter;
    sorter.sort(results.data(),static_cast<int>(results.size()),compare);
    return results;
}
