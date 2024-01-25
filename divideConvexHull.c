#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

typedef struct point{
    double x;
    double y;
    double angle;
}point;

void computeHullSetup(point points[], point* xMin, point* xMax);

void quickHull(point points[], point hullPoints[], point p1, point p2, int* indexHullPoints, int side);

int findSide(point p1, point p2, point p3);

int findLineToPointDist(point p1, point p2, point p3);

void sortClockwise(point hullPoints[], int numHullPoints);

int comparitor(const void *a, const void *b);

void printShortestPath(point hullPoints[], int numHullPoints, int indexS1, int indexS2);

double computeDistance(point p1, point p2);

int main(){
   clock_t start, end;
    int i = 0, totalHullPoints = 0, indexS1 = -1, indexS2 = -1;
    double totalTime = 0;
    point s1, s2, xMin, xMax;
    point fileArr[30000], hullP[30000];
    
    FILE* file = fopen("test.txt", "r");

    if(file == NULL){
        printf("ERROR: Failed to open file\n");
        exit(-1);
    }
    while (!feof (file)){
        fscanf(file, "%lf %lf", &fileArr[i].x, &fileArr[i].y);
        i++;
    }
    fclose(file);

    printf("Enter x and y coordinates of s1 separated by a space (x y): ");
    scanf("%lf %lf", &s1.x, &s1.y);
    printf("Enter x and y coordinates of s2 separated by a space (x y): ");
    scanf("%lf %lf", &s2.x, &s2.y);
    printf("Computing...\n");


    //Start timer, actual algo time now
    start = clock();
    computeHullSetup(fileArr, &xMin, &xMax);
    hullP[totalHullPoints++] = xMin;
    hullP[totalHullPoints++] = xMax;
    quickHull(fileArr, hullP, xMin, xMax, &totalHullPoints, 1);
    quickHull(fileArr, hullP, xMin, xMax, &totalHullPoints, -1);
    end = clock();
    totalTime = ((double)(end-start))/CLOCKS_PER_SEC;


    //Print out hull points
    printf("For Divide and Conquer Convex Hull Problem\n");
    printf("Time taken: %f\n", totalTime);
    printf("Number of TOTAL hull points: %d\nPrinting ALL of the total points:\n", totalHullPoints);

    sortClockwise(hullP, totalHullPoints);
    for(int i = 0; i < totalHullPoints; i++){
        printf("Hull Point %d: (%.1lf, %.1lf)\n", i+1, hullP[i].x, hullP[i].y);

    }
    //Getting index of s1 and s2
    for(int i = 0; i < totalHullPoints; i++){
        if(hullP[i].x == s1.x && hullP[i].y == s1.y){
            indexS1 = i;
        } 
        if(hullP[i].x == s2.x && hullP[i].y == s2.y){
            indexS2 = i;
        }
    }
    //Check if s1 and s2 are extreme points
    if(indexS1 != -1 && indexS2 != -1){
        printShortestPath(hullP, totalHullPoints, indexS1, indexS2);
    } else{
        printf("ERROR: S1 or S2 were not extreme points. Cannot find shortest path.\n");
    }
    return 0;
}

//Finds the point with the smallest and largest x value. Needed to start quickhull algo
void computeHullSetup(point points[], point *xMin, point *xMax){
    xMin->x = 99999;
    xMax->x = -99999;
    for(int i = 0; i < 30000; i++){
        if(points[i].x < xMin->x){
            xMin->x = points[i].x;
            xMin->y = points[i].y;
        }
        if(points[i].x > xMax->x){
            xMax->x = points[i].x;
            xMax->y = points[i].y;
        }
    }
}

//Generates all of the extreme points for a convex hull in a divide and conquer fashion (Similar to qsort)
void quickHull(point points[], point hullPoints[], point p1, point p2, int *indexHullPoints, int side){
    int index = -1, maxDist = 0, tempDist = 0, tempSide = 0;

    //Find point on same side that will generate a triangle with max area/distance from line
    for(int i = 0; i < 30000; i++){
        tempDist = findLineToPointDist(p1, p2, points[i]);
        tempSide = findSide(p1, p2, points[i]);
        if(tempSide == side && tempDist > maxDist){
            //Index of point that generate the max distance (along with maxDist)
            index = i;
            maxDist = tempDist;
        }
    }

    //If no greater point, we've found a pair of extreme points!
    if(index == -1){
        //This chuck of code is here to not include duplicate points
        bool isDupOne = false;
        bool isDupTwo = false;
        for(int k = 0; k < (*indexHullPoints); k++){
            if(p1.x == hullPoints[k].x && p1.y == hullPoints[k].y){
                isDupOne = true;
            }  
            if(p2.x == hullPoints[k].x && p2.y == hullPoints[k].y){
                isDupTwo = true;
            }                                  
        }
        if(!isDupOne){
            hullPoints[(*indexHullPoints)++] = p1;
        }
        if(!isDupTwo){
            hullPoints[(*indexHullPoints)++] = p2;
        }
        return;
    }
    
    //Recursively call quickhull, until all extreme points are found.
    quickHull(points, hullPoints, points[index], p1, indexHullPoints, -findSide(points[index], p1, p2));
    quickHull(points, hullPoints, points[index], p2, indexHullPoints, -findSide(points[index], p2, p1));
}

//Returns what side p3 lies on compared to the line created by connecting the points p1 and p2
int findSide(point p1, point p2, point p3){
    double val = (p2.y - p1.y) * (p3.x - p2.x) - (p2.x - p1.x) * (p3.y - p2.y);
    if(val > 0){
        return 1;
    } else if (val < 0){
        return -1;
    } else{
        return 0;
    }
}

//Finds distance proportional from p3 to the line created by p1 to p2
int findLineToPointDist(point p1, point p2, point p3){
    double val = (p2.y - p1.y) * (p3.x - p2.x) - (p2.x - p1.x) * (p3.y - p2.y);
    //Distance cannot be negative (its just in opposite direction, which is checked for in quickhull)
    if(val < 0){
        return (val * -1);
    } else{
        return val;
    }
}

//Sort the points clockwise based of off a given point (this case its the first hull point)
//This will organize the points such that the next point is the proper traversal of the hull points
void sortClockwise(point hullP[], int hullPIndex){
    point testP;
    //testP is just a point to compare the rest to. Doesn't matter which one we'll choose, the points will be ordered
    testP.x = hullP[1].x;
    testP.y = hullP[1].y;
    for(int i = 0; i < hullPIndex; i++){
        hullP[i].angle = atan2(hullP[i].y - testP.y, hullP[i].x - testP.x);
    }
    qsort(hullP, hullPIndex, sizeof(struct point), comparitor);
}

//Comparing function for qsort in sortClockwise. Compares the angles
int comparitor(const void *a, const void *b){
    struct point *p1 = (struct point *)a;
    struct point *p2 = (struct point *)b;

    if(p1->angle < p2->angle){
        return -1;
    } else if (p1->angle > p2->angle){
        return 1;
    } else{
        return 0;
    }
}

//This computes both paths from s1 to s2. Given the shorter path it prints the points from s1 to s2 inclusive and total distance
void printShortestPath(point hullP[], int sizeHullP, int indexS1, int indexS2){
    double distOne = 0, distTwo = 0;
    int j = indexS1, i = indexS1, pointsOnPath = 0;

    //Finding the total distance from s1 and s2 going one direction around
  while(true){
        if(i == sizeHullP - 1){
            if(hullP[i].x == hullP[indexS2].x && hullP[i].y == hullP[indexS2].y){
                break;
            } else{
                distOne += computeDistance(hullP[i], hullP[0]);
                i = 0;
                continue;
            }
        }
        if(hullP[i].x == hullP[indexS2].x && hullP[i].y == hullP[indexS2].y){
            break;
        }
        distOne += computeDistance(hullP[i], hullP[i+1]);
        i++;
    }
    //Finding the total distance from s1 and s2 going the other direction
    while(true){
        if(j == 0){
            if(hullP[j].x == hullP[indexS2].x && hullP[j].y == hullP[indexS2].y){
                break;
            } else{
                distTwo += computeDistance(hullP[j], hullP[sizeHullP - 1]);
                j = sizeHullP - 1;
                continue;
            }
        }
        if(hullP[j].x == hullP[indexS2].x && hullP[j].y == hullP[indexS2].y){
            break;
        }
        distTwo += computeDistance(hullP[j], hullP[j-1]);
        j--;
    }
   
    printf("\nThe points from s1 to s2 inclusive are:\n");
    //Based on which method generated the shortest distance, use that to print the points in order of traversal and total distance
    if(distOne < distTwo){
        i = indexS1;
        while(true){
            if(i == indexS2){
                printf("(%.1lf, %.1lf)\n", hullP[i].x, hullP[i].y);
                pointsOnPath++;
                break;
            }
            if(i == sizeHullP - 1){
                printf("(%.1lf, %.1lf)\n", hullP[i].x, hullP[i].y);
                pointsOnPath++;
                i = 0;
                continue;
            }
            printf("(%.1lf, %.1lf)\n", hullP[i].x, hullP[i].y);
            i++;
            pointsOnPath++;
        }
        printf("Total points on path: %d\n", pointsOnPath);
        printf("Total Distance: %.1lf\n", distOne);
    } else{
        j = indexS1;
        while(true){
            if(j == indexS2){
                printf("(%.1lf, %.1lf)\n", hullP[j].x, hullP[j].y);
                pointsOnPath++;
                break;
            }
            if(j == 0){
                printf("(%.1lf, %.1lf)\n", hullP[j].x, hullP[j].y);
                j = sizeHullP - 1;
                pointsOnPath++;
                continue;
            }
            printf("(%.1lf, %.1lf)\n", hullP[j].x, hullP[j].y);
            j--;
            pointsOnPath++;
        }
        printf("Total points on path: %d\n", pointsOnPath);
        printf("Total Distance: %.1lf\n", distTwo);
    }
}

//Finds distance btw two 2D points
double computeDistance(point p1, point p2){
    return sqrt(((p2.x - p1.x) * (p2.x - p1.x)) + ((p2.y - p1.y) * (p2.y - p1.y)));
}

