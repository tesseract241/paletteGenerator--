#include <cassert>
#include <cmath>
#include <random>
#include <vector>
#include <paletteGenerator.hpp>

void coordinatesFromIndex(const int index, const int edge, int coordinates[]){
    assert(coordinates && "coordinatesFromIndex called with uninitialized coordinates\n");
    coordinates[0] = index/(edge*edge);
    int dummy = index - coordinates[0]*edge*edge;
    coordinates[1] = dummy/edge;
    coordinates[2] = dummy - coordinates[1]*edge;
}

void rotatecolourspace(const float inColours[], float outColours[]){
    outColours[0] = (inColours[0] + inColours[2])/2.;
    outColours[1] = (inColours[1] + inColours[2])/2.;
    outColours[2] = (inColours[0] + inColours[1])/2.;
}

int findEdge(int numberOfColours){
    float delta = std::cbrt(std::sqrt(27*27*numberOfColours*numberOfColours + 27*4*numberOfColours) + (27*numberOfColours + 2)/2.);
    return int(std::ceil((delta + 1/delta + 1.)/3.));
}

void generatePalette(int numberOfColours, float *colours){
    assert(numberOfColours>0 && "generatePalette called with non-positive numberOfColours");
    assert(colours && "generatePalette called with uninitialized colours\n");
    int nodesPerSide = findEdge(numberOfColours);
    int skipThreshold = int(std::ceil(nodesPerSide/6.));
    float nodeSize = 1./float(nodesPerSide - 1);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<int> randomColoursVector(nodesPerSide*nodesPerSide*nodesPerSide);
    for(int i=0;i<nodesPerSide*nodesPerSide*nodesPerSide;++i){
        randomColoursVector[i] = i;
    }
    int randomColours[nodesPerSide*nodesPerSide*nodesPerSide];
    for(int i=0;i<nodesPerSide*nodesPerSide*nodesPerSide;++i){
        std::uniform_int_distribution<> distrib(0, randomColoursVector.size() - 1);
        int index = distrib(gen);
        randomColours[i] = randomColoursVector[index];
        randomColoursVector.erase(randomColoursVector.begin() + index);
    }
    int j=0;
    for(int i=0;i<numberOfColours;++i){
        int indexes[3];
        for( ; ;++j){
            coordinatesFromIndex(randomColours[j], nodesPerSide, indexes);
            if(
                    std::abs(indexes[0]-indexes[1]>= skipThreshold) ||
                    std::abs(indexes[1]-indexes[2]>= skipThreshold) ||
                    std::abs(indexes[0]-indexes[2]>= skipThreshold)
            ){
                ++j;
                break;
            }
        }
        float fColours[3];
        for(int k=0;k<3;++k){
            fColours[k] = nodeSize * float(indexes[k]);
        }
        rotatecolourspace(fColours, colours+ 3*i);
    }
}
