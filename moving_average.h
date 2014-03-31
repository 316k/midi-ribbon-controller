#ifndef MovingAverage_H
#define MovingAverage_H

#define SIZE 25

class MovingAverage {

  private:
    int values[SIZE];
    int size;

  public:
    /**
     * Constructor
     */
    MovingAverage() {
        size = SIZE;
        for(int i = 0; i < size; i++) {
            values[i] = 0;
        }
    }

    /**
     * @return the average of the stored values.
     */
    int average() {
        int total = 0;
        int i;

        for(i = 0; i < used_size(); i++) {
            total += values[i];
        }

        return total / i;
    }

    int median() {
        int max = 0;
        int min = 0;

        for(int i = 0; i < SIZE && values[i] != 0; i++) {
            min = (min == 0 || values[i] < min ? values[i] : min);
            max = (max == 0 || values[i] > max ? values[i] : max);
        }

        return (max + min)>>1; // bitshift to divide per two 
    }

    int max() {
        int max = 0;

        for(int i = 0; i < used_size(); i++) {
            max = (max == 0 || values[i] > max ? values[i] : max);
        }

        return max;
    }

    /**
     * @return the minimum value.
     */
    int min() {
        int min = 0;

        for(int i = 0; i < used_size() && values[i] != 0; i++) {
            min = (min == 0 || values[i] < min ? values[i] : min);
        }

        return min;
    }

    /**
     * Adds a value in the array. Note : adding a 0 will screw up your moving
     * average, since the first 0 is considered to be the end of the stored
     * values.
     *
     * @return the position at which the value has been added.
     */
    int add(int value) {
        if(!value) {
            return -1;
        }

        int i = used_size();

        if(i == SIZE) {
            // Remove the oldest value and free the last space
            for(i = 1; i < SIZE && values[i] != 0; i++) {
                values[i-1] = values[i];
            }
            i--;
        }

        values[i] = value;

        return i;
    }

    /**
     * Returns the used size (aka. the number of values stored)
     */
    int used_size() {
        int i = 0;

        while(i < SIZE && values[i] != 0) {
            i++;
        }

        return i;
    }

    /**
     * @return the total size available
     */
    int available_size() {
        return SIZE;
    }

    /**
     * Sets every element to zero
     */
    void reset() {
        for(int i = 0; i < size; i++) {
            values[i] = 0;
        }
    }

    /**
     * @return the value at position
     */
    int get(int position) {
        return values[position];
    }
};
#endif
