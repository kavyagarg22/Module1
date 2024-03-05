#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <iomanip>
#include <algorithm>

using namespace std;

mutex trafficMutex;

// Initializing the number of threads for producers and consumers
int numProducers = 2;
int numConsumers = 2;

int rowsPerHour = 48; // This value will be used to check if an hour passed by (48 rows for an hour)

int producerCount = 0; // Producer counter
int consumerCount = 0; // Consumer counter
int totalRows = 0; // Total number of rows

condition_variable producerCV, consumerCV; // Initializing condition variables for producers and consumers

// String variables and vectors are initialized to get values from the data file
string rowIndex, timeStamp, trafficLightId, numberOfCars;
vector<int> indices;
vector<int> trafficLightIds;
vector<int> carCounts;
vector<string> timeStamps;

// Struct for traffic data row
struct TrafficSignal
{
    int index;
    std::string timeStamp;
    int trafficLightId;
    int numCars;
};

// TrafficSignal array of four is initialized to hold the totals of each of the 4 traffic lights
TrafficSignal signalSorter[4] = {{0, "", 1, 0}, {0, "", 2, 0}, {0, "", 3, 0}, {0, "", 4, 0}};

// Queue to store traffic light data
queue<TrafficSignal> trafficSignalQueue;
TrafficSignal signal;

// Function to sort traffic light data
bool sortMethod(struct TrafficSignal first, struct TrafficSignal second)
{
    if (first.numCars > second.numCars)
        return 1;
    return 0;
}

void *produce(void *args)
{
    while (producerCount < totalRows)
    {
        unique_lock<mutex> lock(trafficMutex); // Locking until producer finishes processing 

        if (producerCount < totalRows) // If count is less than the number of rows in the dataset 
        {
            trafficSignalQueue.push(TrafficSignal{indices[producerCount], timeStamps[producerCount], trafficLightIds[producerCount], carCounts[producerCount]}); // Push into queue
            consumerCV.notify_all(); // Notifying consumer threads
            producerCount++;
        }
        else
        {
            producerCV.wait(lock, [] { return producerCount < totalRows; }); // If count is greater than the number of rows in the data set, wait
        }

        lock.unlock(); // Unlock after processing
        sleep(rand() % 3);
    }
}

void *consume(void *args)
{
    while (consumerCount < totalRows)
    {
        unique_lock<mutex> lock(trafficMutex); // Lock until processing

        if (!trafficSignalQueue.empty())
        {
            signal = trafficSignalQueue.front(); // Getting the front elements of the queue
            
            // Add the number of cars into the respective traffic light id
            if (signal.trafficLightId == 1)
            {
                signalSorter[0].numCars += signal.numCars;
            }
            else if (signal.trafficLightId == 2)
            {
                signalSorter[1].numCars += signal.numCars;
            }
            else if (signal.trafficLightId == 3)
            {
                signalSorter[2].numCars += signal.numCars;
            }
            else if (signal.trafficLightId == 4)
            {
                signalSorter[3].numCars += signal.numCars;
            }
            
            trafficSignalQueue.pop(); // Pop the data
            producerCV.notify_all(); // Notify producer
            consumerCount++;
        }
        else
        {
            consumerCV.wait(lock, [] { return !trafficSignalQueue.empty(); }); // If queue is empty, wait until producer produces
        }

        if (consumerCount % rowsPerHour == 0)
        {
            // Check if an hour passed by, checking every 48th row
            sort(signalSorter, signalSorter + 4, sortMethod); // Sorting data
            printf("Traffic lights sorted according to most busy | Time: %s \n", signal.timeStamp.c_str());
            cout << "Traffic Light" << "\t" << "Number of Cars" << endl;
            cout << signalSorter[0].trafficLightId << "\t" << "\t" << signalSorter[0].numCars << endl;
            cout << signalSorter[1].trafficLightId << "\t" << "\t" << signalSorter[1].numCars << endl;
            cout << signalSorter[2].trafficLightId << "\t" << "\t" << signalSorter[2].numCars << endl;
            cout << signalSorter[3].trafficLightId << "\t" << "\t" << signalSorter[3].numCars << endl;
        }

        lock.unlock();
        sleep(rand() % 3);
    }
}

// Function to get data from file
void getTrafficData()
{
    ifstream infile;

    string filename;
    cout << "Enter the filename: ";
    cin >> filename;

    infile.open(filename);

    if (infile.is_open())
    {
        std::string line;
        getline(infile, line);

        while (!infile.eof())
        {
            getline(infile, rowIndex, ',');
            indices.push_back(stoi(rowIndex));
            getline(infile, timeStamp, ',');
            timeStamps.push_back(timeStamp);
            getline(infile, trafficLightId, ',');
            trafficLightIds.push_back(stoi(trafficLightId));
            getline(infile, numberOfCars, '\n');
            carCounts.push_back(stoi(numberOfCars));

            totalRows += 1;
        }
        infile.close();
    }
    else
    {
        printf("Could not open file, try again.");
    }
}

int main()
{
    getTrafficData();

    pthread_t producers[numProducers];
    pthread_t consumers[numConsumers];

    for (long i = 0; i < numProducers; i++)
        pthread_create(&producers[i], NULL, produce, (void *)i);
    for (long i = 0; i < numConsumers; i++)
        pthread_create(&consumers[i], NULL, consume, (void *)i);

    for (long i = 0; i < numProducers; i++)
        pthread_join(producers[i], NULL);
    for (long i = 0; i < numConsumers; i++)
        pthread_join(consumers[i], NULL);

    return 0;
}
