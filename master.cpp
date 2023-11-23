#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <unistd.h>

class Player {
public:
    Player() : ind(0), prev(nullptr), next(nullptr) {}

    int ind;
    std::string song;
    Player* prev;
    Player* next;
};

class Playlist {
private:
    Player* head;
    Player* last;
    int num;
    
    int inPlaylist(const std::string& newSong) {
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
    void addSong(const std::string& newSong) {
        if (inPlaylist(newSong)) {
            std::cout << "Song is already in the playlist.\n";
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
            std::cout << "Your playlist is already empty.\n";
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
            std::cout << "Empty Playlist.\n";
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

    void displayPlaylist() {
        if (head == nullptr && last == nullptr) {
            std::cout << "Empty playlist.\n";
        } else {
            Player* temp = head;
            while (temp != nullptr) {
                std::cout << temp->ind << " " << temp->song << std::endl;
                temp = temp->next;
            }
        }
    }

    void saveToFile(const std::string& filename) {
        std::ofstream file(filename);

        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
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

    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);

        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return;
        }

        std::string song;
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
        std::cout << "To add a song to your playlist, enter 1:\n";
        std::cout << "To delete a song from your playlist, enter 2:\n";
        std::cout << "To delete the playlist, enter 3:\n";
        std::cout << "To display the playlist, enter 4:\n";
        std::cout << "To save to file, enter 5:\n";
        std::cout << "To load from file, enter 6:\n";
        std::cout << "To exit, enter 0:\n";

        std::cin >> choice;

        switch (choice) {
            case 1: {
                std::cout << "Enter the name of the song (without spaces): ";
                std::string newSong;
                std::cin >> newSong;
                playlist.addSong(newSong);
                break;
            }
            case 2: {
                playlist.displayPlaylist();
                Player* temp = playlist.getHead();
                std::cout << "Enter the serial number of the song you want to delete: ";
                if (temp != nullptr) {
                    int srn;
                    std::cin >> srn;
                    if (srn < 1 || srn > playlist.getNum()) {
                        std::cout << "Serial number not in range.\n";
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
                std::cout << "Enter the file name to save to: ";
                std::string filename;
                std::cin >> filename;
                playlist.saveToFile(filename);
                break;
            }
            case 6: {
                std::cout << "Enter the file name to load from: ";
                std::string filename;
                std::cin >> filename;
                playlist.loadFromFile(filename);
                break;
            }
            case 0:
                break;
            default:
                std::cout << "Invalid choice.\n";
        }

        std::cout << "If you want to perform another action, enter 1; otherwise, enter 0: ";
        std::cin >> op;

        if (op == 0) {
            break;
        }
    }

    std::cout << "Playlist:\n";
    std::cout << "*************\n";
    playlist.displayPlaylist();

    return 0;
}
