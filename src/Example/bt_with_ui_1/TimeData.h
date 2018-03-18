#ifndef NGSERVER_TIMEDATA_H
#define NGSERVER_TIMEDATA_H


/**
 * @brief Represents time.
 *
 * Used to pass time values to behaviour tree nodes.
 */
class TimeData {
public:
    explicit TimeData(float dt) {
        delta_time = dt;
    }

    float delta_time;
};


#endif //NGSERVER_TIMEDATA_H
