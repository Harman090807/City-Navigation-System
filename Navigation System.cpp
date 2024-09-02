#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <climits>
#include <unordered_map>
#include <algorithm>

using namespace std;

// Class representing a city
class City {
public:
    string name;
    unordered_map<string, int> neighbors; // Adjacency list of neighbors and corresponding weights
};

// Graph class representing the city model
class CityModel {
public:
    unordered_map<string, City> cities;

    void addCity(const string& cityName) {
        City city;
        city.name = cityName;
        cities[cityName] = city;
    }

    void addConnection(const string& city1, const string& city2, int weight) {
        cities[city1].neighbors[city2] = weight;
        cities[city2].neighbors[city1] = weight;
    }

    /*
    The findShortestPath function uses Dijkstra's algorithm to find the shortest path between two cities.
    */
    vector<string> findShortestPath(const string& startCity, const string& endCity) {
        unordered_map<string, string> parent; // To store the parent of each city in the shortest path
        unordered_map<string, int> distance; // To store the shortest distance from startCity to each city
        priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> pq; // Min-heap for Dijkstra's algorithm

        // Initialize distances to infinity and startCity distance to 0
        for (auto& city : cities) {
            distance[city.first] = INT_MAX;
        }
        distance[startCity] = 0;

        pq.push(make_pair(0, startCity));

        while (!pq.empty()) {
            string currentCity = pq.top().second;
            pq.pop();

            for (const auto& neighbor : cities[currentCity].neighbors) {
                string neighborCity = neighbor.first;
                int weight = neighbor.second;
                int newDistance = distance[currentCity] + weight;

                if (newDistance < distance[neighborCity]) {
                    distance[neighborCity] = newDistance;
                    parent[neighborCity] = currentCity;
                    pq.push(make_pair(newDistance, neighborCity));
                }
            }
        }

        // Reconstruct the shortest path using the parent mapping
        vector<string> shortestPath;
        string city = endCity;
        while (city != startCity) {
            shortestPath.push_back(city);
            city = parent[city];
        }
        shortestPath.push_back(startCity);
        reverse(shortestPath.begin(), shortestPath.end());

        return shortestPath;
    }
    
    /*
    The findBridges function finds all the bridges in the city model.
    uses a modified version of Depth-First Search (DFS) algorithm to identify the bridges.
    */
    vector<vector<string>> findBridges() {
        vector<vector<string>> bridges;
        unordered_map<string, int> discovery; // Time of first discovery for each city
        unordered_map<string, int> low; // Lowest discovery time reachable from a city
        unordered_map<string, bool> visited; // Whether a city has been visited during the DFS
        int timer = 0;

        for (auto& city : cities) {
            discovery[city.first] = -1;
            low[city.first] = -1;
            visited[city.first] = false;
        }

        for (auto& city : cities) {
            if (!visited[city.first]) {
                findBridgesUtil(city.first, "", discovery, low, bridges, timer, visited);
            }
        }

        return bridges;
    }

    /*
    function finds the minimum spanning tree of the city model.
    A minimum spanning tree is a subgraph that connects all the cities with theminimum total edge weight. 
    It uses Kruskal's algorithm to find the minimum spanning tree.
    */
    vector<vector<string>> findMinimumSpanningTree() {
        vector<vector<string>> minimumSpanningTree;
        vector<pair<int, pair<string, string>>> edges;

        // Create a list of all edges
        for (const auto& city : cities) {
            string city1 = city.first;
            for (const auto& neighbor : city.second.neighbors) {
                string city2 = neighbor.first;
                int weight = neighbor.second;
                edges.push_back(make_pair(weight, make_pair(city1, city2)));
            }
        }

        // Sort edges based on weights in ascending order
        sort(edges.begin(), edges.end());

        // Create a parent mapping for each city
        unordered_map<string, string> parent;
        for (const auto& city : cities) {
            parent[city.first] = city.first;
        }

        int numEdges = 0;
        int numCities = cities.size();
        for (const auto& edge : edges) {
            string city1 = edge.second.first;
            string city2 = edge.second.second;

            string parent1 = findParent(parent, city1);
            string parent2 = findParent(parent, city2);

            if (parent1 != parent2) {
                minimumSpanningTree.push_back({city1, city2});
                numEdges++;
                parent[parent1] = parent2;

                if (numEdges == numCities - 1) {
                    break;
                }
            }
        }

        return minimumSpanningTree;
    }

    void displayCityMap() {
        for (const auto& city : cities) {
            cout << "City: " << city.first << endl;
            cout << "Neighbors: ";
            for (const auto& neighbor : city.second.neighbors) {
                cout << neighbor.first << "(" << neighbor.second << ") ";
            }
            cout << endl;
        }
    }

private:
    void findBridgesUtil(const string& currentCity, const string& parentCity, unordered_map<string, int>& discovery, unordered_map<string, int>& low,
                        vector<vector<string>>& bridges, int& timer, unordered_map<string, bool>& visited) {
        visited[currentCity] = true;
        discovery[currentCity] = low[currentCity] = ++timer;

        for (const auto& neighbor : cities[currentCity].neighbors) {
            string neighborCity = neighbor.first;

            if (neighborCity == parentCity)
                continue;

            if (!visited[neighborCity]) {
                findBridgesUtil(neighborCity, currentCity, discovery, low, bridges, timer, visited);
                low[currentCity] = min(low[currentCity], low[neighborCity]);

                if (low[neighborCity] > discovery[currentCity]) {
                    bridges.push_back({currentCity, neighborCity});
                }
            }
            else {
                low[currentCity] = min(low[currentCity], discovery[neighborCity]);
            }
        }
    }

    string findParent(unordered_map<string, string>& parent, const string& city) {
        if (parent[city] != city) {
            parent[city] = findParent(parent, parent[city]);
        }
        return parent[city];
    }
};


int main() {
    CityModel cityModel;

    // Add cities
    cityModel.addCity("A");
    cityModel.addCity("B");
    cityModel.addCity("C");
    cityModel.addCity("D");
    cityModel.addCity("E");

    // Add connections with weights
    cityModel.addConnection("A", "B", 2);
    cityModel.addConnection("A", "C", 4);
    cityModel.addConnection("B", "C", 1);
    cityModel.addConnection("B", "D", 4);
    cityModel.addConnection("B", "E", 2);
    cityModel.addConnection("C", "D", 1);

    // Variables for menu selection
    int choice = -1;
    while (choice != 0) {
        // Display menu
        cout << "=== MENU ===" << endl;
        cout << "1. Find shortest path" << endl;
        cout << "2. Find bridges" << endl;
        cout << "3. Find minimum spanning tree" << endl;
        cout << "4. Find minimum distance between two cities" << endl;
        cout << "5. Display City Map" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string startCity, endCity;
                cout << "Enter the start city: ";
                cin >> startCity;
                cout << "Enter the end city: ";
                cin >> endCity;

                vector<string> shortestPath = cityModel.findShortestPath(startCity, endCity);

                cout << "Shortest path: ";
                for (const auto& city : shortestPath) {
                    cout << city << " ";
                }
                cout << endl;
                break;
            }
            case 2: {
                vector<vector<string>> bridges = cityModel.findBridges();

                cout << "Bridges: " << endl;
                for (const auto& bridge : bridges) {
                    cout << bridge[0] << " - " << bridge[1] << endl;
                }
                break;
            }
            case 3: {
                vector<vector<string>> minimumSpanningTree = cityModel.findMinimumSpanningTree();

                cout << "Minimum spanning tree: " << endl;
                for (const auto& edge : minimumSpanningTree) {
                    cout << edge[0] << " - " << edge[1] << endl;
                }
                break;
            }
            case 4: {
                string city1, city2;
                cout << "Enter the first city: ";
                cin >> city1;
                cout << "Enter the second city: ";
                cin >> city2;

                vector<string> shortestPath = cityModel.findShortestPath(city1, city2);

                cout << "Minimum distance between " << city1 << " and " << city2 << ": " << shortestPath.size() - 1 << endl;
                break;
            }
            case 5: {
                cityModel.displayCityMap();
                break;
            }
            case 0:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }

        cout << endl;
    }
    
    return 0;
}
