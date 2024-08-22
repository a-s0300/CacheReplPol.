#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <unordered_set>
#include <limits.h>
using namespace std;
typedef vector<string> vs;
typedef vector<int> vi;
typedef vector<string>::iterator vsi;
typedef unordered_set<string>::iterator usi;
string a[50000];
int main(int argc, char** argv) {
    ifstream fileHandler(argv[3]);
    int count=0;
    while (!fileHandler.eof())
    {
        string  x;
        fileHandler >> x;
        a[count]=(x);
        count++;
    }
    fileHandler.close();
    int blockSize=stoi(argv[2]);
    cout<<"Cache Size : "<<blockSize<<endl;
    string policy[] = {"LRU","OPTIMAL","FIFO"};
    //CHECK FOR POLICY {LRU OR OPTIMAL}
    //FOR LRU
    int access=0; int hits=0; int miss=0; int compMiss=0; int capMiss=0;
    unordered_map<string,int>::iterator itr;
    //for LRU
    if(argv[1]==policy[0]){
        unordered_map<string,int> lru;
        vs::iterator it;
        cout<<"LRU USED"<<endl;
        //code for LRU
        vs* cache = new vector<string>();
        for(int i=0;i<count;i++) {
            if(a[i]=="" || a[i]==" ") {
                continue;
            }
            access++;
            itr=lru.find(a[i]);
            if(itr==lru.end()) {
                compMiss++;
                lru.insert({a[i],1});
            }
            if(cache->empty()) {
                miss++;
                cache->push_back(a[i]);
                continue;
            }
            it = find(cache->begin(),cache->end(), a[i]);

            if(itr!=lru.end() && it==cache->end()) {
                capMiss++;
            }

            if(cache->size()<blockSize) {
                if(it==cache->end()) {
                    miss++;
                    cache->push_back(a[i]);
                    continue;
                }
                hits++;
                int index = it-cache->begin();
                for(int i=index;i<cache->size()-1;i++) {
                    cache->at(i)=cache->at(i+1);
                }
                cache->at(cache->size()-1)=a[i];
                continue;
            }
            if(it==cache->end()) {
                miss++;
                for(int i=0;i<blockSize-1;i++) {
                    cache->at(i)=cache->at(i+1);
                }
                cache->at(blockSize-1)=a[i];
            }
            if(it!=cache->end()){
                hits++;
                int index = it-cache->begin();
                for(int i=index;i<blockSize-1;i++) {
                    cache->at(i)=cache->at(i+1);
                }
                cache->at(blockSize-1)=a[i];
            }
        }
        cout<<"ACCESS : "<<access<<endl<<"HITS : "<<hits<<endl;
        cout<<"MISS : "<<miss<<endl<<"Compulsory Misses : "<<compMiss<<endl<<"Capacity Misses : "<<capMiss<<endl;
    }

    //FOR OPTIMAL

    if(argv[1]==policy[1]) {
        cout<<"OPTIMAL USED"<<endl;
        vs* cach = new vs();
        unordered_set<string> cachSet;
        usi iter;
        for(int i=0;i<count;i++) {
            if(a[i]=="" || a[i]==" ") {
                continue;
            }
            access++;
            if(cach->empty()) {
                miss++;
                compMiss++;
                cach->push_back(a[i]);
                cachSet.insert(a[i]);
                continue;
            }
            vsi itr;
            itr = find(cach->begin(),cach->end(),a[i]);
            iter = cachSet.find(a[i]);
            if(iter==cachSet.end()) {
                compMiss++;
                cachSet.insert(a[i]);
            }
            if(iter!=cachSet.end() && itr==cach->end()) {
                capMiss++;
            }
            if(cach->size()<blockSize) {
                if(itr==cach->end()) {
                    miss++;
                    cach->push_back(a[i]);
                }
                else if(itr!=cach->end()) {
                    hits++;
                }
                continue;
            }
            if(itr==cach->end()) {
                miss++;
                int optIndex=-1;
                int optCount=0;
                for(int p=0;p<blockSize;p++) {
                    int temp=0;
                    for(int q=i;q<count;q++) {
                        if(cach->at(p)!=a[q])
                            temp++;
                        else if(cach->at(p)==a[q])
                            break;
                    }
                    if(temp>optCount) {
                        optIndex=p;
                        optCount=temp;
                    }
                }
                cach->at(optIndex)=a[i];
                continue;
            }
            if(itr!=cach->end()) {
                hits++;
            }

        }
        cout<<"ACCESS : "<<access<<endl;
        cout<<"MISS : "<<miss<<endl<<"Compulsory Misses : "<<compMiss<<endl<<"Capacity Misses : "<<capMiss<<endl;
    }
}

