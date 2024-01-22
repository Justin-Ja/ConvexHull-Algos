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

int computeHull(point points[], point hullPoints[]);

int side(point p1, point p2, point point);

void sortClockwise(point hullPoints[], int numHullPoints);

int comparitor(const void *a, const void *b);

void printShortestPath(point hullPoints[], int numHullPoints, int indexS1, int indexS2);

double computeDistance(point p1, point p2);

int main(){
    clock_t start, end;
    int i = 0, totalHullPoints, indexS1 = -1, indexS2 = -1;
    double totalTime = 0;
    point s1, s2;
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
    totalHullPoints = computeHull(fileArr, hullP);
    end = clock();
    totalTime = ((double)(end-start))/CLOCKS_PER_SEC;

    printf("For Brute force Convex Hull problem\n");
    printf("Time taken: %f\nNumber of hull points found: %d\n", totalTime, totalHullPoints);
    printf("Printing out ALL of the hull points\n");
    for(int i = 0; i < totalHullPoints; i++){
        printf("Hull Point %d: (%.1lf, %.1lf)\n", i+1, hullP[i].x, hullP[i].y);
    }
    //Sort points clockwise, for easy traversal from s1 to s2 (sorts points so that the next point is graphically next in line)
    sortClockwise(hullP, totalHullPoints);

    //Getting index of s1 and s2
    for(int i = 0; i < totalHullPoints; i++){
        if(hullP[i].x == s1.x && hullP[i].y == s1.y){
            indexS1 = i;
        } 
        if(hullP[i].x == s2.x && hullP[i].y == s2.y){
            indexS2 = i;
        }
    }

    //Check if points are acutally extreme points
    if(indexS1 != -1 && indexS2 != -1){
        printShortestPath(hullP, totalHullPoints, indexS1, indexS2);
    } else{
        printf("ERROR: S1 or S2 were not extreme points. Cannot find shortest path.\n");
    }
    return 0;
}

//Bruteforce method for finding all extreme points of a convex hull
int computeHull(point points[30000], point hullPoints[30000]){
    int totalHullPoints = 0;
    for(int i = 0; i < 30000; i++){
        for(int j = i+1; j < 30000; j++){
            int initSide = 0;
            bool isExtremePair = true;
            if(points[i].x == points[j].x && points[i].y == points[j].y){
                //Duplicate point, should not be considered
                break;
            }
            for(int k = 0; k < 30000; k++){
                //Find what side of point k lies on line make by i or j
                int pointSide = side(points[i], points[j], points[k]);
                if(pointSide == 0){
                    //Point k lies on line ij, dont consider this pair
                    continue;
                }

                if(initSide == 0){
                    initSide = pointSide;
                }
                if((initSide > 0 && pointSide < 0) || (initSide < 0 && pointSide > 0)){
                    isExtremePair = false;
                    break;
                }
            }
            //In j Loop
            if(isExtremePair){
                //This chuck of code is here to not include duplicate points (Otherwise you would get 44 Hull points in test.txt)
                bool isDupi = false;
                bool isDupj = false;
                for(int k = 0; k < totalHullPoints; k++){
                    if(points[i].x == hullPoints[k].x && points[i].y == hullPoints[k].y){
                        isDupi = true;
                    }  
                    if(points[j].x == hullPoints[k].x && points[j].y == hullPoints[k].y){
                        isDupj = true;
                    }                                  
                }
                //Adding not duplicate, extreme points to hullPoints
                if(!isDupi){
                    hullPoints[totalHullPoints++] = points[i];
                }
                if(!isDupj){
                    hullPoints[totalHullPoints++] = points[j];
                }
            }
        }
    }
    return totalHullPoints;
}

//Given a line created by p1 and p2, determine which side of the line p3 lies on
int side(point p1, point p2, point p3){
    double val = (p2.y - p1.y) * (p3.x - p2.x) - (p2.x - p1.x) * (p3.y - p2.y);
    if(val > 0){
        return 1;
    } else if (val < 0){
        return -1;
    } else{
        return 0;
    }
}

//Sort the points clockwise based of off a given point (this case its the first hull point)
//This will organize the points such that the next point is the proper traversal of the hull points
void sortClockwise(point hullP[], int hullPIndex){
    point testPoint;
    //testP is just a point to compare the rest to. Doesn't matter which one we'll choose, the points will be ordered
    testPoint.x = hullP[0].x;
    testPoint.y = hullP[0].y;
    for(int i = 0; i < hullPIndex; i++){
        hullP[i].angle = atan2(hullP[i].y - testPoint.y, hullP[i].x - testPoint.x);
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

//Computes the distance between two points
double computeDistance(point p1, point p2){
    return sqrt(((p2.x - p1.x) * (p2.x - p1.x)) + ((p2.y - p1.y) * (p2.y - p1.y)));
}
    