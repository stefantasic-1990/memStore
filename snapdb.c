int hashTableSize = 10;

unsigned int hash(char* key) {
    unsigned int hashValue = 0;
    while (*key) {
        hashValue = hashValue << 5;
        hashValue += *key;
        *key++;
    }
    return hashValue % hashTableSize;
}

int main(int argc, char* argv) {

}