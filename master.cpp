#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <unistd.h>

using namespace std;

class Player {
public:
    Player() : ind(0), prev(nullptr), next(nullptr) {}

    int ind;
    string song;
    Player* prev;
    Player* next;
};

class Playlist {
private:
    Player* head;
    Player* last;
    int num;
    
    int inPlaylist(const string& newSong) {
        Player* temp = head;
        while (temp != nullptr) {
            if (newSong == temp->song)
                return 1;
            else
                temp = temp->next;
        }
        return 0;
    }

public:
    Playlist() : head(nullptr), last(nullptr), num(0) {}
    int getNum() const {
        return num;
    }
    Player* getHead() const {
        return head;
    }
    void addSong(const string& newSong) {
        if (inPlaylist(newSong)) {
            cout << "Song is already in the playlist.\n";
        } else {
            Player* newPlayer = new Player();
            newPlayer->song = newSong;
            newPlayer->ind = ++num;

            if (head == nullptr) {
                head = newPlayer;
                last = newPlayer;
            } else {
                newPlayer->prev = last;
                last->next = newPlayer;
                last = newPlayer;
            }
        }
    }

    void deleteSong(int n) {
        if (head == nullptr && last == nullptr) {
            cout << "Your playlist is already empty.\n";
        } else {
            Player* temp = head;

            if (n == 1) {
                head = head->next;
                if (head != nullptr) {
                    head->prev = nullptr;
                }
                delete temp;
                num--;

                int count = 1;
                Player* temp1 = head;
                while (count <= num) {
                    temp1->ind = count;
                    temp1 = temp1->next;
                    count++;
                }
            } else if (n == num) {
                temp = last;
                last = last->prev;
                if (last != nullptr) {
                    last->next = nullptr;
                }
                delete temp;
                num--;
                last->ind = num;
            } else {
                int count = 1;
                while (count < n) {
                    temp = temp->next;
                    count++;
                }
                Player* temp1 = temp->prev;
                temp1->next = temp->next;
                temp->next->prev = temp1;
                delete temp;
                num--;

                Player* temp2 = head;
                count = 1;
                while (count <= num) {
                    temp2->ind = count;
                    temp2 = temp2->next;
                    count++;
                }
            }
        }
    }

    void deletePlaylist() {
        Player* tempS = head;
        Player* tempD = head;
        Player* tempL = last;

        if (head == nullptr && last == nullptr) {
            cout << "Empty Playlist.\n";
            return;
        } else {
            while (tempS != tempL) {
                if (tempS == head) {
                    tempS = tempS->next;
                } else {
                    delete tempD;
                    tempD = tempS;
                    tempS = tempS->next;
                }
            }
            delete tempS;
            delete tempD;

            Player* newPlayer = new Player();
            head = last = newPlayer;
            newPlayer->song = "";
            num = 0;
        }
    }

    int displayPlaylist() {
        if (head == nullptr && last == nullptr) {
            cout << "Empty playlist.\n";
            return 0;
        } else {
            Player* temp = head;
            while (temp != nullptr) {
                cout << temp->ind << " " << temp->song << endl;
                temp = temp->next;
                return 1;
            }
        }
    }

    void saveToFile(const string& filename) {
        ofstream file(filename);

        if (!file.is_open()) {
            cerr << "Error opening file: " << filename << endl;
            return;
        }

        Player* temp = head;
        while (temp != nullptr) {
            if (!temp->song.empty()) {
                file << temp->song << " ";
            }
            temp = temp->next;
        }

        file.close();
    }

    void loadFromFile(const string& filename) {
        ifstream file(filename);

        if (!file.is_open()) {
            cerr << "Error opening file: " << filename << endl;
            return;
        }

        string song;
        while (file >> song) {
            addSong(song);
        }

        file.close();
    }
};

int main() {
    Playlist playlist;

    int choice;
    int op = 1;
    while (op == 1) {
        cout << "To add a song to your playlist, enter 1:\n";
        cout << "To delete a song from your playlist, enter 2:\n";
        cout << "To delete the playlist, enter 3:\n";
        cout << "To display the playlist, enter 4:\n";
        cout << "To save to file, enter 5:\n";
        cout << "To load from file, enter 6:\n";
        cout << "To exit, enter 0:\n";

        cin >> choice;

        switch (choice) {
            case 1: {
                cout << "Enter the name of the song (without spaces): ";
                string newSong;
                cin >> newSong;
                playlist.addSong(newSong);
                break;
            }
            case 2: {
                int notEmpty = playlist.displayPlaylist();
                Player* temp = playlist.getHead();
                if (temp != nullptr) {
                     cout << "Enter the serial number of the song you want to delete: ";
                    int srn;
                    cin >> srn;
                    if (srn < 1 || srn > playlist.getNum()) {
                        cout << "Serial number not in range.\n";
                    } else {
                        playlist.deleteSong(srn);
                    }
                }
                
                break;
            }
            case 3:
                playlist.deletePlaylist();
                break;
            case 4:
                playlist.displayPlaylist();
                break;
            case 5: {
                cout << "Enter the file name to save to: ";
                string filename;
                cin >> filename;
                playlist.saveToFile(filename);
                break;
            }
            case 6: {
                cout << "Enter the file name to load from: ";
                string filename;
                cin >> filename;
                playlist.loadFromFile(filename);
                break;
            }
            case 0:
                break;
            default:
                cout << "Invalid choice.\n";
        }

        cout << "If you want to perform another action, enter 1; otherwise, enter 0: ";
        cin >> op;

        if (op == 0) {
            break;
        }
    }

    cout << "Playlist:\n";
    cout << "*****\n";
    playlist.displayPlaylist();

    return 0;
}